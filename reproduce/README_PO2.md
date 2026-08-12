# PO2 — Docker one-command + reference ratios

**Goal:** third party runs one Docker command and gets ratios comparable to `docs/phase0_field/REFERENCE_RATIOS_PO2.md`.

```bash
docker build -f reproduce/Dockerfile -t photonic-po2 .
docker run --rm photonic-po2
```

Requires Docker. Network needed at **build** time only (fetches orlp pdqsort). Runtime is offline.

**Not field-level.** Compare ratios, not absolute ms.

**THE BEASTIE BOYZ**
