# Red-Gate Status — Harvest P5.1 / v1.5.1-c Baseline

**Status:** LOCKED 2026-08-11  
**Baseline:** Harvest P5.1 pure residual menu (n=1e6, median-of-7)  
**Suite:** 38-pattern extended suite (`P5_1_FINAL.csv`)

## Rule

Any new measurement that is **slower** than the documented floor for a pattern, or that worsens the group geometric mean beyond normal noise, is a **regression**, not noise. It must be logged on the floor ledger before any stronger claim language is considered.

## Documented floors (pure / pdqsort > 1.0×)

| Pattern | Priority | pure/pdq | pure ms | pdq ms | Notes |
|---------|----------|---------:|--------:|-------:|-------|
| **gaussian** | high | **1.102×** | 38.724 | 35.146 | HE residual quality floor on near-uniform continuous data |
| **equal_heavy** | locked | **1.027×** | 1.641 | 1.598 | Near-parity; within measurement noise band |

## Near-parity (0.95–1.00×) — watch list

| Pattern | Priority | pure/pdq |
|---------|----------|---------:|
| gaussian_mix | high | 0.982× |
| plateau | medium | 0.961× |

## Group geometric means (frozen)

| Group | n | geo pure/pdq |
|-------|--:|-------------:|
| Locked (original 11) | 11 | **0.553×** |
| High priority | 13 | **0.571×** |
| Medium | 13 | **0.400×** |
| Full extended (38) | 38 | **0.500×** |

## Red-gate invariants

1. Pure residual path only (no library pdq/ska/std::sort dispatch on residual routes).
2. EXTERNAL-clean (visible probe metrics only).
3. Worst-case \(O(n \log n)\).
4. Every new floor discovered by residual hunting or real-world data is appended to this ledger with seed + generator.
5. No field-level or “dominates SOTA” language until Phase 5 decision gate is green.

## Citation requirement

All subsequent claims must cite:
- `PHASE0_BASELINE_LOCK.md`
- this red-gate status
- the SHA-256 manifest in `SHA256SUMS.txt`

**THE BEASTIE BOYZ**
