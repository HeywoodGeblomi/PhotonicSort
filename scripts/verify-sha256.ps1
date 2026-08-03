# PhotonicSort — SHA-256 verification (Windows PowerShell 5.1+ / PowerShell 7+)
# From repository root:
#   pwsh -File .\scripts\verify-sha256.ps1
#   powershell -ExecutionPolicy Bypass -File .\scripts\verify-sha256.ps1
#Requires -Version 5.1
$ErrorActionPreference = 'Stop'

$Pin = '841882d906dc3fbf3bbc5c8dfa97f2773c0c1818'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
Set-Location $Root
$fail = 0

function Get-Sha256File([string]$Path) {
    return (Get-FileHash -Algorithm SHA256 -Path $Path).Hash.ToLowerInvariant()
}

function Get-Sha256Bytes([byte[]]$Bytes) {
    $sha = [System.Security.Cryptography.SHA256]::Create()
    try {
        $hash = $sha.ComputeHash($Bytes)
        return ([BitConverter]::ToString($hash) -replace '-', '').ToLowerInvariant()
    } finally { $sha.Dispose() }
}

function Get-GitBlobBytes([string]$RevPath) {
    # Binary-safe: git cat-file -p <blob>
    $blob = & git rev-parse $RevPath 2>$null
    if ($LASTEXITCODE -ne 0) { throw "missing $RevPath" }
    $tmp = [IO.Path]::GetTempFileName()
    try {
        & git cat-file -p $blob | Set-Content -Path $tmp -Encoding Byte -ErrorAction SilentlyContinue
        # Set-Content -Encoding Byte is PS5; PS7 uses differently — use process redirect
    } catch {}
    Remove-Item -Force $tmp -ErrorAction SilentlyContinue

    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = 'git'
    $psi.Arguments = "cat-file -p $blob"
    $psi.RedirectStandardOutput = $true
    $psi.UseShellExecute = $false
    $psi.CreateNoWindow = $true
    $proc = [Diagnostics.Process]::Start($psi)
    $ms = New-Object System.IO.MemoryStream
    $proc.StandardOutput.BaseStream.CopyTo($ms)
    $proc.WaitForExit()
    if ($proc.ExitCode -ne 0) { throw "git cat-file failed for $RevPath" }
    return $ms.ToArray()
}

function Test-SumsFile([string]$SumsPath, [string]$Title) {
    Write-Host "--- $Title ---"
    if (-not (Test-Path -LiteralPath $SumsPath)) {
        Write-Host "MISSING $SumsPath"
        $script:fail++
        Write-Host ''
        return
    }
    foreach ($line in Get-Content -LiteralPath $SumsPath) {
        $t = $line.Trim()
        if ($t -eq '' -or $t.StartsWith('#')) { continue }
        if ($t -notmatch '^([0-9a-fA-F]{64})  (.+)$' -and $t -notmatch '^([0-9a-fA-F]{64})\s+(.+)$') {
            Write-Host "SKIP $t"
            continue
        }
        if ($t -match '^([0-9a-fA-F]{64})\s+(.+)$') {
            $want = $Matches[1].ToLowerInvariant()
            $relUnix = $Matches[2].Trim()
        } else { continue }
        $relWin = $relUnix.Replace('/', [IO.Path]::DirectorySeparatorChar)
        $full = Join-Path $Root $relWin
        if (-not (Test-Path -LiteralPath $full)) {
            Write-Host "MISSING  $relUnix"
            $script:fail++
            continue
        }
        $got = Get-Sha256File $full
        if ($got -eq $want) {
            Write-Host "OK  $got  $relUnix"
        } else {
            Write-Host "BAD $relUnix"
            Write-Host "  got:  $got"
            Write-Host "  want: $want"
            $script:fail++
        }
    }
    Write-Host ''
}

Write-Host '========================================================================'
Write-Host 'PhotonicSort SHA-256 verification (PowerShell)'
Write-Host "Repo root: $Root"
Write-Host "Implementation pin: $Pin"
Write-Host '========================================================================'
Write-Host ''

Test-SumsFile (Join-Path $Root 'SHA256SUMS_python.txt') 'Python (SHA256SUMS_python.txt)'
Test-SumsFile (Join-Path $Root 'c\SHA256SUMS') 'C code (c/SHA256SUMS)'

$gitOk = $false
if (Get-Command git -ErrorAction SilentlyContinue) {
    & git rev-parse --is-inside-work-tree 1>$null 2>$null
    if ($LASTEXITCODE -eq 0) {
        & git cat-file -e "${Pin}^{commit}" 1>$null 2>$null
        if ($LASTEXITCODE -eq 0) { $gitOk = $true }
    }
}

if ($gitOk) {
    Write-Host "--- Per-file digests @ $Pin ---"
    $items = @(
        @('photonic_sort.py', '2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c'),
        @('pyproject.toml', '2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d'),
        @('tests/test_photonic_sort.py', '03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798'),
        @('c/photonic_sort.c', 'e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54'),
        @('c/photonic_sort.h', '16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622')
    )
    foreach ($it in $items) {
        $path = $it[0]; $want = $it[1]
        try {
            $bytes = Get-GitBlobBytes "${Pin}:${path}"
            $got = Get-Sha256Bytes $bytes
            if ($got -eq $want) {
                Write-Host "OK  $got  $path"
            } else {
                Write-Host "BAD $path"
                Write-Host "  got:  $got"
                Write-Host "  want: $want"
                $fail++
            }
        } catch {
            Write-Host "ERR $path : $_"
            $fail++
        }
    }
    Write-Host ''
} else {
    Write-Host "(skip git pin checks — git or pin object not available)"
    Write-Host ''
}

Write-Host '========================================================================'
if ($fail -eq 0) {
    Write-Host 'All SHA-256 verification commands PASSED.'
    exit 0
} else {
    Write-Host "One or more SHA-256 checks FAILED ($fail)."
    exit 1
}
