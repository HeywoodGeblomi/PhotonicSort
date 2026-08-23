# FIELD_LEVEL_CLAIM — PHO-EXT-001 Phase 1 Charged Expansion

**Date:** 2026-08-23  
**Status:** **LOCKED**  
**Base:** FIELD_LEVEL_CLAIM_v0.5  
**Tip:** 3c43b0c8 (PR #177)  
**Squad:** THE BEASTIE BOYZ / Blam  
**Configuration:** `residual/hybrid_residual_menu.hpp` (DEFAULT dual residual / residual talent; escape `-DCLASSICAL_RESIDUAL`)  
**Honesty primary. EXTERNAL-clean.**

---

## Delta from v0.5

| Pattern | Lever / Intent | Status |
|---------|----------------|--------|
| late_phase_shift_mixed_blocks | Late phase-shift on mixed ascending/descending blocks; σ_Δ polarity / near-floor while classical remains high → dual_owned=false | **added to charged hard-fail** |
| near_tie_he_odd_starvation | Controlled odd-event starvation on near-tie HE; |σ_Δ| near dual_confirm floor → dual_owned=false | **added to charged hard-fail** |

**Acceptance (all met):**
- soft@1.20 = 0, major = 0, ok = 1 on the expanded charged surface (x86_64 + aarch64, n=1e6) — tip Actions (sp-multi-arch #32 family) + commit claim.
- `hybrid_residual_menu.hpp` and `secondary_parity.hpp` **completely untouched**. No χ, no OwnershipHarness, no internal irreversible state.
- dual_owned=false on both patterns as designed (local probe). Residual abates correctly under pure dual-evidence.
- Docker.sp / sp_money_shot_gate.py already include both cells; money-shot GREEN line remains the stranger path.
- Flip history: empty. No post-hoc residual retune.

---

## Charged surface (hard-fail) — Phase 1 expanded

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps · reverse_segments · equal_heavy · mixed_blocks · **late_phase_shift_mixed_blocks** · **near_tie_he_odd_starvation**

**Hard-fail exclusions:** *none*

---

## Reproduce (unchanged)

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
# expected: SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1
```

---

## What this is not

Same as v0.5: not a universal replacement, not every residual soft vs specialized closed, not a complexity theorem, not asymptotic novelty. EXTERNAL-clean only. See [`NON_CLAIMS.md`](../../NON_CLAIMS.md).

THE BEASTIE BOYZ / Blam · 2026-08-23
