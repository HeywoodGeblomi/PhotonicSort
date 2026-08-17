# Containerized deployment — PhotonicSort

EXTERNAL-clean. THE BEASTIE BOYZ · 2026-08-17

---

## Quick deploy

```bash
# Default runtime (C11 tests + demo)
docker compose up --build runtime

# Fully static (scratch)
docker compose up --build runtime-static

# Field-Level hybrid residual money-shot
docker compose up --build money-shot
# expected: SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1
```

## Image targets

| Service / target | Base | Contents |
|------------------|------|----------|
| `runtime` | debian slim | dynamic C11 demo + tests + static lib |
| `runtime-static` | scratch | fully static binaries |
| `runtime-distroless` | distroless/static | static binaries, policy-friendly |
| `money-shot` | debian slim | hybrid residual suite + soft-gate |
| `lib-export` | scratch | `libphotonic_sort.a` + `photonic_sort.h` only |

## Extract static library

```bash
docker compose build lib-export
docker create --name ps-lib photonicsort:lib
docker cp ps-lib:/libphotonic_sort.a .
docker cp ps-lib:/include/photonic_sort.h .   # path may be /photonic_sort.h on lib-export
docker rm ps-lib
```

(Adjust `docker cp` paths to match `Dockerfile` `lib-export` stage layout.)

## Native host bench (not container)

```bash
cd c && make native-bench
```

Compares `photonic_sort_i64` vs libc `qsort` at n=1e6 (best-of-5). Ratio < 1 means PhotonicSort is faster.

## Non-claims

Container images do **not** expand Field-Level claims. Soft-gate authority remains the money-shot charged surface. See `NON_CLAIMS.md` and `residual/UNSCOPED_BOUNDARIES.md`.

**Windows:** clone to a short path or `git -c core.longpaths=true clone ...` before `docker compose`.
