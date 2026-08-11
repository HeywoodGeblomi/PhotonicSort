# Field-Level Attack Plan — PhotonicSort

**Status:** LOCKED 2026-08-11  
**Squad:** THE BEASTIE BOYZ  
**Baseline:** Residual-Improvement pure residual v2.4.1 (`docs/residual_improve/BASELINE_LOCK.md`)

---

## 1. Goal

Convert PhotonicSort from a refined classical adaptive hybrid into a **broadly superior practical algorithm** that can displace established sequential sorters (pdqsort, ska_sort, Timsort, std::sort variants, and peers) across real-world distributions and architectures,

**or**

produce a fundamental shift in how adaptive sorting is understood and implemented (asymptotic / adaptive-complexity result).

Success requires hitting **at least one** of:

| Path | Definition of success |
|------|------------------------|
| **(a) Empirical domination** | Consistent, statistically significant outperformance of the relevant SOTA set across a broad, reproducible, independently-auditable suite of real-world + adversarial distributions, multiple architectures, and a wide *n*-range. Multi-type/generic support required for claims against pdq / Timsort / std::sort. |
| **(b) Theoretical advance** | Novel adaptive complexity result with practical consequences for residual selection or measures of presortedness. |
| **(c) Adoptable technique** | A clean, general technique (especially the visible-metric decision / residual-selection layer) that becomes a reference implementation and is adopted into major libraries or engines. |

Current v2.4.1 is **not** field-level on any of these paths. Honesty primary.

---

## 2. Current position (honest)

- **Strengths:** Strong classical adaptive pure residual menu on structured / low-card / run-heavy patterns. Targeted floors closed (k=2, reverse_segments). EXTERNAL-clean. Worst-case O(*n* log *n*). Engineering baseline locked.
- **Limits:** Residual quality / path limits on continuous near-uniform (Gaussian HE) and mid-card few-unique. Pure residual geo vs pdq still ~0.55–0.67× on mixed suites. Primarily i64-specialized. Synthetic suite only. No multi-arch / multi-compiler independent corpus. No asymptotic novelty.
- **Implication:** Excellent specialized integer adaptive hybrid. Not yet a candidate to displace general-purpose library sorts.

---

## 3. Success metrics

**Path (a):**
- Primary: geometric mean wall-time ratio vs best specialized sequential baseline on the expanded suite ≤ 0.85–0.90× (or clear statistical superiority with confidence intervals).
- Secondary: no catastrophic pattern (all patterns ≤ ~1.15–1.20× of best baseline).
- Coverage: real-world distributions + adversarial synthetics; *n* from ~10³ to ≥10⁸ where feasible; ≥3 architectures (x86-64 Skylake+/Zen3+, Apple Silicon, server ARM64).
- Statistical standard: median-of-odd or paired tests; multi-machine; published raw CSVs.

**Path (c):**
- Technique extracted, documented, and independently re-implemented by at least one external party **or** merged / used as reference in a major library or high-visibility engine.
- API and routing guarantees clean enough to copy.

**Path (b):**
- Published theorem + matching practical algorithm that improves a recognized adaptive bound or introduces a new, useful measure of disorder that the probe already approximates.

---

## 4. Kill criteria (falsifiable)

| Scope | Kill condition |
|-------|----------------|
| **Global** | After expanded suite + residual excellence + genericity work, pure residual still loses geo-mean to pdq/ska on the HE-heavy real-world slice by >15–20% **and** the generic path is not competitive with pdq → freeze as “excellent specialized adaptive for structured integer data” and retire the displacement claim. |
| **Phase 0** | Cannot produce a reproducible multi-baseline harness that others can run in one command / Docker → stop claiming independent verification progress. |
| **Phase 1** | Pure residual remains >1.15× ska/pdq on continuous HE patterns across ≥2 architectures after focused probe-tax + residual work → treat HE as permanent documented limit; do not continue pure residual micro-attacks. |
| **Phase 2** | Generic / comparator path cannot reach competitive performance with pdq on non-POD types without destroying the i64 pure residual fast-path → keep specialized only; displacement of std::sort/Timsort abandoned. |
| **Theory** | No concrete new measure or lower-bound statement emerges from existing probe metrics within a fixed window → park theory permanently as parallel. |

---

## 5. Hard constraints

1. **EXTERNAL-clean default** — residual selection uses visible metrics only. Any internal irreversible state requires an explicit kill-experiment-style justification and is currently out of scope.
2. **Protect pure residual excellence** — i64 (and later POD) pure residual menu remains a first-class fast-path; genericity must not dilute it.
3. **Multi-type / generic is mandatory** for any claim of displacing pdqsort / Timsort / std::sort.
4. **Memory** — prefer O(1) extra; any O(*n*) buffers must be documented and optional.
5. **Honesty gates** — every public artifact states “not field-level until [criteria]”. No marketing language that outruns measured surface.
6. **Worst-case** — retain O(*n* log *n*).

---

## 6. Phased roadmap

**Phase 0 — Definition + Evidence Foundation** (first real work)  
Lock this plan. Expand the suite (real distributions + adversarial). Multi-baseline harness (pdq, ska, std, Timsort-style/flux, IPS⁴o-seq if feasible, vqsort where available). Publish harness + raw results. Measure current v2.4.1 honestly → gap map.

**Phase 1 — Residual Excellence on the New Bar**  
Attack probe tax. Close or permanently document remaining residual quality limits measured against the expanded suite and SOTA. Goal: pure residual non-dominated (or clearly characterized) on integer patterns.

**Phase 2 — Multi-type / Generic Foundation**  
Template + comparator + optional key-extractor. Layered architecture: specialized pure residual fast-path for supported POD; classical adaptive hybrid (or better) for fully generic. Preserve i64 excellence.

**Phase 3 — Architecture + Scale**  
x86-64 (Intel + AMD), Apple Silicon, server ARM64. Optional accelerators never required. *n*-range and real corpus stress.

**Phase 4 — Independent Verification + Bake-off**  
External runners, multi-compiler, multi-machine. Public evidence package. Only then is a path-(a) claim admissible.

**Phase 5 — Claim, Adoption, or Theory**  
If 0–4 green → publish practical superiority and/or extract decision layer (path c). Theory (path b) only if a concrete statement has already appeared.

---

## 7. Immediate next moves

1. Lock this plan (done).
2. Design the expanded reproducible suite + multi-baseline harness specification (Phase 0 deliverable).
3. Sketch multi-type API surface while protecting pure residual fast-path.
4. Only then open residual excellence measured against the new bar.

Do **not** open another pure-residual micro-attack cycle first.

---

## 8. Non-claims discipline (continuous)

Until path (a), (b), or (c) is met under the metrics above:

- PhotonicSort remains a refined classical adaptive hybrid with a strong specialized pure residual menu.
- **Not a field-level breakthrough.**
- **Not a universal replacement** for library pdq / ska / Timsort / std::sort.
- **Not multi-architecture / multi-type** until those surfaces exist and are measured.
- **Not independently verified** until the published harness has been run by others.

---

**THE BEASTIE BOYZ**
