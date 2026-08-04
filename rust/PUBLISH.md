# Publishing PhotonicSort Rust crates to crates.io

The workspace is packaged for crates.io with a **vendored** C11 core
(`photonic-sort-sys/vendor/`). No monorepo checkout is required for downstream
consumers after publish.

## Prerequisites

1. crates.io account + API token (`cargo login`).
2. C11 toolchain on the publish machine (gcc/clang/MSVC) — needed only for the
   publish-host dry-run build, not for end users of the binary crates index.
3. Confirm crate names are free (or you own them):
   - https://crates.io/crates/photonic-sort-sys
   - https://crates.io/crates/photonic-sort

## Dry-run checklist

From the `rust/` directory:

```bash
# 1. Build + test with the vendored sources
cargo test -p photonic-sort --release

# 2. Package inspection (lists files that would be uploaded)
cargo package -p photonic-sort-sys --list
cargo package -p photonic-sort --list

# 3. Dry-run publish (builds the package, does not upload)
cargo publish -p photonic-sort-sys --dry-run
cargo publish -p photonic-sort --dry-run
```

Verify `cargo package --list` for `photonic-sort-sys` includes:

- `src/lib.rs`
- `build.rs`
- `vendor/photonic_sort.c`
- `vendor/photonic_sort.h`
- `LICENSE` / `README.md`

## Real publish order

Sys crate first (the safe crate depends on it by version):

```bash
cargo publish -p photonic-sort-sys
# wait until https://crates.io/crates/photonic-sort-sys shows 1.3.1
cargo publish -p photonic-sort
```

If the index lag is long:

```bash
cargo publish -p photonic-sort --dry-run   # re-check
# then
cargo publish -p photonic-sort
```

## Version policy

Rust crate version **1.3.1** mirrors C `PHOTONIC_SORT_VERSION_STRING` `"1.3.1-c"`.
Bump both when the residual menu changes.

## Monorepo vs vendor

| Context | C sources used |
|---------|----------------|
| Building inside this repo | `vendor/` if present, else `../../c` |
| crates.io / offline | always `vendor/` (shipped in the package) |

After changing `c/photonic_sort.{c,h}` on main, refresh the vendor copies before
the next crates.io release:

```bash
cp c/photonic_sort.c c/photonic_sort.h rust/photonic-sort-sys/vendor/
```

## Credit

Primary contributor: **Heywood Geblomi**
