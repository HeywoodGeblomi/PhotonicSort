#!/usr/bin/env bash
# PhotonicSort — SHA-256 verification commands (run from repo root)
#   ./scripts/verify-sha256.sh
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

PIN="841882d906dc3fbf3bbc5c8dfa97f2773c0c1818"
fail=0

echo "========================================================================"
echo "PhotonicSort SHA-256 verification"
echo "Repo root: $ROOT"
echo "Implementation pin: $PIN"
echo "========================================================================"
echo

run_check() {
  local title="$1"
  shift
  echo "--- $title ---"
  if "$@"; then
    echo "PASS"
  else
    echo "FAIL"
    fail=1
  fi
  echo
}

if [[ -f SHA256SUMS_python.txt ]]; then
  run_check "Python (sha256sum -c SHA256SUMS_python.txt)" \
    sha256sum -c SHA256SUMS_python.txt
else
  echo "MISSING SHA256SUMS_python.txt"; fail=1; echo
fi

if [[ -f c/SHA256SUMS ]]; then
  run_check "C code (sha256sum -c c/SHA256SUMS)" \
    sha256sum -c c/SHA256SUMS
else
  echo "MISSING c/SHA256SUMS"; fail=1; echo
fi

if command -v git >/dev/null 2>&1 && git cat-file -e "${PIN}^{commit}" 2>/dev/null; then
  echo "--- Per-file digests @ $PIN ---"
  paths=(
    photonic_sort.py
    pyproject.toml
    tests/test_photonic_sort.py
    c/photonic_sort.c
    c/photonic_sort.h
  )
  wants=(
    2bbdd552e0782d07c5d5f5d72983331f21d41b376c6ad0f530352be97589e89c
    2e250763a95d26346a8eaa888aabb691556b08465f7c370de06b00d7af9c375d
    03197caa57e33f92e6aecae8a82dc4e5aa4e8286c637121652de494c49238798
    e4d4446e136b8b03c95dbb5a53ea4884db0351491a235e7c4e63eb4ad68ffb54
    16b795d5150d59e4d0bcf5a16182be041f351a3e8d0ecdc1fb667814b348e622
  )
  for i in "${!paths[@]}"; do
    path="${paths[$i]}"
    want="${wants[$i]}"
    got=$(git show "${PIN}:${path}" | sha256sum | awk '{print $1}')
    if [[ "$got" == "$want" ]]; then
      echo "OK  $got  $path"
    else
      echo "BAD $path"
      echo "  got:  $got"
      echo "  want: $want"
      fail=1
    fi
  done
  echo
else
  echo "(skip git pin checks — object $PIN not available in this clone)"
  echo
fi

if [[ -f SHA256SUMS_v1.0.1-c.txt ]] && command -v git >/dev/null 2>&1 \
   && git cat-file -e "${PIN}^{commit}" 2>/dev/null; then
  echo "--- Full tree @ $PIN (git archive + sha256sum -c) ---"
  tmp=$(mktemp -d)
  grep -E '^[0-9a-f]{64}  ' SHA256SUMS_v1.0.1-c.txt > "$tmp/SUMS"
  git archive --format=tar "$PIN" | tar -x -C "$tmp"
  if (cd "$tmp" && sha256sum -c SUMS); then
    echo "PASS full tree"
  else
    echo "FAIL full tree"
    fail=1
  fi
  rm -rf "$tmp"
  echo
fi

echo "========================================================================"
if [[ "$fail" -eq 0 ]]; then
  echo "All SHA-256 verification commands PASSED."
  exit 0
else
  echo "One or more SHA-256 checks FAILED."
  exit 1
fi
