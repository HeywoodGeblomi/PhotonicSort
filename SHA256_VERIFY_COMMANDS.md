# SHA-256 verification commands

Pin: `841882d906dc3fbf3bbc5c8dfa97f2773c0c1818` · tag `v1.0.1-c`  
Full narrative: [`VERIFY.md`](./VERIFY.md) · runner: [`scripts/verify-sha256.sh`](./scripts/verify-sha256.sh)

## Quick

```bash
./scripts/verify-sha256.sh
```

```bash
sha256sum -c SHA256SUMS_python.txt
sha256sum -c c/SHA256SUMS
```

## Python

```bash
sha256sum -c SHA256SUMS_python.txt

git show 841882d:photonic_sort.py | sha256sum
# 2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c

git show 841882d:pyproject.toml | sha256sum
# 2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d

git show 841882d:tests/test_photonic_sort.py | sha256sum
# 03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798
```

## C

```bash
sha256sum -c c/SHA256SUMS

git show 841882d:c/photonic_sort.c | sha256sum
# e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54

git show 841882d:c/photonic_sort.h | sha256sum
# 16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622
```

## Full tree @ pin

```bash
PIN=841882d906dc3fbf3bbc5c8dfa97f2773c0c1818
tmp=$(mktemp -d)
git archive --format=tar "$PIN" | tar -x -C "$tmp"
grep -E '^[0-9a-f]{64}  ' SHA256SUMS_v1.0.1-c.txt > "$tmp/SUMS"
(cd "$tmp" && sha256sum -c SUMS)
rm -rf "$tmp"
```

## OpenSSL

```bash
openssl dgst -sha256 photonic_sort.py
openssl dgst -sha256 c/photonic_sort.c
openssl dgst -sha256 c/photonic_sort.h
```

## Python hashlib

```bash
python3 -c "import hashlib,pathlib; print(hashlib.sha256(pathlib.Path('photonic_sort.py').read_bytes()).hexdigest())"
python3 -c "import hashlib,pathlib; print(hashlib.sha256(pathlib.Path('c/photonic_sort.c').read_bytes()).hexdigest())"
```

---

## Windows PowerShell equivalents

Requires **Windows PowerShell 5.1+** or **PowerShell 7+** (`pwsh`).  
`Get-FileHash` is built-in (no OpenSSL required).

### One-shot script

```powershell
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort
pwsh -File .\scripts\verify-sha256.ps1
# or:
powershell -ExecutionPolicy Bypass -File .\scripts\verify-sha256.ps1
```

### Live working tree (`Get-FileHash`)

```powershell
# Single file
(Get-FileHash -Algorithm SHA256 .\photonic_sort.py).Hash.ToLower()
# expect: 2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c

(Get-FileHash -Algorithm SHA256 .\c\photonic_sort.c).Hash.ToLower()
# expect: e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54

(Get-FileHash -Algorithm SHA256 .\c\photonic_sort.h).Hash.ToLower()
# expect: 16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622
```

### Check a SUMS file (Python or C)

```powershell
function Test-Sha256SumsFile {
  param([Parameter(Mandatory)][string]$SumsPath)
  Get-Content $SumsPath | ForEach-Object {
    $line = $_.Trim()
    if ($line -match '^\s*#' -or $line -eq '') { return }
    if ($line -notmatch '^([0-9a-fA-F]{64})\s+(.+)$') { return }
    $want = $Matches[1].ToLowerInvariant()
    $rel  = $Matches[2].Trim().Replace('/', '\')
    $got  = (Get-FileHash -Algorithm SHA256 -Path $rel).Hash.ToLowerInvariant()
    if ($got -eq $want) { "OK  $got  $($Matches[2].Trim())" }
    else { "BAD $($Matches[2].Trim())"; "  got:  $got"; "  want: $want"; exit 1 }
  }
}

Test-Sha256SumsFile .\SHA256SUMS_python.txt
Test-Sha256SumsFile .\c\SHA256SUMS
```

### Per-file via git pin `841882d` (binary-safe)

```powershell
$Pin = '841882d906dc3fbf3bbc5c8dfa97f2773c0c1818'

function Get-GitPathSha256([string]$RevPath) {
  $blob = git rev-parse $RevPath
  $psi = New-Object System.Diagnostics.ProcessStartInfo
  $psi.FileName = 'git'
  $psi.Arguments = "cat-file -p $blob"
  $psi.RedirectStandardOutput = $true
  $psi.UseShellExecute = $false
  $proc = [Diagnostics.Process]::Start($psi)
  $ms = New-Object System.IO.MemoryStream
  $proc.StandardOutput.BaseStream.CopyTo($ms)
  $proc.WaitForExit()
  $sha = [Security.Cryptography.SHA256]::Create()
  try {
    $hash = $sha.ComputeHash($ms.ToArray())
    return ([BitConverter]::ToString($hash) -replace '-', '').ToLowerInvariant()
  } finally { $sha.Dispose(); $ms.Dispose() }
}

Get-GitPathSha256 "$Pin`:photonic_sort.py"
# 2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c

Get-GitPathSha256 "$Pin`:c/photonic_sort.c"
# e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54

Get-GitPathSha256 "$Pin`:c/photonic_sort.h"
# 16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622
```

### CertUtil (Windows built-in alternative)

```powershell
certutil -hashfile photonic_sort.py SHA256
certutil -hashfile c\photonic_sort.c SHA256
certutil -hashfile c\photonic_sort.h SHA256
```

### Python functional checks on Windows

```powershell
python -m unittest discover -s tests -v
python photonic_sort.py
```

### C build on Windows (optional)

Use MSVC, clang, or MinGW; the portable Makefile targets GCC/clang on Unix.  
For MinGW/MSYS2:

```powershell
# in MSYS2/MinGW shell
cd c
make clean all test
./demo
```

| Unix / GNU | PowerShell / Windows |
|------------|----------------------|
| `sha256sum FILE` | `(Get-FileHash -Algorithm SHA256 FILE).Hash.ToLower()` |
| `sha256sum -c SUMS` | `Test-Sha256SumsFile .\SUMS` or `.\scripts\verify-sha256.ps1` |
| `git show REV:PATH \| sha256sum` | `Get-GitPathSha256 "REV:PATH"` (above) |
| `openssl dgst -sha256 FILE` | `Get-FileHash` or `certutil -hashfile FILE SHA256` |
| `./scripts/verify-sha256.sh` | `pwsh -File .\scripts\verify-sha256.ps1` |

---

## PowerShell error handling

Design notes for `$ErrorActionPreference`, `$LASTEXITCODE`, `$script:` scopes, and binary-safe git reads:

→ [`docs/POWERSHELL_ERROR_HANDLING.md`](./docs/POWERSHELL_ERROR_HANDLING.md)
