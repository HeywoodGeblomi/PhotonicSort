# PhotonicSort — Rust bindings

Safe Rust API over the verified **C11 PhotonicSort v1.3.1-c** core.

Brand contract (unchanged):

```
probe → structure early-exit → residual talent menu
```

## Crates

| Crate | Role |
|-------|------|
| [`photonic-sort-sys`](./photonic-sort-sys) | Low-level FFI (`extern "C"`) + `cc` build of vendored C |
| [`photonic-sort`](./photonic-sort) | Safe idiomatic wrapper (`sort_i64`, `probe_i64`, `PathCode`) |

`photonic-sort-sys` ships a **vendored** copy of `c/photonic_sort.{c,h}` under
`vendor/` so the crate builds offline and is crates.io-ready. When building
inside this monorepo, `build.rs` falls back to `../../c` if the vendor tree is
absent.

## Build

Requires a C toolchain (gcc/clang or MSVC).

```bash
cd rust
cargo build --release
cargo test -p photonic-sort
```

## Quick start

```rust
use photonic_sort::{sort_i64, PathCode};

let mut data = vec![7_i64, 2, 9, 1, 5, 3, 8, 4, 6, 0];
let path = sort_i64(&mut data).expect("alloc");
assert!(data.windows(2).all(|w| w[0] <= w[1]));
assert!(matches!(path, PathCode::Structure | PathCode::Residual | PathCode::Trivial));
```

## crates.io

See [PUBLISH.md](./PUBLISH.md) for dry-run and publish order.

## Non-claims

Same as the C library: not optical hardware, not light-speed sorting, not a P=NP claim.
See the root [README](../README.md).

## Contributors

- **Heywood Geblomi** — design, adaptive hybrid architecture
- Grok (xAI) / Harper / Benjamin / Lucas — implementation collaboration

## License

MIT — same as the parent project.
