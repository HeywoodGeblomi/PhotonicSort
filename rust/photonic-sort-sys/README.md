# photonic-sort-sys

Low-level FFI to [PhotonicSort](https://github.com/HeywoodGeblomi/PhotonicSort) C11 v1.3.1-c.

- Builds `../../c/photonic_sort.c` via the `cc` crate (`vendored` feature, default).
- Hand-written `extern "C"` bindings matching `c/photonic_sort.h`.

Prefer the safe [`photonic-sort`](../photonic-sort) crate.
