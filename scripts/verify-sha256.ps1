# PhotonicSort — SHA-256 verification (Windows PowerShell 5.1+ / PowerShell 7+)
#
# From repository root:
#   pwsh -File .\scripts\verify-sha256.ps1
#   powershell -ExecutionPolicy Bypass -File .\scripts\verify-sha256.ps1
#
# Error-handling model (see docs/POWERSHELL_ERROR_HANDLING.md):
#   - Cmdlets: $ErrorActionPreference = 'Stop' (non-terminating → terminating)
#   - Native git: check $LASTEXITCODE after each call (exit codes do not throw)
#   - Aggregate failures in $script:FailCount; always exit 0/1 at end
#   - try/catch around I/O and crypto; finally for IDisposable cleanup
#Requires -Version 5.1

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'

$Pin = '841882d906dc3fbf3bbc5c8dfa97f2773c0c1818'
$script:FailCount = 0

function Write-Pass([string]$Message) { Write-Host "PASS  $Message" -ForegroundColor Green }
function Write-Fail([string]$Message) { Write-Host "FAIL  $Message" -ForegroundColor Red }
function Write-Info([string]$Message) { Write-Host $Message }

function Add-Failure([string]$Message) {
    $script:FailCount++
    Write-Fail $Message
}

function Get-RepoRoot {
    if (-not $PSScriptRoot) {
        throw 'PSScriptRoot is empty; run this script as a file (pwsh -File ...), not via stdin paste without path context.'
    }
    $parent = Join-Path -Path $PSScriptRoot -ChildPath '..'
    try {
        return (Resolve-Path -LiteralPath $parent -ErrorAction Stop).Path
    } catch {
        throw "Cannot resolve repository root from '$parent': $($_.Exception.Message)"
    }
}

function Get-Sha256File {
    param(
        [Parameter(Mandatory)]
        [ValidateNotNullOrEmpty()]
        [string]$Path
    )
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "File not found: $Path"
    }
    try {
        return (Get-FileHash -Algorithm SHA256 -LiteralPath $Path -ErrorAction Stop).Hash.ToLowerInvariant()
    } catch {
        throw "Get-FileHash failed for '$Path': $($_.Exception.Message)"
    }
}

function Get-Sha256Bytes {
    param(
        [Parameter(Mandatory)]
        [ValidateNotNull()]
        [byte[]]$Bytes
    )
    $sha = $null
    try {
        $sha = [System.Security.Cryptography.SHA256]::Create()
        $hash = $sha.ComputeHash($Bytes)
        return ([BitConverter]::ToString($hash) -replace '-', '').ToLowerInvariant()
    } catch {
        throw "SHA256 ComputeHash failed: $($_.Exception.Message)"
    } finally {
        if ($null -ne $sha) { $sha.Dispose() }
    }
}

function Invoke-Git {
    <#
    .SYNOPSIS
      Run git and fail on non-zero exit. Captures stdout as string.
    .NOTES
      Native apps do not honor $ErrorActionPreference; always check $LASTEXITCODE.
    #>
    param(
        [Parameter(Mandatory)]
        [string[]]$Args,
        [switch]$AllowFail
    )
    $prevEap = $ErrorActionPreference
    # Avoid Stop turning native stderr noise into script-terminating in some hosts
    $ErrorActionPreference = 'Continue'
    try {
        $output = & git @Args 2>&1
        $code = $LASTEXITCODE
    } finally {
        $ErrorActionPreference = $prevEap
    }
    if (-not $AllowFail -and $code -ne 0) {
        $msg = if ($output) { ($output | Out-String).Trim() } else { "(no output)" }
        throw "git $($Args -join ' ') failed (exit $code): $msg"
    }
    return [pscustomobject]@{ ExitCode = $code; Output = $output }
}

function Test-GitAvailable {
    if (-not (Get-Command -Name git -ErrorAction SilentlyContinue)) {
        return $false
    }
    $r = Invoke-Git -Args @('rev-parse', '--is-inside-work-tree') -AllowFail
    return ($r.ExitCode -eq 0)
}

function Test-GitPinPresent {
    param([Parameter(Mandatory)][string]$Commit)
    $r = Invoke-Git -Args @('cat-file', '-e', "${Commit}^{commit}") -AllowFail
    return ($r.ExitCode -eq 0)
}

function Get-GitBlobBytes {
    param(
        [Parameter(Mandatory)]
        [ValidateNotNullOrEmpty()]
        [string]$RevPath   # e.g. 841882d:photonic_sort.py
    )
    $rev = Invoke-Git -Args @('rev-parse', $RevPath)
    $blob = ($rev.Output | Out-String).Trim()
    if ([string]::IsNullOrWhiteSpace($blob)) {
        throw "git rev-parse returned empty blob id for $RevPath"
    }

    # Binary-safe: do not pipe through PowerShell strings (encoding corruption).
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = 'git'
    $psi.Arguments = "cat-file -p $blob"
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $psi.UseShellExecute = $false
    $psi.CreateNoWindow = $true

    $proc = $null
    $ms = $null
    try {
        $proc = [System.Diagnostics.Process]::Start($psi)
        if ($null -eq $proc) { throw "Failed to start git for $RevPath" }
        $ms = New-Object System.IO.MemoryStream
        $proc.StandardOutput.BaseStream.CopyTo($ms)
        $stderr = $proc.StandardError.ReadToEnd()
        $proc.WaitForExit()
        if ($proc.ExitCode -ne 0) {
            throw "git cat-file -p $blob failed (exit $($proc.ExitCode)): $stderr"
        }
        return $ms.ToArray()
    } catch {
        throw "Get-GitBlobBytes($RevPath): $($_.Exception.Message)"
    } finally {
        if ($null -ne $ms) { $ms.Dispose() }
        if ($null -ne $proc) { $proc.Dispose() }
    }
}

