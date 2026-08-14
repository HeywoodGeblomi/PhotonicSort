# Orthogonality Theorem — σ_Δ ⟂ Classical Presortedness

**Status:** **STATED** · empirical Claim A GREEN on F1/F2 · formal lemma at engineering rigor  
**Date:** 2026-08-14  
**Squad:** THE BEASTIE BOYZ / Blam  
**Track:** Epic 1 (path-b)  
**Honesty primary. EXTERNAL-clean.**

---

## One line

There exist infinite families of arrays that agree exactly on classical presortedness measures \((\mathrm{Inv}, \mathrm{Runs}, \alpha)\) (and on fixed-size primary probe statistics) while disagreeing on the sign of the Secondary Parity contrast \(\sigma_\Delta\). Therefore residual selectors that observe only classical summaries cannot simulate selectors that observe \(\sigma_\Delta\).

---

## Definitions

### Classical vector

For an array \(X\) of length \(n\):

\[
C(X) = \bigl(\mathrm{Inv}(X),\; \mathrm{Runs}(X),\; \alpha(X)\bigr)
\]

where \(\mathrm{Inv}\) is inversion count, \(\mathrm{Runs}\) is the number of monotonic runs, and \(\alpha\) is any fixed linear disorder aggregate computable from a constant-size sample (or from the full array if desired). Primary Gyro / sample statistics used by hybrid residual menus are treated as functions of \(C\) or of a fixed-size sample of \(X\).

### Secondary Parity contrast

Given a continuous probe stream \(\{m_t\}_{t=0}^{T-1}\) derived from windowed local disorder of \(X\):

1. Events \(\tau_i\) = threshold crossings of \(m_t\) (or sign changes).
2. Restrict to odd-indexed events \(\mathcal{O}\).
3. Partition \(\mathcal{O}\) by ordinal index into \(E_{\mathrm{even}}\) and \(E_{\mathrm{odd}}\).
4.

\[
\sigma_\Delta(X) = \mathbb{E}[m_\tau \mid \tau \in E_{\mathrm{even}}] - \mathbb{E}[m_\tau \mid \tau \in E_{\mathrm{odd}}].
\]

(Implementation: `residual/secondary_parity.hpp`, `primitive/dual_evidence/`.)

---

## Theorem (Orthogonality / Non-reducibility)

**Claim A.**  
There is no continuous function \(g\) such that

\[
\mathrm{sign}(\sigma_\Delta(X)) = g\bigl(C(X)\bigr)
\]

for all arrays \(X\) in the families below. Equivalently: \(\sigma_\Delta\) is not a function of the classical vector on those families.

**Stronger form (residual decision).**  
There exist pairs \((X^+, X^-)\) with

\[
C(X^+) = C(X^-), \qquad \sigma_\Delta(X^+) \cdot \sigma_\Delta(X^-) < 0,
\]

such that a dual-evidence residual selector (classical + sign of \(\sigma_\Delta\)) assigns different residual talents to \(X^+\) and \(X^-\), while any classical-only selector that is a function of \(C\) must assign the same talent to both.

---

## Proof sketch (constructive)

### Family F1 — spike-phase separation

**Construction** (`gen_f1_spike_phase`):

1. Base: sorted array of length \(n\).
2. Place \(B\) local reverse-spikes of increasing amplitude at positions controlled by a phase bit \(\phi \in \{0,1\}\).
3. Phase shift moves large spikes onto opposite ordinal classes of the odd-events of the windowed inv-density stream.

**Invariants (verified n = 10³ … 10⁵):**

| Quantity | Phase 0 vs Phase 1 |
|----------|--------------------|
| \(\mathrm{Inv}\) | **exact match** |
| \(\mathrm{Runs}\) | **exact match** |
| \(\sigma_\Delta\) | **polarity flips** |

Because \(C(X^0) = C(X^1)\) while \(\mathrm{sign}(\sigma_\Delta)\) differs, no function of \(C\) alone recovers the sign.  
**Claim A holds on F1.**

