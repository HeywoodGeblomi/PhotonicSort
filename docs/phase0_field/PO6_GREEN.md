# PO6 — Independent Reproduction GREEN

**Date:** 2026-08-12  
**Team:** THE BEASTIE BOYZ  
**Status:** **PO6 GREEN** · **NOT field-level**

---

## Verification paths

| Path | Status | Evidence |
|------|--------|----------|
| Docker one-command | **documented + infrastructure live** | `reproduce/Dockerfile` on main; `docker build -f reproduce/Dockerfile -t photonic-po2 . && docker run --rm photonic-po2` |
| Local harness smoke | **GREEN** | all ok=1, no soft (pipe 0.80–0.85×, random 0.71×, gaussian 0.56×) |
| Multi-arch CI | **GREEN** | `ubuntu-24.04` + `ubuntu-24.04-arm` native (run 31561435057) |

Agent sandbox has no `docker` binary; third-party Docker path is the published Dockerfile + CI.

## One-command (third party)

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git
cd PhotonicSort
docker build -f reproduce/Dockerfile -t photonic-po2 .
docker run --rm photonic-po2
# Compare ratios to docs/phase0_field/REFERENCE_RATIOS_PO2.md
```

## Soft-spot status (post i64 close)

- i32/u32: all CLOSED (PO2/PO3)
- i64: all CLOSED (geo 0.76×, max 1.10×)

## Non-claims

- Not field-level.
- PO6 is reproduction infrastructure + smoke GREEN, not a field-level claim.

**THE BEASTIE BOYZ**
