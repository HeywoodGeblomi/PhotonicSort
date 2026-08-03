# PhotonicSort — Verification instructions

**Give everything. Take nothing. Become photonic.**

Public integrity checks for **Python** and **C** trees.  
Implementation pin (C port + Python companion at tag time):  
`841882d906dc3fbf3bbc5c8dfa97f2773c0c1818` · tag `v1.0.1-c`

| Machine-readable digests | Scope |
|--------------------------|--------|
| [`SHA256SUMS_python.txt`](./SHA256SUMS_python.txt) | Python sources (live-check on `main`) |
| [`c/SHA256SUMS`](./c/SHA256SUMS) | C code artifacts (live-check on `main`) |
| [`SHA256SUMS_v1.0.1-c.txt`](./SHA256SUMS_v1.0.1-c.txt) | Full repo tree @ `841882d` (14 files) |
| [`SHA256_VERIFY_COMMANDS.md`](./SHA256_VERIFY_COMMANDS.md) | Command-only sheet |
| [`scripts/verify-sha256.sh`](./scripts/verify-sha256.sh) | One-shot verifier |

---

## SHA-256 verification commands (copy-paste)

Run from the **repository root** after clone.

### One-shot script

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort
chmod +x scripts/verify-sha256.sh
./scripts/verify-sha256.sh
```

### Manual — live working tree

```bash
# Python sources
sha256sum -c SHA256SUMS_python.txt

# C implementation artifacts
sha256sum -c c/SHA256SUMS
```

Expected output (abbreviated):

```
photonic_sort.py: OK
pyproject.toml: OK
tests/test_photonic_sort.py: OK
c/photonic_sort.c: OK
c/photonic_sort.h: OK
c/Makefile: OK
c/examples/demo.c: OK
c/tests/test_photonic_sort.c: OK
```

### Manual — per-file via git pin `841882d`

```bash
PIN=841882d906dc3fbf3bbc5c8dfa97f2773c0c1818

# Python
git show $PIN:photonic_sort.py | sha256sum
# → 2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c

git show $PIN:pyproject.toml | sha256sum
# → 2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d

git show $PIN:tests/test_photonic_sort.py | sha256sum
# → 03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798

# C
git show $PIN:c/photonic_sort.c | sha256sum
# → e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54

git show $PIN:c/photonic_sort.h | sha256sum
# → 16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622
```

### Manual — full repository tree @ pin

```bash
PIN=841882d906dc3fbf3bbc5c8dfa97f2773c0c1818
tmp=$(mktemp -d)
git archive --format=tar "$PIN" | tar -x -C "$tmp"
grep -E '^[0-9a-f]{64}  ' SHA256SUMS_v1.0.1-c.txt > "$tmp/SUMS"
(cd "$tmp" && sha256sum -c SUMS)
rm -rf "$tmp"
```

### Manual — OpenSSL (if `sha256sum` is unavailable)

```bash
openssl dgst -sha256 photonic_sort.py
openssl dgst -sha256 c/photonic_sort.c
openssl dgst -sha256 c/photonic_sort.h
```

### Manual — Python `hashlib` (stdlib)

```bash
python3 - <<'PY'
import hashlib, pathlib
files = [
    ("photonic_sort.py", "2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c"),
    ("pyproject.toml", "2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d"),
    ("tests/test_photonic_sort.py", "03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798"),
    ("c/photonic_sort.c", "e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54"),
    ("c/photonic_sort.h", "16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622"),
]
for path, want in files:
    got = hashlib.sha256(pathlib.Path(path).read_bytes()).hexdigest()
    status = "OK" if got == want else "BAD"
    print(f"{status}  {got}  {path}")
    if got != want:
        raise SystemExit(1)
print("All digests match.")
PY
```

### Windows PowerShell equivalents

```powershell
# One-shot
pwsh -File .\scripts\verify-sha256.ps1

# Live files
(Get-FileHash -Algorithm SHA256 .\photonic_sort.py).Hash.ToLower()
(Get-FileHash -Algorithm SHA256 .\c\photonic_sort.c).Hash.ToLower()

