# Wave 2 Multi-Type Design — Barrier 4

**Status:** LOCKED 2026-08-12  
**Parent:** [`BARRIER_BREAK_PLAN.md`](./BARRIER_BREAK_PLAN.md) · [`FIELD_LEVEL_ATTACK_PLAN.md`](./FIELD_LEVEL_ATTACK_PLAN.md)  
**Squad:** THE BEASTIE BOYZ

---

## Goal

Open a multi-type surface so the pure residual path is no longer sequential-int64-only. Mandatory for any path-(a) claim against pdq / Timsort / std::sort.

## Success / Kill (from Barrier Break Plan)

| Criterion | Definition |
|-----------|------------|
| **Success** | Competitive vs pdq on ≥2 non-i64 POD types (int32, uint32 first) across the Phase 0-style suite |
| **Kill** | Generic / multi-type work destroys i64 pure residual fast-path **or** never competitive on the non-i64 types → remain specialized only |

## Hard constraints

1. **Protect i64 pure residual excellence** — zero regression on locked Wave 1 residual floors. `sort_i64` and residual_*_i64 remain first-class and untouched in behavior.
2. EXTERNAL-clean residual selection (visible metrics only).
3. Worst-case O(n log n).
4. Prefer O(1) extra memory; any O(n) buffers documented.
5. No field-level language until path (a)/(b)/(c) criteria met.

## Type order

| Phase | Types | Rationale |
|-------|-------|-----------|
| **2.0** | `int32_t`, `uint32_t` | Same bit-width class, signed/unsigned, highest ROI, reuses most residual logic with type swaps |
| **2.1** | `float` (IEEE-754 binary32) | Requires bit-cast / radix or comparator path; real-world relevance |
| **2.2** | Generic comparator + optional key extractor | Full genericity; classical adaptive hybrid fallback if pure residual not applicable |

## Architecture

```
pure_residual::
  sort_i64(int64_t*, size_t)     // existing locked fast-path — DO NOT REGRESS
  sort_i32(int32_t*, size_t)     // new
  sort_u32(uint32_t*, size_t)    // new
  // later: sort_f32, sort(T*, size_t, Comp)
```

- Explicit specializations preferred over heavy templates for residual quality control and compile-time clarity.
- Shared structure (STRUCTURE probe, reverse-runs skeleton, counting skeleton, few_wide skeleton) can be templated internally where safe.
- Type-specific residual modules: residual_few_wide_i32.hpp, residual_he_msd_i32.hpp, residual_low_disorder_i32.hpp, etc. (or a single residual_*_pod.hpp with explicit instantiations).

## Residual adaptation notes

| Residual | i32 / u32 notes |
|----------|-----------------|
| STRUCTURE / reverse | Identical logic; compare operators work |
| few_wide / counting | Range gates scaled to 32-bit; HCAP / KMAX same; mix hash for 32-bit keys |
| low_disorder | Identical inv-sample + insertion / introsort |
| HE MSD | 32-bit MSD radix (4–8 passes) instead of 64-bit; or introsort fallback |
| sparse / adversarial | Analogous; bit-width adjustments |
| identity-almost | Consecutive domain assumes integer; works for i32/u32 |

## Harness extension

- Keep Wave 0 `reproduce/harness.cpp` strictly int64 for independent reproduction of locked residual floors. Do not touch it.
- Add `reproduce/harness_multitype.cpp` (or `reproduce/multitype/`) with type-parameterized generators for int32 / uint32 first.
- Same 18-pattern family, scaled ranges for 32-bit (e.g. wide keys use ~1e8–1e9 scale).
- Baselines: std::sort, pdqsort, ska_sort (already support these types).
- Report per-type geo and soft spots. i64 numbers remain the Wave 1 reference; multi-type is additive.

## Measurement protocol

1. n=1e6 (and n=1e5 smoke), median-of-5, full verify.
2. Compare ratios vs pdq (primary) and std/ska.
3. Success target for Wave 2.0: geo ≤ ~1.0–1.15× pdq on i32 and on u32 independently, with no catastrophic pattern (>1.5×) on the primary suite, and **zero regression** on i64 residual floors.

## Non-claims (continuous)

- Not field-level.
- Not a universal replacement for library pdq / ska / Timsort / std::sort.
- Multi-type surface is under construction; until measured, claims remain sequential-int64 specialized.
- Protecting i64 excellence is non-negotiable.

## Execution order

1. Lock this design. ← current
2. Scaffold residual_few_wide + counting + STRUCTURE + low_disorder + HE for int32 (and uint32).
3. Wire pure_residual::sort_i32 / sort_u32.
4. Extend harness; measure; document.
5. Only then open float / generic.

**Ownership (BEASTIE BOYZ)**
- Design lock: Benjamin
- residual_few_wide + menu scaffold for i32/u32: Harper
- Multi-type harness extension: Lucas

**THE BEASTIE BOYZ**
