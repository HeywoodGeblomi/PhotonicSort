# External shot — PhotonicSort Field-Level dual residual

No hype. Receipts + invitation.

## One-command money-shot

```bash
git clone https://github.com/HeywoodGeblomi/PhotonicSort.git && cd PhotonicSort
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
```

**Windows:** clone to a short path (e.g. `C:\ps`) or enable long paths so deep trees do not hit `MAX_PATH`:

```powershell
git -c core.longpaths=true clone https://github.com/HeywoodGeblomi/PhotonicSort.git C:\ps
cd C:\ps
docker build -f reproduce/Dockerfile.sp -t photonic-sp .
docker run --rm photonic-sp
```

Expected last line:

```text
SP MONEY-SHOT GREEN charged soft=0 major=0 ok=1
```

Formal R=11 CI:

```bash
docker run --rm -e STAT_SIG=1 photonic-sp
```

## What is being claimed

On a fixed charged surface (17 patterns, n=1e6, both x86_64 and aarch64):

- charged soft@1.20 vs **pdq** = **0**
- charged major (>1.5×) = **0**
- ok = 1

Dual residual / residual talent (Secondary Parity σ_Δ + classical probe agreement) is the **default** production path. Classical residual is behind `-DCLASSICAL_RESIDUAL` only.

Full claim package: [`docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md`](./docs/field_level/FIELD_LEVEL_CLAIM_v0.5.md)

## What is not being claimed

- Not a universal replacement for pdq / ska / Timsort / std::sort outside the charged surface.
- Not a complexity theorem.
- Not asymptotic novelty.
- Packaging does not expand claims.

EXTERNAL-clean (visible metrics only). No χ.

## Use it

```bash
# C++ (DEFAULT dual residual)
mkdir -p baselines
curl -fsSL -o baselines/pdqsort.h https://raw.githubusercontent.com/orlp/pdqsort/master/pdqsort.h
curl -fsSL -o baselines/ska_sort.hpp https://raw.githubusercontent.com/skarupke/ska_sort/master/ska_sort.hpp
cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME/.local && cmake --build build && cmake --install build
```

```cpp
#include <PhotonicSort/hybrid_residual_menu.hpp>
hybrid_residual::sort_i64(a, n);   // talent path is default
```

## Break this or use it

If the money-shot fails on your machine, open an issue with the full log.  
If you can expand the charged surface or improve high-entropy talent hit rate while staying EXTERNAL-clean, PRs are welcome (see open issues labeled `good first residual talent` / `soft-gate expansion`).

THE BEASTIE BOYZ / Blam · 2026-08-15
