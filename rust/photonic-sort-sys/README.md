# photonic-sort-sys

Low-level FFI bindings to **PhotonicSort C11 v1.3.1-c**.

Prefer the safe [`photonic-sort`](https://crates.io/crates/photonic-sort) crate for application code.

## Build

Compiles the vendored sources under `vendor/` with the `cc` crate (`-std=c11`).
When used inside the PhotonicSort monorepo and `vendor/` is absent, falls back to
`../../c`.

Requires a C11 toolchain.

## License

MIT — Primary contributor: **Heywood Geblomi**
