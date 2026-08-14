# Wave 0 — SP Field-Level Independent Reproduction

**Status:** SHIPPED 2026-08-14  
**Closes:** third-party / Wave 0 reproduce gate for Secondary Parity dual-evidence claim surface

## One-command

```bash
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
```

Formal CI packaging:

```bash
docker run --rm -e STAT_SIG=1 photonic-sp
```

Full protocol: [`../../reproduce/SP_FIELD_LEVEL.md`](../../reproduce/SP_FIELD_LEVEL.md)

## What a third party verifies

| Mode | Command surface | Success line |
|------|-----------------|--------------|
| Money-shot | R=3, summary CSV | `SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1` |
| Stat-sig | R=11, raw trials + bootstrap | `SP STAT-SIG GREEN charged CI-upper soft=0 major=0` |

Both ISAs already measured GREEN on GitHub Actions (`sp-multi-arch`, `sp-stat-sig`). This package lets an external host reproduce the same gates without Actions.

## Non-claims

Controlled suite evidence on the charged surface. Not SOTA displacement. EXTERNAL-clean.

**THE BEASTIE BOYZ / Blam**
