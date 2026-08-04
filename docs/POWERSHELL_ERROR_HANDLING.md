# PowerShell error handling — PhotonicSort verify scripts

This note documents how [`scripts/verify-sha256.ps1`](../scripts/verify-sha256.ps1) handles errors, and the PowerShell rules that motivated the design.

## 1. Two different failure systems

| Kind | Examples | Throws? | How to detect |
|------|----------|---------|----------------|
| **Cmdlet** (PowerShell) | `Get-FileHash`, `Get-Content`, `Resolve-Path` | Depends on `$ErrorActionPreference` / `-ErrorAction` | `try/catch` when terminating |
| **Native executable** | `git`, `certutil` | **No** (usually) | `$LASTEXITCODE` after the call |

`$ErrorActionPreference = 'Stop'` only affects **cmdlets** and PowerShell advanced functions. It does **not** turn `git` exit code 1 into a catchable exception.

```powershell
$ErrorActionPreference = 'Stop'
git cat-file -e "nope^{commit}"   # does NOT enter catch by default
if ($LASTEXITCODE -ne 0) { throw "git failed: $LASTEXITCODE" }
```

## 2. Preferences used by the verifier

```powershell
Set-StrictMode -Version Latest   # uninitialized vars / bad property access → terminating
$ErrorActionPreference = 'Stop'  # cmdlet errors → terminating (catchable)
$ProgressPreference = 'SilentlyContinue'
```

| Preference | Effect |
|------------|--------|
| `Stop` | Non-terminating cmdlet errors become script-terminating |
| `Continue` (default) | Cmdlet errors write to `$Error` and continue |
| `SilentlyContinue` | Suppress non-terminating errors |
| `Inquire` | Prompt (interactive only; avoid in CI) |

Per-call override:

```powershell
Get-Content -LiteralPath $p -ErrorAction Stop
Get-Command git -ErrorAction SilentlyContinue
```

## 3. Patterns in `verify-sha256.ps1`

### 3.1 Aggregate vs fail-fast

Digest mismatches **increment** `$script:FailCount` and continue so the user sees every bad file.  
Unexpected infrastructure errors (missing repo root, crypto failure) **throw** and map to exit code **2**.

| Exit code | Meaning |
|----------:|---------|
| `0` | All checks passed |
| `1` | One or more digest / missing-file failures |
| `2` | Unhandled terminating error |

### 3.2 Script-scoped counters

```powershell
$script:FailCount = 0
function Add-Failure { $script:FailCount++; ... }
```

Inside functions, `$fail++` would create a **local** variable under `Set-StrictMode` / scoping rules and would **not** update the caller. Always use `$script:` for cross-function tallies.

### 3.3 `try / catch / finally` for IDisposable

```powershell
$sha = $null
try {
    $sha = [Security.Cryptography.SHA256]::Create()
    return $sha.ComputeHash($Bytes)
} finally {
    if ($null -ne $sha) { $sha.Dispose() }
}
```

`finally` runs even when `Stop` aborts the `try` block.

### 3.4 Native git wrapper

```powershell
function Invoke-Git {
    $ErrorActionPreference = 'Continue'  # local soft preference for native stderr
    $output = & git @Args 2>&1
    $code = $LASTEXITCODE
    if (-not $AllowFail -and $code -ne 0) { throw "git failed ($code)" }
}
```

- Capture `$LASTEXITCODE` **immediately** (next native call overwrites it).
- `2>&1` merges stderr so messages are available when throwing.
- `-AllowFail` for probes (`cat-file -e`) where non-zero is a soft “not present”.

### 3.5 Binary-safe blob reads

Never:

```powershell
git show HEAD:file.bin | Get-FileHash   # pipes as strings → encoding corruption
```

Do:

```powershell
# Process.StandardOutput.BaseStream → MemoryStream → SHA256.ComputeHash
```

(as in `Get-GitBlobBytes`).

### 3.6 Path safety

Use `-LiteralPath` so paths with `[ ]` wildcards are not globbed:

```powershell
Test-Path -LiteralPath $full
Get-Content -LiteralPath $SumsPath
Get-FileHash -LiteralPath $Path
```

### 3.7 Top-level guard

```powershell
try {
    # main verification
} catch {
    Write-Host "Unhandled: $($_.Exception.Message)"
    Write-Host $_.ScriptStackTrace
    exit 2
}
exit $exitCode
```

Ensures StrictMode / resolve failures still return a defined code to CI.

## 4. Common pitfalls (explored)

| Pitfall | Symptom | Fix |
|---------|---------|-----|
| Empty `catch { }` | Failures swallowed | Log + rethrow or `Add-Failure` |
| Local `$fail++` in function | Always “0 failures” | `$script:FailCount` |
| Trusting `$ErrorActionPreference` for `git` | False green | `$LASTEXITCODE` |
| `git show` through PowerShell pipeline | Hash mismatch on binary | Byte stream / `cat-file` + `BaseStream` |
| Forgetting `-LiteralPath` | Odd “not found” on valid paths | Always `-LiteralPath` for user paths |
| `$Matches` reused after nested `-match` | Wrong capture groups | Copy `$want` / `$rel` immediately after match |

## 5. Manual cmdlets for ad-hoc checks

```powershell
# Terminating error if missing
Get-FileHash -Algorithm SHA256 -LiteralPath .\photonic_sort.py -ErrorAction Stop

# Soft probe
if (Get-Command git -ErrorAction SilentlyContinue) { 'git ok' }

# Inspect last errors
$Error[0] | Format-List * -Force
```

## 6. Mapping to the bash verifier

| bash | PowerShell |
|------|------------|
| `set -e` | `$ErrorActionPreference = 'Stop'` **plus** `$LASTEXITCODE` checks for natives |
| `set -u` | `Set-StrictMode -Version Latest` |
| `fail=$((fail+1))` | `$script:FailCount++` |
| `trap` / `|| true` | `try/catch` / `-AllowFail` / `-ErrorAction SilentlyContinue` |
| `exit 1` | `exit 1` (or `2` for hard errors) |

## 7. Related files

- [`scripts/verify-sha256.ps1`](../scripts/verify-sha256.ps1)
- [`scripts/verify-sha256.sh`](../scripts/verify-sha256.sh)
- [`SHA256_VERIFY_COMMANDS.md`](../SHA256_VERIFY_COMMANDS.md)