function Test-SumsFile {
    param(
        [Parameter(Mandatory)][string]$SumsPath,
        [Parameter(Mandatory)][string]$Title,
        [Parameter(Mandatory)][string]$RepoRoot
    )
    Write-Info "--- $Title ---"
    if (-not (Test-Path -LiteralPath $SumsPath -PathType Leaf)) {
        Add-Failure "MISSING sums file: $SumsPath"
        Write-Host ''
        return
    }

    $lineNo = 0
    try {
        $lines = Get-Content -LiteralPath $SumsPath -ErrorAction Stop
    } catch {
        Add-Failure "Cannot read sums file '$SumsPath': $($_.Exception.Message)"
        Write-Host ''
        return
    }

    foreach ($line in $lines) {
        $lineNo++
        $t = $line.Trim()
        if ($t -eq '' -or $t.StartsWith('#')) { continue }

        if ($t -notmatch '^([0-9a-fA-F]{64})\s+(.+)$') {
            Write-Info "SKIP line ${lineNo}: $t"
            continue
        }

        $want = $Matches[1].ToLowerInvariant()
        $relUnix = $Matches[2].Trim()
        $relWin = $relUnix.Replace('/', [IO.Path]::DirectorySeparatorChar)
        $full = Join-Path -Path $RepoRoot -ChildPath $relWin

        if (-not (Test-Path -LiteralPath $full -PathType Leaf)) {
            Add-Failure "MISSING  $relUnix"
            continue
        }

        try {
            $got = Get-Sha256File -Path $full
        } catch {
            Add-Failure "HASHERR $relUnix : $($_.Exception.Message)"
            continue
        }

        if ($got -eq $want) {
            Write-Host "OK  $got  $relUnix"
        } else {
            Add-Failure "BAD $relUnix"
            Write-Host "  got:  $got"
            Write-Host "  want: $want"
        }
    }
    Write-Host ''
}

# --- main ---
$exitCode = 0
try {
    $Root = Get-RepoRoot
    Set-Location -LiteralPath $Root

    Write-Info '========================================================================'
    Write-Info 'PhotonicSort SHA-256 verification (PowerShell)'
    Write-Info "Repo root: $Root"
    Write-Info "Implementation pin: $Pin"
    Write-Info "ErrorActionPreference: $ErrorActionPreference | StrictMode: Latest"
    Write-Info '========================================================================'
    Write-Host ''

    Test-SumsFile -SumsPath (Join-Path $Root 'SHA256SUMS_python.txt') `
                  -Title 'Python (SHA256SUMS_python.txt)' -RepoRoot $Root
    Test-SumsFile -SumsPath (Join-Path $Root 'c\SHA256SUMS') `
                  -Title 'C code (c/SHA256SUMS)' -RepoRoot $Root

    if (-not (Test-GitAvailable)) {
        Write-Info '(skip git pin checks — git not available or not a work tree)'
        Write-Host ''
    } elseif (-not (Test-GitPinPresent -Commit $Pin)) {
        Write-Info "(skip git pin checks — object $Pin not in this clone; try: git fetch --tags)"
        Write-Host ''
    } else {
        Write-Info "--- Per-file digests @ $Pin ---"
        $items = @(
            @('photonic_sort.py', '2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c'),
            @('pyproject.toml', '2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d'),
            @('tests/test_photonic_sort.py', '03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798'),
            @('c/photonic_sort.c', 'e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54'),
            @('c/photonic_sort.h', '16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622')
        )
        foreach ($it in $items) {
            $path = $it[0]
            $want = $it[1]
            try {
                $bytes = Get-GitBlobBytes -RevPath "${Pin}:${path}"
                $got = Get-Sha256Bytes -Bytes $bytes
                if ($got -eq $want) {
                    Write-Host "OK  $got  $path"
                } else {
                    Add-Failure "BAD $path"
                    Write-Host "  got:  $got"
                    Write-Host "  want: $want"
                }
            } catch {
                Add-Failure "ERR $path : $($_.Exception.Message)"
            }
        }
        Write-Host ''
    }

    Write-Info '========================================================================'
    if ($script:FailCount -eq 0) {
        Write-Pass 'All SHA-256 verification commands PASSED.'
        $exitCode = 0
    } else {
        Write-Fail "One or more SHA-256 checks FAILED ($($script:FailCount))."
        $exitCode = 1
    }
} catch {
    # Unexpected terminating error (strict mode, resolve path, etc.)
    Write-Fail "Unhandled error: $($_.Exception.Message)"
    if ($_.InvocationInfo) {
        Write-Host "  at $($_.InvocationInfo.ScriptName):$($_.InvocationInfo.ScriptLineNumber)"
    }
    Write-Host "  $($_.ScriptStackTrace)"
    $exitCode = 2
}

exit $exitCode
