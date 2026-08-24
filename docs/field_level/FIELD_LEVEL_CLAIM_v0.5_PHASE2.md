# FIELD_LEVEL_CLAIM v0.5 — Phase 2 / Tranche 2

**Status:** SURFACE EXPANDED — soft@1.20 re-lock **PENDING CI money-shot both ISAs**  
**Builds on:** FIELD_LEVEL_CLAIM_v0.5 + Phase 1 (tip 3c43b0c8 / PR #177)  
**Change character:** Charged surface expansion only. Residual decision path untouched.  
**EXTERNAL-clean.** No χ. Empty flip history.

## Constraint lock

- `hybrid_residual_menu.hpp` decision logic — **NO CHANGE**
- `secondary_parity.hpp` dual_confirm / dual_evidence — **NO CHANGE**
- External `dual-residual` core decision table — **NO CHANGE**
- Thresholds 0.04 / 0.005 / 0.1 / 0.2 — **immutable**

## Phase 2 charged additions

| Pattern | Generator | Expected dual_owned behavior |
|---------|-----------|------------------------------|
| `late_phase_shift_dense` | `gen_late_phase_shift_dense` | Late cut @0.80 + denser local swaps → classical elevated, σ_Δ near floor → dual_owned=false band |
| `near_tie_he_even_starvation` | `gen_near_tie_he_even_starvation` | Even-event starvation complement of Phase 1 odd starvation → σ_Δ near floor, contested HE |

Both stress the pure dual-evidence dual_owned boundary. No pure residual lever required at authoring time; menu routes under existing DEFAULT dual residual.

## Full charged surface (hard-fail) after Phase 2

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps · reverse_segments · equal_heavy · mixed_blocks · late_phase_shift_mixed_blocks · near_tie_he_odd_starvation · **late_phase_shift_dense** · **near_tie_he_even_starvation**

**Hard-fail exclusions:** *none*

## Gates (must re-lock before claim promotion)

| Gate | Required |
|------|----------|
| Charged soft@1.20 vs pdq (x86_64 + aarch64, n=1e6) | **0** |
| Charged major (>1.5×) | **0** |
| ok full-suite | **1** |
| Flip history | **empty** |

## Reproduce

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
# Success line: SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1
```

## Flip history

*(empty — no post-hoc residual retune)*

## Non-claims

- Not a decision-path change.
- Not a dual-residual core change.
- Not a complexity theorem.
- soft@1.20 / major gates vs **pdq** only; specialized may still win isolated cells.
- EXTERNAL-clean visible metrics only — no χ.

THE BEASTIE BOYZ · 2026-08-24
