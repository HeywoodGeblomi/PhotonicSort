# Field-Level Entry Attack Plan — Path (a) Primary

**Date:** 2026-08-12  
**Squad:** THE BEASTIE BOYZ  
**Status:** LOCKED for execution  
**Parent:** [`FIELD_LEVEL_ATTACK_PLAN.md`](../FIELD_LEVEL_ATTACK_PLAN.md)  
**Honesty:** Not field-level until path (a)/(b)/(c) success metrics are met.

---

## 0. Verdict on path (b) Theory

### Are there viable pathways?

**Yes, but narrow — not the primary bet for field-level entry in a fixed window.**

Adaptive sorting theory is mature:

| Classical result | Status |
|------------------|--------|
| Inv-optimal: Θ(n(1+log(1+Inv/n))) | Solved (Mannila, Elmasry, …) |
| Runs / run-entropy optimal merges | Solved (powersort, peeksort, PersiSort 2024) |
| Multiset / multiplicity entropy | Solved |
| Newer measures (α-sorted subsequences, LRM, …) | Active but occupied |

Our contribution is a **practical residual menu + visible-metric routing**, not a new merge policy or a new combinatorial measure of presortedness.

### Viable theory targets (if pursued in parallel)

| ID | Target | Why it might publish | Why it may fail as “field-level (b)” |
|----|--------|----------------------|-------------------------------------|
| **T1** | Prove each residual path is optimally adaptive w.r.t. a **probe-accessible** measure (sampled Inv, card, direction-change runs) | Matches what the code actually does; closes “we have no complexity result” | Likely incremental restatement of known Inv/Runs bounds for insertion / counting / pdq residual |
| **T2** | Decision theorem: when is O(sample) residual routing optimal among a fixed menu | Novel framing of multi-residual hybrids | Hard to state non-vacuously; may not move practice |
| **T3** | Cache-aware / RAM-model bounds for residual paths (not comparison model) | Practical theory aligned with measured wins | Model-sensitive; referees may call it engineering analysis |

**Recommendation:** Keep T1–T3 as a **parallel, non-blocking** track. Do **not** gate field-level entry on (b). Path (a) is the viable primary route given current assets (closed soft spots, multi-arch, reproduction, i32/u32/i64 surface).

Path (c) (adoption) follows only after (a) evidence is strong enough that a library maintainer would take the residual-selection technique seriously.

---

## 1. Path (a) — definition of done (no soft language)

Field-level entry via (a) requires **all** of the following simultaneously:

1. **Primary metric:** Geometric mean wall-time ratio vs **best specialized sequential baseline** (min of pdqsort, ska_sort where applicable, and std::sort) on the **Expanded Field Suite** ≤ **0.90×**, with bootstrap or t-interval excluding 1.0 at 95% confidence (or equivalent clear statistical superiority).
2. **No catastrophe:** Every controlled pattern ≤ **1.15×** vs best specialized under cold isolation (same rule as soft-spot kill).
3. **Breadth:** Expanded Field Suite includes real-world-shaped families + adversarial + HE, at least **n ∈ {1e5, 1e6, 1e7}** where RAM allows.
4. **Multi-arch:** ≥2 ISAs already MET (x86_64 + aarch64); must **hold** on Expanded Field Suite, not only PO2 subset.
5. **Multi-type:** Competitive on **i32, u32, i64** (done) **and** at least one of: float64 **or** generic comparator surface with the same residual philosophy (visible metrics only).
6. **Independent reproduction:** Third party can re-run from public Docker/CI (PO6 GREEN — hold).
7. **Honesty package:** Published NON_CLAIMS, scope (sequential POD / stated types only), and residual limits where any remain.

Until 1–7 hold, language stays **not field-level**.

---

## 2. Current position (post PO2–PO6 + i64 soft-spot close)