### Family F2 — adversarial residual modulation

**Construction:** reverse-segments / organpipe skeleton with low-amplitude polarity modulation of local slope or windowed inv-density zero-crossings so that classical statistics stay within tolerance \(\varepsilon_{\mathrm{stat}}\) while ordinal parity of odd events flips.

**Verified:** classical match within \(\varepsilon\); \(\sigma_\Delta\) polarity flips; dual-evidence residual mock routes the two phases differently (classical-only cannot).

**Claim A holds on F2** (including residual-decision form).

### Infinite family

For each \(n = 2^k\) in a geometric range, F1 produces a distinct pair with exact classical match and opposite \(\sigma_\Delta\) polarity. The construction scales: spike count and amplitude schedule grow with \(n\), preserving the phase-shift argument. Hence the separating set is infinite.

---

## Corollary (minimax residual improvement — Claim B, conditional)

Let \(\mathcal{R}\) be a residual menu and \(\mu_C\) the minimax residual cost of a classical-only selector over a cell of constant \(C\).  
On any soft-spot class \(\mathcal{C}\) that contains an F1/F2 pair whose optimal residuals differ, a dual-evidence selector that conditions on \(\mathrm{sign}(\sigma_\Delta)\) can achieve

\[
\mu_{\sigma}(\mathcal{C}) \;<\; \mu_C(\mathcal{C})
\]

whenever the two phases prefer different members of \(\mathcal{R}\).

**Status of Claim B:** residual-routing contrast mock GREEN on F2 (differential OWNED vs PREPARED). Full cost-gap measurement on suite residual times remains open engineering work — not required for Claim A.

---

## Relation to F5–F6 (M_S-optimality)

F5–F6 proves hybrid residual is \(M_S\)-optimal over menu \(\mathcal{R}\) for fixed-width integers. That theorem treats the **sample measure** \(M_S\) as given; it does **not** claim that \(M_S\) captures everything residual-relevant.

Orthogonality says: there is residual-relevant structure (\(\sigma_\Delta\)) outside classical \(C\) and, for sufficiently coarse samples, outside what a fixed-size primary probe resolves. Dual-evidence extends the observed measure so that some cells of constant classical \(C\) split, improving minimax cost inside those cells when the menu contains distinct talents for the two phases.

The two results are complementary:
- F5–F6: optimality **given** the observed measure.
- Orthogonality: the observed measure should include \(\sigma_\Delta\) if classical summaries alone are used.

---

## Non-claims

1. **Not** a claim that \(\sigma_\Delta\) improves every pattern on every suite.
2. **Not** classical Inv-optimality or comparison-model asymptotics.
3. **Not** χ / internal irreversible state. EXTERNAL-clean visible metrics only.
4. **Not** a completed journal-formal proof with all ε-quantifiers written out — engineering rigor on constructive families; journal polish is future work.
5. **Not** Claim B cost-gap on the full charged surface (open measurement).

---

## Reproduce Claim A (F1)

```bash
# from repo root or Phase0_PathA/secondary_parity/
python3 -c "
from generators import f1_pair
for n in [1024, 2048, 8192, 32768]:
    p = f1_pair(n=n, n_spikes=max(8, n//2000))
    print(n, 'classical_match', p['classical_match'],
          'sigma_flip', p['sigma_flip'],
          'd0', round(p['sigma_delta0'], 4), 'd1', round(p['sigma_delta1'], 4))
"
```

Success: `classical_match True` and `sigma_flip True` for each \(n\).

---

## Next formal steps

| Step | Goal |
|------|------|
| A1 | Write ε-quantified statement for F2 (classical within ε, σ_Δ Ω(1) separation) |
| A2 | Prove phase-shift argument without relying on empirical tables (analytic windowed-density model) |
| B1 | Measure residual wall-clock gap on F1/F2 under dual vs classical hybrid |
| B2 | If gap holds, promote Claim B from conditional to measured |

---

**THE BEASTIE BOYZ / Blam**
