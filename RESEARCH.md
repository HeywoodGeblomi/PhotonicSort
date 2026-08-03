# PhotonicSort — Research Mapping

## What the photon experiment actually showed

**Experiment (University of Toronto):** Daniela Angulo, Aephraim M. Steinberg, collaborators; theory with Howard M. Wiseman (Griffith).

**Claim (precise):** For photons transmitted through a cold rubidium cloud, the weak-value / post-selected measure of how long atoms spend in the excited state can be **negative**. The optical **group delay** near resonance can also be negative. Photons can appear, on average, to exit before they enter.

**What it is not:**

- Not time travel
- Not faster-than-light signaling
- Not a violation of special relativity or causality
- Not a usable clock that runs backward for communication

**Mechanism (standard QM):** Pulse reshaping, interference, and post-selection. Negative group delay has been known theoretically for a long time; the Toronto work established that an independent atomic-excitation probe reports the same negative figure, giving the “negative time” more physical weight than a mere reshaping illusion.

**Key references**

| Ref | Role |
|-----|------|
| arXiv:2409.03680 | Preprint: experimental evidence of negative excitation time |
| Physical Review Letters (2026) | Peer-reviewed follow-on of the same line |
| Scientific American / Physics World coverage | Popular summaries (treat carefully for precision) |

## Castagnoli retrocausality (inspirational only)

Giuseppe Castagnoli (arXiv:2505.08346 and earlier works) argues that a classical-logic description of quantum algorithms must admit a form of logical retrocausality: it is as if the solver already knew half the solution information, with the future measurement outcome acting as an attractor. This is a *philosophical / interpretive* account of quantum speedup — not a recipe for classical algorithms that beat P vs NP barriers.

PhotonicSort **borrows the design metaphor** (boundary condition as attractor; die at the answer) and **does not claim** Castagnoli’s logic applies as a classical complexity breakthrough.

## Mapping table (philosophy → code)

| Analogy fragment | Implementation |
|------------------|----------------|
| Photon enters haystack with less energy | `photonic_probe` — O(n) stratified / full scan, not full sort work |
| Negative dwell / exit before entry | `is_negative_delay` + `negative_time_early_exit` on structured data |
| Survivors that transmit early | Long monotonic runs / high sortedness path |
| Die at the objective answer | `photonic_collapse` via stable argsort ranks |
| Only consistent exit points | Rank placement is uniquely determined by sorted order |
| Give everything | Probe metrics fully committed (single-pass) |
| Pure monotone haystack | O(n) reverse/copy early exit via `monotone_sign` |
| Take nothing | Output is pure ordered state |
| Become photonic | Tagline for the ordered result |

## Complexity honesty

| Claim we make | Claim we refuse |
|---------------|-----------------|
| Always correct sort | Solves NP-complete search |
| Adaptive near-linear best case | Beats comparison sorting asymptotically |
| O(n log n) worst case | Proves P = NP |
| Stable on equal keys | Physically retrocausal computation |
| Metaphorically “retrocausal collapse” | Information travels backward in the machine |

## Integration notes (Geblomi / Implosion)

- Probe style deliberately echoes **Geblomi richer Gyro** metrics (`inv_ratio`, `max_run`, direction changes).
- Collapse is the purest “boundary attractor” embodiment; an in-place cycle-following placement would align with `residual_automaton` / CycleGuard spirit.
- Multi-D: apply probe + early-exit / collapse per fiber (rows, then columns) inside an Implosion-style disposition.

## Citation

If you use PhotonicSort in academic or commercial work, cite this repository and, for the physics analogy, the Angulo/Steinberg line (arXiv:2409.03680). Do not cite PhotonicSort as experimental physics or as a P=NP result.

## C implementation

`c/photonic_sort.c` is a fidelity port of the Python adaptive logic:

- Same probe thresholds and negative-delay decision
- O(n) monotone exits via `monotone_sign`
- Residual is **stable** bottom-up mergesort (matches Timsort stability class on equals)
- No claim of asymptotic superiority over comparison sorting
