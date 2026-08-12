# PO3 — Multi-Arch Plan

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Status:** PLAN LOCKED · Arch-1 measured · **second-arch HARDWARE-GATED**  
**Parent:** FIELD_LEVEL_ATTACK_PLAN / Phase 0 suite design  
**Not field-level.**

---

## 1. Goal

Satisfy the multi-arch gate for path-(a) field-level entry:

> Measured ratios on **≥2 distinct ISAs or ≥2 distinct microarchitectures**, same suite, same baselines, ratio methodology.

Single-arch numbers alone do **not** meet the gate.

---

## 2. Architecture matrix

| Slot | ISA | Representative target | Phase 0 required? | Status |
|------|-----|----------------------|-------------------|--------|
| **Arch-1** | x86-64 | Intel Xeon Platinum 8481C (Sapphire Rapids class) | Yes | **MEASURED** |
| **Arch-2** | ARM64 | Apple M-series **or** Neoverse/Graviton | Yes | **HARDWARE-GATED** |
| Arch-3 (optional) | x86-64 alt µarch | AMD Zen3+ | Preferred | pending |
| Optional | SVE / AVX-512 specialized | only if residual uses them | Never required for gate | n/a |

Minimum for gate MET: Arch-1 + Arch-2 (different ISA preferred; different µarch acceptable if documented).

---

## 3. Measurement protocol (identical on every arch)

1. **Suite:** PO2 reference harness patterns (`reproduce/harness_po2.cpp`) at n=1e6, median of 7.
2. **Baselines:** orlp pdqsort (vendored or curl-fetched at build).
3. **Compiler:** g++ or clang, `-O3 -std=c++17 -DNDEBUG`. Record full version string.
4. **Metric:** menu/pdq **ratio** only. Absolute ms are host-dependent and not compared across arch.
5. **Correctness:** every row `ok=1` (sorted + equal to pdq result).
6. **Fingerprint:** publish `ARCH*_FINGERPRINT.txt` (uname, CPU model, compiler, flags).

Acceptance: pattern-for-pattern ratios within ~20% relative of Arch-1 reference, **and** no controlled pattern >1.15× under cold isolation.

Soft-spot reopen: any controlled pattern >1.15× on any required arch.

---

## 4. Arch-1 baseline (locked)

| Field | Value |
|-------|--------|
| Machine | `x86_64` Linux |
| CPU | Intel(R) Xeon(R) Platinum 8481C CPU @ 2.70GHz |
| Compiler | g++ 13.3.0 |
| Flags | `-O3 -std=c++17 -DNDEBUG` |
| Date | 2026-08-12 |

### Arch-1 ratios (n=1e6, median of 7)

| type | pattern | menu/pdq | ok |
|------|---------|----------:|----|
| i32 | pipe_sparse | **0.81×** | 1 |
| u32 | pipe_sparse | **0.83×** | 1 |
| i32 | almost_sorted | **0.24×** | 1 |
| i32 | timestamps | **0.51×** | 1 |
| i32 | few_k16_wide | **0.88×** | 1 |
| i32 | db_pk | **0.82×** | 1 |
| i32 | random | **0.63×** | 1 |
| i32 | gaussianish | **0.49×** | 1 |
| i32 | organpipe | **0.08×** | 1 |
| i32 | sawtooth | **0.16×** | 1 |

All residual soft spots CLOSED on Arch-1.

---

## 5. How to obtain Arch-2 numbers

### Path A — native ARM64 host (preferred)

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort
docker build -f reproduce/Dockerfile -t photonic-po2 .
docker run --rm photonic-po2 | tee ARCH2_RATIOS.txt
uname -a; g++ --version | head -1
```

### Path B — Docker buildx multi-platform (when builder available)

```bash
docker buildx create --use --name multi 2>/dev/null || true
docker buildx build --platform linux/arm64 -f reproduce/Dockerfile -t photonic-po2:arm64 --load .
docker run --rm photonic-po2:arm64
```

Requires buildx + qemu-user or a remote ARM builder. **Not available in current agent sandbox.**

### Path C — CI matrix

GitHub Actions `ubuntu-24.04` (x86_64) + `macos-14` (ARM64) or self-hosted Graviton runner. Same harness; publish both ratio tables under `docs/phase0_field/arch/`.

---

## 6. Success / fail criteria

| Criterion | MET when |
|-----------|----------|
| Plan published | this file on main |
| Arch-1 measured | ARCH1 ratios + fingerprint on main |
| Arch-2 measured | second-ISA or second-µarch ratios + fingerprint on main |
| Soft spots held | no controlled pattern >1.15× on required arches |
| Field-level multi-arch gate | Arch-1 + Arch-2 both GREEN |

**Current:** Plan + Arch-1 GREEN. Arch-2 **HARDWARE-GATED**. Multi-arch gate **NOT MET**.

---

## 7. Non-claims

- Arch-1 alone is not multi-arch.
- Docker one-command on a single ISA is not multi-arch.
- Cross-compile without running on the target is not a measurement.
- Not field-level.

**THE BEASTIE BOYZ**
