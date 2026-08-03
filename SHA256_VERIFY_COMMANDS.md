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
