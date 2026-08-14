# Claim B Measurement — Minimax Residual Wall-Clock on F1/F2

**Date:** 2026-08-14  
**Status:** **MEASURED** (narrow support)  
**Squad:** THE BEASTIE BOYZ / Blam  
**Harness:** `scripts/claim_b_measure.cpp`  
**EXTERNAL-clean. Honesty primary.**

---

## Protocol

For each separation-family phase pair \((X_0, X_1)\) with matched classical statistics and opposite \(\sigma_\Delta\) polarity:

\[
\mu_{\mathrm{classical}} = \max\bigl(T_{\mathrm{class}}(X_0),\, T_{\mathrm{class}}(X_1)\bigr)
\]
\[
\mu_{\mathrm{dual}} = \max\bigl(T_{\mathrm{dual}}(X_0),\, T_{\mathrm{dual}}(X_1)\bigr)
\]

- **Classical** = hybrid residual without `-DSECONDARY_PARITY`
- **Dual** = hybrid residual with `-DSECONDARY_PARITY` (odd-restricted contrast + dual-evidence on borderline HE)
- Times: best-of-5 wall-clock ms, `n` elements, `i64`, ok=1 required
- Claim B **supported** on a pair if \(\mu_{\mathrm{dual}} < 0.98 \cdot \mu_{\mathrm{classical}}\) (>2% minimax improvement)

---

## Results (x86_64, g++ -O3, reps=5)

| Family | n | μ_classical ms | μ_dual ms | dual/class | >2% win |
|--------|--:|---------------:|----------:|-----------:|:-------:|
| f1 | 4096 | 0.0271 | 0.0338 | 1.25 | no |
| f1 | 8192 | 0.0477 | 0.0590 | 1.24 | no |
| f1 | 32768 | 0.1808 | 0.2197 | 1.22 | no |
| f1 | 1e5 | 0.4345 | 0.5842 | 1.34 | no |
| **f1** | **1e6** | **6.436** | **6.150** | **0.956** | **yes** |
| f2_organpipe | 8192 | 0.0257 | 0.0267 | 1.04 | no |
| f2_organpipe | 32768 | 0.0765 | 0.0775 | 1.01 | no |
| f2_organpipe | 1e5 | 0.2050 | 0.2005 | 0.978 | yes |
| f2_organpipe | 1e6 | 26.52 | 26.97 | 1.017 | no |
| f2_rseg | 8192 | 0.2127 | 0.2280 | 1.07 | no |
| f2_rseg | 32768 | 0.4614 | 0.4543 | 0.985 | no |
| f2_rseg | 1e5 | 1.744 | 1.690 | 0.969 | yes |
| f2_rseg | 1e6 | 23.22 | 23.20 | 0.999 | no |

**Wins:** 3 / 13 pairs. **Correctness:** ok=1 all cells.

### Headline — F1 n=1e6 phase detail

| Gate | phase0 ms | phase1 ms | μ |
|------|----------:|----------:|--:|
| classical | 4.276 | 6.436 | 6.436 |
| dual | 6.137 | 6.150 | 6.150 |

Classical exhibits phase asymmetry on the F1 pair (same classical Inv/Runs construction, different residual cost). Dual equalizes the phases and **improves minimax by ~4.4%**. That is the Claim B shape: the worse phase under classical is pulled down when odd-restricted contrast is available as second solid.

### Small-n F1

Dual pays a measurable probe tax (~20–35%) at n ≤ 1e5. Expected: σ stream + dual_confirm work is not free; residual-path differences do not amortize until n is large enough for residual choice to dominate probe cost.

### F2

Mostly parity. Organpipe already wins big vs pdq under both gates (structured residual). Reverse-segments shows mild dual edge at n=1e5; near-tie at 1e6.

---

## Interpretation

| Statement | Verdict |
|-----------|---------|
| Claim B holds on *some* F1/F2 pairs as measured minimax gap | **YES** (narrow) |
| Dual never introduces correctness failure on these pairs | **YES** |
| Dual is uniformly faster on all F1/F2 | **NO** — small-n probe tax |
| Claim B is a suite-wide soft-gate claim | **NO** — separation-family measurement only |

**Claim B status:** **MEASURED** with narrow scope — constructive support on F1 at n=1e6 (and two milder F2 cells). Not promoted to a blanket residual-cost theorem.

---

## Reproduce

```bash
# from repo root after fetching baselines/pdqsort.h + residual/ska_sort.hpp
g++ -O3 -std=c++17 -DNDEBUG -I. -Iresidual -Ibaselines \
  -o claim_b_classical scripts/claim_b_measure.cpp
g++ -O3 -std=c++17 -DNDEBUG -DSECONDARY_PARITY -I. -Iresidual -Ibaselines \
  -o claim_b_dual scripts/claim_b_measure.cpp
./claim_b_classical 5
./claim_b_dual 5
```

---

## Non-claims

1. Not a claim that dual-evidence improves every charged-surface cell.
2. Not Claim A (non-reducibility) — that remains the constructive F1/F2 separation result.
3. Not asymptotic complexity improvement — wall-clock minimax on finite pairs only.
4. EXTERNAL-clean. No χ.

---

**THE BEASTIE BOYZ / Blam**
