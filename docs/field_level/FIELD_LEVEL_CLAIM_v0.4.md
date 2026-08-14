# FIELD_LEVEL_CLAIM_v0.4 — Expanded Charged Surface (Secondary Parity)

**Date:** 2026-08-14  
**Status:** CHARGED SURFACE EXPANDED (gates pending CI re-confirm)  
**Squad:** THE BEASTIE BOYZ / Blam  
**Configuration:** `residual/hybrid_residual_menu.hpp` + `-DSECONDARY_PARITY`  
**Honesty primary. EXTERNAL-clean.**

---

## Delta from v0.3

Soft-attack residual levers already on main closed two former residual softs vs pdq:

| Pattern | Lever | Approx ratio vs pdq (local i64 n=1e6) |
|---------|-------|---------------------------------------|
| reverse_segments | O(n) fixed-block reverse (`residual_reverse_segments.hpp`) | ~0.08× |
| equal_heavy | early equal-sample → library pdq path | ~1.0× |

These two patterns are **promoted into the charged hard-fail surface**.  
`mixed_blocks` remains excluded (residual quality vs specialized + aarch64 history).

---

## Claim (narrow)

On the **expanded charged** Expanded Field Suite surface, the hybrid residual configuration with Secondary Parity dual-evidence achieves:

1. **Charged soft@1.20 vs pdq = 0** on **x86_64 and aarch64** (n=1e6, R≥3)
2. **Charged major (>1.5×) = 0** on both ISAs
3. **ok = 1** full suite correctness
4. **Stat-sig:** bootstrap 95% CI upper on median `menu/pdq` ≤ 1.20 for every charged cell (n=1e6, R=11, both ISAs) — *re-confirm after this expansion*
5. **Independent host reproduce:** Windows Docker Desktop money-shot GREEN under the prior (v0.3) charged set; Wave 0 path remains valid and will hard-gate the two new patterns after merge

This remains a **path-(a) empirical evidence package for the SP dual-evidence residual-selection technique**. It is not a SOTA displacement claim and not a complexity theorem.

---

## Charged surface (hard-fail) — v0.4

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps · **reverse_segments** · **equal_heavy**

**Excluded from hard-fail** (documented residual / not dual-evidence claims):

| Pattern | Note |
|---------|------|
| mixed_blocks | residual quality vs specialized (~1.5×) + aarch64 residual variance history; x86 SP close retained in charge record |

---

## Measured evidence (inherited + expansion)

### Multi-arch money-shot (R=3, Actions) — v0.3 surface

| Gate | x86_64 | aarch64 |
|------|--------|---------|
| charged soft@1.20 vs pdq | **0** | **0** |
| charged major | **0** | **0** |
| ok | **1** | **1** |

Workflow: `sp-multi-arch.yml` · lock: [`MULTI_ARCH_MONEY_SHOT_LOCKED.md`](./MULTI_ARCH_MONEY_SHOT_LOCKED.md)

After this PR merges, the same workflow hard-fails on the two new patterns. Re-confirm GREEN is required before claiming the expanded surface.

### Soft-attack local (pre-expansion)

| Pattern | ratio vs pdq (i64 n=1e6) | Status |
|---------|--------------------------|--------|
| reverse_segments | ~0.08× | CLOSED by O(n) block reverse |
| equal_heavy | ~1.0× | CLOSED by library-pdq equal path |
| mixed_blocks | ≤1.20 vs pdq / ~1.5× vs best specialized | residual quality limit; stays excluded |

### Stat-sig packaging (R=11, bootstrap CI upper) — v0.3 surface

| Gate | x86_64 | aarch64 |
|------|--------|---------|
| charged CI-upper soft | **0** | **0** |
| charged major | **0** | **0** |

Workflow: `sp-stat-sig.yml` · protocol: [`STAT_SIG_PROTOCOL.md`](./STAT_SIG_PROTOCOL.md)  
Run: https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31757149142

### Independent host (Wave 0)

| Host | Result |
|------|--------|
| Windows + Docker Desktop (Blam, 2026-08-14) | **SP MONEY-SHOT GREEN** under v0.3 charged set |

One-command remains:

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

Soft-attack levers (already on main): `residual/residual_reverse_segments.hpp` + equal-heavy early-exit to library pdq.

---

## What this is not

1. **Not a universal replacement** for library pdq/ska/Timsort/std::sort outside the measured charged surface.
2. **Not a claim that every residual soft on the full 23-pattern suite is closed.** mixed_blocks remains a residual-quality limit.
3. **Not a novel adaptive complexity theorem** (see path-(b) docs for separate asymptotic work).
4. **Not asymptotic novelty.** EXTERNAL-clean only.
5. Prior pure-residual and hybrid-without-SP packages remain historical evidence surfaces; v0.4 is the expanded charged-surface packaging of SP dual-evidence + soft-attack closures.

---

## Reproduce checklist for a third party

| Step | Command / location | Success line |
|------|-------------------|--------------|
| Money-shot | Docker one-command above | `SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1` |
| Stat-sig | `-e STAT_SIG=1` or R=11 + `sp_stat_sig_gate.py` | `SP STAT-SIG GREEN charged CI-upper soft=0 major=0` |
| CI | Actions `sp-multi-arch` / `sp-stat-sig` | both jobs success on expanded charged set |

Compare **ratios**, not absolute milliseconds. Host noise is expected; soft-gate is the auditable surface.

---

**THE BEASTIE BOYZ / Blam**
