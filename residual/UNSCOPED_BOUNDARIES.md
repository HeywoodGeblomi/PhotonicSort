# Unscoped Soft Residual Boundaries

**Purpose:** Machine-readable contract separating the **charged soft-gate surface** (hybrid Field-Level) from **unscoped pure residual path limits**.

Honesty primary. EXTERNAL-clean. THE BEASTIE BOYZ · 2026-08-17

---

## Charged surface (hybrid residual — soft@1.20 vs pdq = 0)

Patterns on the Field-Level charged surface are hard-fail at soft_threshold **1.20** vs **pdq** under DEFAULT dual residual:

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps · reverse_segments · equal_heavy · mixed_blocks

**Hard-fail exclusions:** none (v0.5).

Gate: `scripts/sp_money_shot_gate.py` · Claim: `docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md`

Hybrid routing on HE / gaussianish: **library ska** (talent-gated on borderline). This is why gaussianish is in-scope for hybrid soft=0 while pure residual still has an HE path limit.

---

## Unscoped pure residual path limits (documented floors)

These are **not** Field-Level failures. They apply only to the pure residual menu (no library ska/pdq dispatch):

| Floor | Approx ratio vs pdq | Status |
|-------|--------------------:|--------|
| few_unique_k16 | ~1.0–1.2× | residual quality limit |
| Gaussian HE (full menu) | ~1.1–1.2× | residual quality / path limit (CLOSED 2026-08-11) |
| push_middle | ~1.5–1.7× | 3-run merge residual quality limit |

See `docs/residual_improve/GAUSSIAN_HE_PHASE_CLOSE.md` and `residual/README.md`.

**Rule:** Do not raise soft_threshold or claim pure residual unscoped soft=0 to paper over these floors. Hybrid DEFAULT path is the production soft-gate surface.

---

## Boundary rule (for residual work)

1. **Charged + hybrid** → soft@1.20 vs pdq must stay 0; money-shot is the authority.
2. **Pure residual HE / residual-quality** → path limits stay documented; new residual levers only if they improve the pure surface without regressing hybrid gates.
3. **Unscoped patterns** outside the charged list are not Field-Level hard-fail unless explicitly promoted with multi-arch evidence.

---

**End of boundaries contract.**
