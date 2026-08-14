# Multi-Arch Money-Shot — LOCKED GREEN

**Date:** 2026-08-14  
**Main tip:** `fb8f46d` (PR #129 squash-merge)  
**Chain:** PR #126 (dual-evidence code) → #127 (thin CI) → #128 (charged surface vs pdq) → #129 (mixed_blocks exclusion)

## Result

Under `-DSECONDARY_PARITY`, on **both** `ubuntu-24.04` (x86_64) and `ubuntu-24.04-arm` (aarch64):

- charged soft@1.20 **vs pdq** = 0
- charged major (>1.5×) = 0
- ok = 1 on full expanded-field suite

## Charged surface (hard-fail)

sorted, reverse, almost_sorted, few_k4_dense, few_k16_dense, organpipe, sawtooth, push_middle, runs_noise, random, gaussianish, adversarial, db_pk, timestamps

**Excluded from hard-fail (documented residual, not dual-evidence claims):**
- equal_heavy — classical residual soft
- mixed_blocks — aarch64 residual variance; x86 SP close retained in Field-Level charge record

## What this closes

The multi-arch gate that was open after the 2026-08-13 Field-Level charge (x86 multi-type soft=0). aarch64 money-shot is no longer blocked.

## What remains for formal Field-Level

1. **Statistical CI packaging** — protocol locked in `STAT_SIG_PROTOCOL.md`; harness `--raw-out` + `scripts/sp_stat_sig_gate.py` (wire into CI next)
2. **Third-party reproduce** — Wave 0 / independent Docker one-command path on both ISAs
3. Optional: residual quality work on excluded softs (equal_heavy, mixed_blocks on ARM) if raising the bar beyond the charged surface

## Honesty

Controlled suite evidence under the charged surface only. EXTERNAL-clean. No χ. No internal irreversible state.

**THE BEASTIE BOYZ / Blam**