| Asset | Status |
|-------|--------|
| Soft spots i32/u32/i64 on Phase 0 set | CLOSED |
| Multi-arch | MET |
| Reproduction | GREEN |
| i64 geo vs pdq (19-pattern) | ~0.76× (vs **pdq only**, not vs best specialized) |
| i32 geo vs pdq (12-pattern) | ~0.38× |
| Float / generic | **Missing** |
| ska / Timsort / multi-baseline geo | **Incomplete for field claim** |
| n=1e7 + stats package | **Missing** |
| Real fixed datasets (checksummed) | **Sparse** |

**Gap to (a):** Primary metric is defined vs **best specialized**, not vs pdq alone. Suite must expand. Float/generic and statistical package are mandatory for a legitimate entry claim.

---

## 3. Attack phases (path a only)

### Phase A0 — Claim surface freeze (1 sprint)

**Deliverables:**
- `docs/field_level/EXPANDED_FIELD_SUITE.md` — exact pattern list, generators/seeds, checksums for any fixed datasets
- Baselines locked: orlp pdqsort, ska_sort (integer), libstdc++ `std::sort`; optional Timsort only on generic/float if in scope
- Metric script: geo mean, per-pattern ratios, bootstrap CI (n_boot ≥ 1000 on median-of-7 wall times)
- Kill criterion written: if after A3 primary geo > 0.95× vs best specialized with CI overlapping 1.0 → freeze specialized claim, do not use field-level language

**Exit:** Suite + metrics frozen on main; no residual code changes in A0.

### Phase A1 — Multi-baseline gap map (1 sprint)

**Deliverables:**
- Harness reports ratio vs **each** baseline and vs **min(baselines)**
- Full gap map at n=1e6 (primary) and n=1e5 (sanity) for i32/u32/i64
- Identify patterns where we lose to ska or std (not just pdq)

**Exit:** Published gap map; soft spots redefined vs best specialized (≤1.15× rule).

### Phase A2 — Close or scope residual losses (1–2 sprints)

For every pattern >1.15× vs best specialized:
- Either close with EXTERNAL-clean residual (preferred), **or**
- Document as **out of claim scope** with explicit rationale (e.g. continuous HE vs radix-specialized ska)

Do not hide losses inside a geo mean.

**Exit:** Zero in-scope pattern >1.15× vs best specialized on n=1e6.

### Phase A3 — Scale + multi-type (1–2 sprints)

- n=1e7 where RAM allows (at least x86_64 CI)
- float64 surface **or** generic comparator path (visible-metric residual selection, no type-punning cheats)
- Re-run multi-arch CI on Expanded Field Suite

**Exit:** Primary geo ≤0.90× vs best specialized with CI; multi-type clause satisfied.

### Phase A4 — External verification + claim package (1 sprint)

- Third-party runnable Docker/CI (already PO6) pointed at Expanded Field Suite
- `docs/field_level/FIELD_LEVEL_CLAIM_v0.1.md` with mandatory scope + NON_CLAIMS
- Only then drop “not field-level” language — and only for the exact scope claimed

**Kill:** Any reopened soft spot, CI red on second arch, or geo regression → claim withdrawn.

---

## 4. Non-goals during this attack

- Parallel (b) theory deep-dives that block A0–A4
- Hybrid library-pdq residual for claim numbers (Vector-2 hybrid remains a separate configuration if needed; field-level pure residual claim stays EXTERNAL-clean)
- String/struct sorts, parallel sorts, GPU
- Weakening the ≤1.15× catastrophe rule

---

## 5. Resource order (what to do next)

1. **A0 now** — freeze Expanded Field Suite + best-specialized metric  
2. **A1** — multi-baseline gap map (expects some “wins vs pdq” to look weaker vs ska)  
3. **A2** — residual or scope  
4. **A3** — n=1e7 + float/generic  
5. **A4** — claim package only if metrics hold  

Path (b) T1 may start in parallel after A1 if someone has bandwidth; it does not unlock field-level language alone.

---

## 6. One-line position

**Path (b) is viable as formalization, unlikely as a short-path field-level breakthrough. Path (a) is the real entry route — and it starts by measuring against best specialized on a frozen expanded suite, not by celebrating pdq-only soft-spot kills.**

**THE BEASTIE BOYZ**