# SUMS files (see SHA256_VERIFY_COMMANDS.md for Test-Sha256SumsFile helper)
# or run: pwsh -File .\scripts\verify-sha256.ps1

# certutil
certutil -hashfile photonic_sort.py SHA256
certutil -hashfile c\photonic_sort.c SHA256
```

Full PowerShell command sheet: [`SHA256_VERIFY_COMMANDS.md`](./SHA256_VERIFY_COMMANDS.md) · script: [`scripts/verify-sha256.ps1`](./scripts/verify-sha256.ps1)

---


## 1. Python verification

### 1.1 Files under test

| Path | Role | Bytes @ pin |
|------|------|------------:|
| `photonic_sort.py` | Library + demo | 19688 |
| `pyproject.toml` | Packaging metadata (v1.0.1) | 1271 |
| `tests/test_photonic_sort.py` | Unit tests (15 cases) | 3833 |

### 1.2 SHA-256 digests (full)

```
2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c  photonic_sort.py
2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d  pyproject.toml
03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798  tests/test_photonic_sort.py
```

These match both `main` (post-docs) and `841882d` (Python sources unchanged by C docs commits).

### 1.3 Checksum verification

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort

# Live check (GNU coreutils)
sha256sum -c SHA256SUMS_python.txt

# Per-file from implementation pin
git show 841882d:photonic_sort.py | sha256sum
# expect: 2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c

git show 841882d:pyproject.toml | sha256sum
# expect: 2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d

git show 841882d:tests/test_photonic_sort.py | sha256sum
# expect: 03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798

# Optional: Python hashlib cross-check
python3 - <<'PY'
import hashlib, pathlib
expected = {
    "photonic_sort.py": "2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c",
    "pyproject.toml": "2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d",
    "tests/test_photonic_sort.py": "03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798",
}
for path, want in expected.items():
    got = hashlib.sha256(pathlib.Path(path).read_bytes()).hexdigest()
    assert got == want, (path, got, want)
    print(f"OK  {got}  {path}")
print("All Python digests match.")
PY
```

### 1.4 Functional verification (Python)

Requires **Python 3.10+** (stdlib only; no `pip install` required to run demo/tests).

```bash
# Unit tests (expect: Ran 15 tests … OK)
python3 -m unittest discover -s tests -v

# Marketing demo + self-check (expect: All correctness checks passed. / Become photonic.)
python3 photonic_sort.py

# Import API smoke
python3 - <<'PY'
from photonic_sort import photonic_sort, photonic_probe, __version__
assert __version__ == "1.0.1"
data = [7, 2, 9, 1, 5, 3, 8, 4, 6, 0]
assert photonic_sort(data) == list(range(10))
p = photonic_probe(list(range(100)))
assert p["is_negative_delay"] is True
assert p["monotone_sign"] == 1
print("API smoke OK", __version__)
PY
```

### 1.5 Expected Python outcomes

| Check | Pass criterion |
|-------|----------------|
| `sha256sum -c SHA256SUMS_python.txt` | all three lines `OK` |
| `unittest discover` | `Ran 15 tests` · `OK` · exit 0 |
| `python3 photonic_sort.py` | table paths correct · `All correctness checks passed.` · exit 0 |
| Sorted n=50000 path | near-instant structure exit (host-dependent ms) |

---

## 2. C verification (summary)

```bash
sha256sum -c c/SHA256SUMS
cd c && make clean all test && ./demo
```

Primary C digests:

```
e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54  c/photonic_sort.c
16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622  c/photonic_sort.h
```

Full C index: [`c/RELEASE_NOTES_v1.0.1-c.md`](./c/RELEASE_NOTES_v1.0.1-c.md)

---

## 3. Full tree @ `841882d`

```bash
git archive --format=tar 841882d | tar -x -C /tmp/ps-v
cd /tmp/ps-v
# copy SHA256SUMS_v1.0.1-c.txt from main into this dir, then:
sha256sum -c SHA256SUMS_v1.0.1-c.txt
```

---

## 4. Honesty

Verification confirms **file integrity and classical correctness** only.  
PhotonicSort does **not** solve NP-complete problems and does **not** prove P = NP.

Team: Grok · Harper · Benjamin · Lucas · Heywood Geblomi · MIT
