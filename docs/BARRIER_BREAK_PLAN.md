# Barrier Break Plan — PhotonicSort

**Status:** Wave 0 COMPLETE · Wave 1 residual floors LOCKED for barriers 1+2 · 2026-08-11  
**Parent:** [`FIELD_LEVEL_ATTACK_PLAN.md`](./FIELD_LEVEL_ATTACK_PLAN.md)  
**Squad:** THE BEASTIE BOYZ

---

## Goal

Break the remaining barriers between the current pure residual menu and **field-level entry** under the locked Field-Level Attack Plan paths (a)/(b)/(c).

## Barriers

| # | Barrier | Attack | Success | Kill | Status |
|---|---------|--------|---------|------|--------|
| **5** | No independent reproduction | One-command Docker harness + published suite + baselines | External party runs harness; ratios match reference within noise | Cannot produce one-command reproducible harness → stop claiming verification progress | ✅ COMPLETE (PR #49) |
| **2** | few_unique_k16 ~1.0–1.2× | Residual quality at mid-card + routing | ≤1.05× pdq on few_unique_k16 (k=8..32 band) | Still >1.15× after focused attack → permanent residual limit | ✅ CLOSED (v2.5) |
| **1** | Gaussian HE ~1.1–1.2× | Measure path tax vs residual quality | ≤1.05× pdq **or** permanent path limit with measured tax breakdown | Competitive residual + tax-only → document permanent path limit | Residual quality OK / path-tax documented |
| **3** | Suite geo held ~0.80× | Closing #1+#2; geo must move | Geo ≤0.85–0.90× vs best specialized and below prior baseline with significance | Geo flat after residual floors closed → freeze specialized claim | Improved (~0.88–0.90×); re-assess |
| **4** | Sequential int64 only | Multi-type: int32/uint32 → float → generic comparator; protect i64 pure residual | Competitive vs pdq on ≥2 non-i64 POD types | Generic destroys i64 fast-path or never competitive → specialized only | Open (Wave 2) |
| **6** | No complexity result | Adaptive bound tied to measurable disorder the probe approximates | Publishable statement with matching algorithm | No concrete statement in fixed window → park theory | Open (Wave 3) |
| **7** | “Not field-level” language | Automatic: drops only when (a)/(b)/(c) met | Path (a)/(b)/(c) green | Do not drop language early | Held |

## Execution waves

**Wave 0 — Independent reproduction (barrier 5)** ✅ COMPLETE (PR #49)  
Docker + one-command harness under `reproduce/`. Third-party runnable.

**Wave 1 — Residual floors (barriers 2, 1, 3)**  
Barrier 2 CLOSED. Barrier 1 residual-quality OK / path-tax. Geo improved to ~0.88–0.90×. See [`WAVE1_RESIDUAL_FLOORS.md`](./phase0_field/WAVE1_RESIDUAL_FLOORS.md).

**Wave 2 — Multi-type surface (barrier 4)** ← next candidate  
Mandatory for path-(a) claims vs pdq/Timsort/std.

**Wave 3 — Theory (barrier 6)**  
Parallel; does not block Waves 0–2.

**Wave 4 — Claim language (barrier 7)**  
Only when Field-Level Attack Plan success metrics are met.

## Hard constraints

EXTERNAL-clean residual selection · Protect pure residual excellence · Worst-case O(n log n) · No field-level language until criteria met · Multi-type mandatory for displacement of pdq/Timsort/std::sort

## Non-claims (continuous)

Until path (a), (b), or (c) is met:

- Not a field-level breakthrough
- Not a universal replacement for library pdq / ska / Timsort / std::sort
- Not multi-type / multi-architecture until those surfaces exist and are measured
- Not independently verified until external parties have run the published harness

**THE BEASTIE BOYZ**
