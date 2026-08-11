# Gaussian HE Phase — CLOSED

**Status:** CLOSED 2026-08-11  
**Decision:** Document the floor; do not change the residual.  
**Baseline:** Residual-Improvement v2.4.1 (`docs/residual_improve/BASELINE_LOCK.md`)

## Outcome

The Gaussian HE phase opened against the last major documented residual quality floor (~1.1–1.3× vs pdq on continuous near-uniform / gauss_nearflat).

**First experiment (E1):** pure comparison residual (introsort) as alternative HE path.

| Pattern (isolated residual, n=1e6, median-of-11) | classical MSD / pdq | pure introsort / pdq |
|------------------------------------------------|--------------------:|---------------------:|
| gauss_nearflat | 0.47–1.02× | 2.2–3.4× |
| gauss_wide | 0.61× | 1.99× |
| uniform_i64 | 0.53–0.73× | 2.9× |

**Negative result:** Classical fixed-8-bit MSD residual is already competitive or winning in isolation. The pure introsort residual is 2–3× slower and is not a viable replacement.

## Diagnosis

The full-menu floor (~1.05–1.25× on suite `gaussian` / `gauss_nearflat`) is dominated by probe / path overhead (or generator & measurement effects), **not** by residual quality of the MSD itself.

Prior pure MSD micro-experiments (adaptive digit width, 16-bit first digit, in-place American-Flag, ping-pong) were already exhausted and regressed. No further pure scatter lever remains for this family.

## Decision (Option B)

- Treat the remaining ~1.1–1.2× full-menu ratio as a **documented residual quality / path limit**.
- Same honesty class as `few_unique_k16`.
- Classical HE MSD residual stays unchanged.
- No residual code change ships from this phase.
- Option A (broader full-menu probe-tax reduction) is deferred as a separate menu-efficiency effort if desired later.

## Residual limits ledger (updated)

| Floor | Result | Status |
|-------|--------|--------|
| two_values / k=2 | 0.42–0.47× | CLOSED |
| reverse_segments_8 | 0.85–0.93× | CLOSED |
| few_unique_heavy_k4 | ~0.8–0.9× | held |
| few_unique_k16 | ~1.0–1.2× | residual quality limit |
| **Gaussian HE** | **~1.1–1.2× full-menu** | **residual quality / path limit (documented)** |

Isolated residual quality of classical MSD on continuous near-uniform is competitive with pdq. The full-menu ratio remains a path limit under the pure residual contract.

## Rules held

- EXTERNAL-clean
- Pure residual only (no library dispatch)
- Phase 0 remains historical reference
- Not field-level
- Honesty primary

**THE BEASTIE BOYZ**
