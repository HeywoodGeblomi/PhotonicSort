# FIELD_LEVEL_CLAIM_v0.3 — Secondary Parity Dual-Evidence Surface

**Date:** 2026-08-14  
**Status:** CHARGED SURFACE GATES MET  
**Squad:** THE BEASTIE BOYZ / Blam  
**Configuration:** `residual/hybrid_residual_menu.hpp` + `-DSECONDARY_PARITY`  
**Honesty primary. EXTERNAL-clean.**

---

## Claim (narrow)

On the **charged** Expanded Field Suite surface, the hybrid residual configuration with Secondary Parity dual-evidence achieves:

1. **Charged soft@1.20 vs pdq = 0** on **x86_64 and aarch64** (n=1e6, R≥3)
2. **Charged major (>1.5×) = 0** on both ISAs
3. **ok = 1** full suite correctness
4. **Stat-sig:** bootstrap 95% CI upper on median `menu/pdq` ≤ 1.20 for every charged cell (n=1e6, R=11, both ISAs)
5. **Independent host reproduce:** Windows Docker Desktop money-shot GREEN (Blam, 2026-08-14)

This is a **path-(a) empirical evidence package for the SP dual-evidence residual-selection technique**. It is not a SOTA displacement claim and not a complexity theorem.

---

## Charged surface (hard-fail)

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps

**Excluded from hard-fail** (documented residual / not dual-evidence claims):

| Pattern | Note |
|---------|------|
| equal_heavy | classical residual quality vs library pdq |
| mixed_blocks | aarch64 residual variance; x86 SP close retained in charge record |
| reverse_segments | residual quality (ratio_best SOFT possible; not charged vs pdq) |

---

## Measured evidence

### Multi-arch money-shot (R=3, Actions)

| Gate | x86_64 | aarch64 |
|------|--------|---------|
| charged soft@1.20 vs pdq | **0** | **0** |
| charged major | **0** | **0** |
| ok | **1** | **1** |

Workflow: `sp-multi-arch.yml` · lock: [`MULTI_ARCH_MONEY_SHOT_LOCKED.md`](./MULTI_ARCH_MONEY_SHOT_LOCKED.md)

### Stat-sig packaging (R=11, bootstrap CI upper)

| Gate | x86_64 | aarch64 |
|------|--------|---------|
| charged CI-upper soft | **0** | **0** |
| charged major | **0** | **0** |

Workflow: `sp-stat-sig.yml` · protocol: [`STAT_SIG_PROTOCOL.md`](./STAT_SIG_PROTOCOL.md)  
Run: https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31757149142

### Independent host (Wave 0)

| Host | Result |
|------|--------|
| Windows + Docker Desktop (Blam, 2026-08-14) | **SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1** |

One-command: [`../../reproduce/SP_FIELD_LEVEL.md`](../../reproduce/SP_FIELD_LEVEL.md) · lock: [`WAVE0_SP_REPRODUCE.md`](./WAVE0_SP_REPRODUCE.md)

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp sh -c \
  './suite_hybrid --n 1000000 --reps 3 --out results_sp.csv && python3 scripts/sp_money_shot_gate.py results_sp.csv'
```

---

## What Secondary Parity is

Continuous probe stream → odd-event restriction → ordinal partitions → contrast σ_Δ.  
Dual-evidence residual commitment: classical HE signal **and** σ_Δ polarity required for ska_OWNED on borderline HE; single evidence → residual_pdq (abated). Strong classical HE still takes library ska (no abate on pure random).

Flag-gated: compile with `-DSECONDARY_PARITY`. EXTERNAL-clean visible metrics only. No χ. No internal irreversible state.

Code: `residual/secondary_parity.hpp` · wired in `residual/hybrid_residual_menu.hpp`

---

## What this is not

1. **Not a universal replacement** for library pdq/ska/Timsort/std::sort outside the measured charged surface.
2. **Not a claim that every residual soft on the full 23-pattern suite is closed.**
3. **Not a novel adaptive complexity theorem** (see path-(b) docs for separate asymptotic work).
4. **Not asymptotic novelty.** EXTERNAL-clean only.
5. Prior pure-residual and hybrid-without-SP packages (v0.1 / v0.2) remain historical evidence surfaces; v0.3 is the SP dual-evidence packaging.

---

## Reproduce checklist for a third party

| Step | Command / location | Success line |
|------|-------------------|--------------|
| Money-shot | Docker one-command above | `SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1` |
| Stat-sig | `-e STAT_SIG=1` or R=11 + `sp_stat_sig_gate.py` | `SP STAT-SIG GREEN charged CI-upper soft=0 major=0` |
| CI | Actions `sp-multi-arch` / `sp-stat-sig` | both jobs success |

Compare **ratios**, not absolute milliseconds. Host noise is expected; soft-gate is the auditable surface.

---

**THE BEASTIE BOYZ / Blam**
