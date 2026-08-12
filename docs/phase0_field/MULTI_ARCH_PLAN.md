# PO3 — Multi-Arch Plan

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Status:** PLAN LOCKED · Arch-1 measured · **Arch-2 via CI work-around (`ubuntu-24.04-arm`)**  
**Not field-level** until both CI jobs GREEN and artifacts published.

---

## 1. Goal

Measured ratios on **≥2 distinct ISAs**, same suite, same baselines, ratio methodology.

---

## 2. Architecture matrix

| Slot | ISA | Target | Status |
|------|-----|--------|--------|
| **Arch-1** | x86_64 | Xeon 8481C (local) + `ubuntu-24.04` CI | MEASURED / CI |
| **Arch-2** | ARM64 | GitHub `ubuntu-24.04-arm` (Cobalt 100 class) | **CI work-around** |
| Optional | macOS arm64 | `macos-14` | optional third |

---

## 3. Work-around: close gate without local ARM hardware

Inspired by the real-QM construction (APS DOI 10.1103/4k13-sdjh): complex numbers are not necessary if a correctly structured real model predicts the same outcomes under a locality postulate. Analogously, **local ARM hardware is not necessary** if a correctly structured public CI matrix produces native second-ISA measurements under the same residual protocol.

**Mechanism:** `.github/workflows/multi-arch.yml`

```text
matrix:
  - arch: x86_64  → runs-on: ubuntu-24.04
  - arch: arm64   → runs-on: ubuntu-24.04-arm   # free for public repos
```

Both jobs fingerprint, build `harness_po2`, run n=1e6 median-of-7, soft-spot check (≤1.15, ok=1), upload artifacts.

**Honesty constraints:**
- QEMU user-mode ratios are **invalid** for this gate.
- Cross-compile without native run is **invalid**.
- Only native runner numbers count.
- Gate MET when both CI jobs GREEN and artifacts linked here.

Trigger: push to `main` affecting residual/reproduce, or `workflow_dispatch`.

---

## 4. Arch-1 baseline (local lock)

| pattern | menu/pdq |
|---------|----------:|
| pipe_sparse i32/u32 | 0.81 / 0.83× |
| almost_sorted | 0.24× |
| timestamps | 0.51× |
| few_k16_wide | 0.88× |
| db_pk | 0.82× |
| random | 0.63× |
| gaussianish | 0.49× |
| organpipe | 0.08× |
| sawtooth | 0.16× |

---

## 5. Success criteria

| Criterion | MET when |
|-----------|----------|
| Plan + workflow on main | this file + workflow |
| Arch-1 GREEN | local + CI x86_64 |
| Arch-2 GREEN | CI arm64 artifact ratios ≤1.15 all patterns |
| Multi-arch gate | **both** arches GREEN |

**Current:** Plan + workflow landed. Gate closes when CI arm64 job first goes GREEN.

---

## 6. Non-claims

- Workflow existence alone is not gate MET.
- Emulation is not a second arch for performance ratios.
- Not field-level until multi-arch + remaining PO gates.

**THE BEASTIE BOYZ**
