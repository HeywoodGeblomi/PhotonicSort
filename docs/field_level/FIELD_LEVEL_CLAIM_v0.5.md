# FIELD_LEVEL_CLAIM_v0.5 — Full Charged Surface (Secondary Parity)

**Date:** 2026-08-14  
**Status:** **LOCKED**  
**Squad:** THE BEASTIE BOYZ / Blam  
**Configuration:** `residual/hybrid_residual_menu.hpp` (DEFAULT dual residual / residual talent; escape `-DCLASSICAL_RESIDUAL`)  
**Honesty primary. EXTERNAL-clean.**

---

## Delta from v0.4

| Pattern | Lever | Status |
|---------|-------|--------|
| mixed_blocks | O(n) alternating sorted/disordered block residual (`residual_mixed_blocks.hpp`) + cheap 4-block pre-check | **promoted to charged hard-fail** |

**No residual exclusions remain** on the Expanded Field Suite charged surface.

Residual attack: PR #144.  
Claim promote: PR #145.  
Post-merge SP multi-arch (full v0.5 CHARGED) GREEN: [run 31807222980](https://github.com/HeywoodGeblomi/PhotonicSort/actions/runs/31807222980).

---

## Claim (narrow)

On the **full charged** Expanded Field Suite surface, the hybrid residual configuration with Secondary Parity dual-evidence achieves:

1. **Charged soft@1.20 vs pdq = 0** on **x86_64 and aarch64** (n=1e6, R≥3)
2. **Charged major (>1.5×) = 0** on both ISAs
3. **ok = 1** full suite correctness
4. **Stat-sig:** bootstrap 95% CI upper on median `menu/pdq` ≤ 1.20 for every charged cell on the prior surface (n=1e6, R=11, both ISAs); mixed_blocks added under residual attack and hard-gated at point estimate
5. **Independent host reproduce:** Windows Docker Desktop money-shot path remains valid

This remains a **path-(a) empirical evidence package for the SP dual-evidence residual-selection technique**. It is not a SOTA displacement claim and not a complexity theorem.

---

## Charged surface (hard-fail) — v0.5

sorted · reverse · almost_sorted · few_k4_dense · few_k16_dense · organpipe · sawtooth · push_middle · runs_noise · random · gaussianish · adversarial · db_pk · timestamps · reverse_segments · equal_heavy · **mixed_blocks**

**Hard-fail exclusions:** *none*

---

## Soft-attack residual levers (on main)

| Pattern | Lever |
|---------|-------|
| reverse_segments | O(n) fixed-block reverse (`residual_reverse_segments.hpp`) |
| equal_heavy | early equal-sample → library pdq |
| mixed_blocks | O(n) alternating-block detector + disordered-block residual + cheap pre-check (`residual_mixed_blocks.hpp`) |

---

## What Secondary Parity is

Continuous probe stream → odd-event restriction → ordinal partitions → contrast σ_Δ.  
Dual-evidence residual commitment: classical HE signal **and** σ_Δ polarity required for ska on borderline HE; single evidence → residual_pdq (abated). Strong classical HE still takes library ska (no abate on pure random).

DEFAULT production path (no flag required). Escape hatch only: `-DCLASSICAL_RESIDUAL`. EXTERNAL-clean visible metrics only. No χ. No internal irreversible state.

---

## What this is not

1. **Not a universal replacement** for library pdq/ska/Timsort/std::sort outside the measured charged surface.
2. **Not a claim that every residual soft vs specialized is closed** — gate is vs **pdq**; specialized may still win isolated cells.
3. **Not a novel adaptive complexity theorem** (see path-(b) docs for separate asymptotic work).
4. **Not asymptotic novelty.** EXTERNAL-clean only.

---

## Reproduce

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
# expected: SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1
```

Formal R=11:

```bash
docker run --rm -e STAT_SIG=1 photonic-sp
```

---

## Non-claims reminder

See [`NON_CLAIMS.md`](../../NON_CLAIMS.md). Packaging does not expand claims.

THE BEASTIE BOYZ / Blam · 2026-08-15
