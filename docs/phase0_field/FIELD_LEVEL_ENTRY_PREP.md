# Field-Level Entry Preparation

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Status:** PREP — not a field-level claim  
**Parent:** FIELD_LEVEL_ATTACK_PLAN.md · PATH_A_EVIDENCE_LOCK.md · SOFT_SPOTS_CAPTURE.md

---

## 1. Where we stand (honest)

| Gate | Status |
|------|--------|
| Multi-type surface (i32 + u32) | **MET** |
| Phase 0 18-pattern suite + n-matrix | **MET** |
| Independent reproduction harness | **MET** (`harness_phase0.cpp`) |
| Geo < 1.0× vs pdq both types @ n=1e6 | **MET** (0.46× / 0.52× suite) |
| Residual soft spots ≤1.15× (cold isolation) | **MET** (few_k16_wide closed) |
| Soft spots ≤1.15× under full-suite harness | **NEAR** (suite interference still inflates some) |
| Multi-architecture numbers | **NOT MET** |
| Statistical CI / ≥30 trials where feasible | **NOT MET** |
| i64 + generic/comparator path | **NOT MET** (i64 protected, unmeasured here) |
| Library integration (path c) | **NOT MET** |
| Novel complexity theorem (path b) | **NOT MET** |

**Verdict today: NOT field-level.** Closest path remains empirical (a).

---

## 2. Field-level entry checklist (path a)

### Must complete before any field-level language

- [ ] **Re-measure Phase 0 suite** with few_wide v2.6.1 cold + suite modes; publish updated gap maps
- [ ] **Soft-spot table ≤1.15×** under both isolation and full-suite methodology (or document suite-interference protocol)
- [ ] **n-matrix complete:** 1e5, 1e6, and at least one larger (1e7) on i32 + u32
- [ ] **Multi-arch:** ≥2 ISAs or ≥2 microarchitectures (e.g. Zen3 + Intel, or x86 + ARM)
- [ ] **Statistical rigor:** median-of-odd ≥9 for soft patterns; geo-mean with bootstrap or pairwise vs pdq/ska
- [ ] **Independent reproduction:** one-command Docker GREEN with published reference ratios
- [ ] **Honesty banner:** every public artifact states scope (sequential POD i32/u32) and non-claims

### Strongly recommended before claim

- [ ] i64 pure residual gap map on same suite (protect excellence, measure honestly)
- [ ] Comparison vs Timsort-style / IPS⁴o-seq where available
- [ ] Memory / O(1) extra documented
- [ ] Adversarial stress beyond Phase 0 (pipe_sparse variants, equal-heavy outliers)

### Kill criteria (still active)

If after the above, pure residual still loses geo-mean to pdq/ska on HE-heavy real-world slice by >15–20% **and** generic path is not competitive → freeze as specialized adaptive for structured integer data; retire displacement claim.

---

## 3. Immediate next tickets

| # | Ticket | Owner hint |
|---|--------|------------|
| 1 | Re-run Phase 0 gap maps with few_wide v2.6.1 | measurement |
| 2 | Docker one-command + reference ratios publish | reproduction |
| 3 | Multi-arch plan + first second-arch numbers | measurement |
| 4 | Soft-spot methodology note (isolation vs suite) | docs |
| 5 | i64 Phase 0 gap map (protected path, honest measure) | residual |

---

## 4. Non-claims (locked)

- This document is **preparation**, not a field-level claim.
- Soft-spot closure and suite geo do not constitute consistent statistically significant outperformance of SOTA across a broad reproducible multi-arch suite.
- EXTERNAL-clean preference held.
- No χ / no internal irreversible state in residual path.

**THE BEASTIE BOYZ**
