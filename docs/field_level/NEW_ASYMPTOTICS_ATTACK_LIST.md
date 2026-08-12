# New Asymptotics — Petaous Attack List

**Status:** EXPLORATION / CAPTURE LIST ONLY. No theorems. No optimality claimed.  
**Date:** 2026-08-12 · THE BEASTIE BOYZ  
**Context:** Classical Inv/Runs/LRM/α optimality **blocked** for hybrid `f` (see OPTIMALITY_ATTACK.md).  
**Purpose:** Zoom out. Unexplored asymptotic territory for cultivation.

---

## One line

Stop re-proving Mannila. Attack **partial observation, residual menus, and model mixtures** — the actual objects hybrid residual is.

---

## Priority attack list

Ordered by (leverage on our surface) × (novelty) × (near-term viability).

| # | Attack line | Core question | Fit to hybrid | Difficulty |
|---|-------------|---------------|---------------|------------|
| **P1** | **Sample-measure optimality** | Sorting cost under access only to M_S = probes of size S ≪ n | Direct — T1 is already f(M_S) | Med |
| **P2** | **Misrouting regret** | E[T(f(sample)) − T(f(true M))] → 0 when? | Exact T3 formalization | Med |
| **P3** | **Multi-residual decision complexity** | Min probe cost to select best residual from a finite menu | Hybrid menu = finite classes | Med–Hard |
| **P4** | **Instance-optimal residual selection** | Competitive with hindsight-best residual per instance | Stronger than measure-opt | Hard |
| **P5** | **Synergistic word-RAM bound** | Joint (disorder ∧ cardinality ∧ word size w) | ska + counting + pdq live here | Med |
| **P6** | **Local disorder concentration** | dense_inv / block-α as primary measure | Already in hybrid gates | Med |
| **P7** | **Self-improving residual router** | After training on 𝒟, T → O(n + H_𝒟(π)) | Distribution-family opt | Hard |
| **P8** | **Learned-routing asymptotics** | Sample complexity to learn a near-optimal f from labeled instances | Meta over T1 | Hard |
| **P9** | **Cache / I/O adaptive** | Runs help sequential scans; bound in external-memory model | Practical systems | Med |
| **P10** | **Streaming residual class** | One-pass discovery of residual class with o(n) memory | Online T1 | Hard |
| **P11** | **Probe-strategy optimality** | Adaptive vs non-adaptive probes; optimal S and order | Improve sample_full | Med |
| **P12** | **Fine-grained conditional LB** | SETH / 3SUM-style barriers for adaptive residual selection | Kill false targets | Hard |

---

## Detail — top tier (P1–P6)

### P1 · Sample-measure optimality
**Object:** M_S(X) = (Înv_S, Runŝ_S, û_S, êq_S, dôm_S).  
**Claim shape (if proved):** any algorithm given only M_S needs Ω(LB(M_S)); hybrid achieves O(LB(M_S)).  
**Why new:** classical theory assumes exact M. Partial observation is the real model for practical adaptive sorts.  
**First move:** information-theoretic lower bound under fixed-S sampling of inversions/runs.

### P2 · Misrouting regret
**Object:** R = T(f(sample)) − T(f(true)).  
**Claim shape:** R = o(T*) under concentration of Înv_S → Inv/n, etc.  
**Why new:** turns T3 from a risk note into a theorem.  
**First move:** Hoeffding / VC bound on inv-rate estimator; residual cost Lipschitz in measure.

### P3 · Multi-residual decision complexity
**Object:** finite residual menu ℛ = {STRUCTURE, PURE, PDQ, SKA, …}.  
**Claim shape:** probe complexity to identify argmin_{r∈ℛ} T_r(X) within factor (1+ε).  
**Why new:** decision-theoretic view of hybrid; not “sort adaptively to Inv” but “pick the right specialist.”  
**First move:** reduction from residual-class discrimination to property testing / identity testing.

### P4 · Instance-optimal residual selection
**Object:** competitive ratio vs oracle that picks best residual after seeing X.  
**Claim shape:** T_hybrid ≤ c · T_oracle + o(n log n) for all X in class 𝒞.  
**Why new:** stronger than any single measure; related to instance-optimality (Afshani et al.) and self-improving algorithms.  
**First move:** characterize when residual costs are ordered by M_S alone.

### P5 · Synergistic word-RAM
**Object:** T(n, Inv, u, w).  
**Claim shape:** O(n + n·min(log(u), log(Inv/n+1), w)) or tighter.  
**Why new:** classical synergistic work is comparison-based; our menu is mixed-model.  
**First move:** upper bound matching hybrid table under word RAM; lower bound via element distinctness + inversion encoding.

### P6 · Local disorder concentration
**Object:** block-wise Inv or dense_inv as measure (local α-shuffle).  
**Claim shape:** when disorder is concentrated in o(n) blocks, cost O(n + k log k · block).  
**Why new:** mixed_blocks / consecutive-dom already gate on this; measure is under-theorized vs global Inv.  
**First move:** define BlockInv / DenseInv formally; relate to α-sorted subsequences (Sen).

---

## Detail — second tier (P7–P12)

| # | One-sentence seed |
|---|-------------------|
| P7 | Self-improving (Ailon–Chazelle et al.): train on 𝒟, then sort in O(n + H_𝒟(π)); residual router as the learned partition. |
| P8 | Sample complexity of learning f from (X, best_residual) pairs; excess risk → misrouting rate. |
| P9 | External-memory / cache-oblivious: Runs-adaptive merges already exist; hybrid residual I/O bound open. |
| P10 | One-pass sketch of M_S; residual class in o(n) space before full sort. |
| P11 | Optimal adaptive probing: which metric next given current posterior over residual class. |
| P12 | Conditional lower bounds: when residual selection is as hard as sorting under SETH-style hypotheses. |

---

## Capture criteria (when to cultivate)

Promote a line from list → active path-(b) target when **all** hold:

1. Written candidate statement (upper + lower, or conjecture with kill test).  
2. Fits EXTERNAL-clean visible metrics / residual menu.  
3. Does not regress path-(a) hybrid measurements.  
4. Squad agrees the statement is non-trivial relative to Mannila / LRM / α literature.

Park a line when: blocked by model mismatch, or reduces to known theorem with no hybrid gain.

---

## Recommended first petuous strike

**P1 + P2 together** (one paper-shaped unit):

> *Sample-optimal residual selection under fixed-size disorder probes.*

- Define M_S.  
- Lower bound any residual chooser given M_S.  
- Upper bound hybrid (or ideal router) + misrouting regret → 0.  
- Classical Inv-optimality remains out of scope — stated honestly.

P3 and P6 are natural lemmas inside that unit.  
P5 is the parallel systems track if word-RAM is preferred over comparison.

---

## Non-claims

- This list is **not** a theorem.  
- Hybrid residual is **not** optimal under any of these measures until a proof lands.  
- Path-(a) empirical soft=0 remains independent evidence, not asymptotic optimality.

**THE BEASTIE BOYZ — cultivate, then claim.**
