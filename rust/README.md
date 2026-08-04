# PhotonicSort — Rust bindings

Safe Rust API over the verified **C11 PhotonicSort v1.3.1-c** core.

Brand contract (unchanged):

```
probe → structure early-exit → residual talent menu
```

## Crates

| Crate | Role |
|-------|------|
| [`photonic-sort-sys`](./photonic-sort-sys) | Low-level FFI (`extern "C"`) + `cc` build of `../c/photonic_sort.c` |
| [`photonic-sort`](./photonic-sort) | Safe idiomatic wrapper (`sort_i64`, `probe_i64`, `PathCode`) |

The C sources under `c/` remain the **single source of truth**. This workspace does not duplicate algorithm code.

## Build

Requires a C toolchain (gcc/clang or MSVC).

```bash
cd rust
cargo build --release
cargo test -p photonic-sort
```

The sys crate’s `build.rs` compiles `../c/photonic_sort.c` with `-O3 -std=c11`.

## Quick start

```rust
use photonic_sort::{sort_i64, PathCode};

let mut data = vec![7_i64, 2, 9, 1, 5, 3, 8, 4, 6, 0];
let path = sort_i64(&mut data).expect("alloc");
assert!(data.windows(2).all(|w| w[0] <= w[1]));
assert!(matches!(path, PathCode::Structure | PathCode::Residual | PathCode::Trivial));
```

## Feature flags

- **sys / `vendored` (default)**: compile bundled C from this repo’s `c/` directory.
- Future: `system` link against an installed `libphotonic_sort` (not yet implemented).

## Non-claims

Same as the C library: not optical hardware, not light-speed sorting, not a P=NP claim.
See the root [README](../README.md).

## Contributors

- **Heywood Geblomi** — design, adaptive hybrid architecture
- Grok (xAI) / Harper / Benjamin / Lucas — implementation collaboration

## License

MIT — same as the parent project.
