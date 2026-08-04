# photonic-sort

Safe Rust API for **PhotonicSort** — adaptive hybrid sorting with a C11 performance core.

```rust
use photonic_sort::{sort_i64, PathCode};

let mut data = vec![3_i64, 1, 4, 1, 5, 9];
let path = sort_i64(&mut data).unwrap();
assert!(data.windows(2).all(|w| w[0] <= w[1]));
println!("path = {path:?}");
```

See the [workspace README](../README.md) and the C library docs in `../../c/`.

**Contributor credit:** Heywood Geblomi (design) · Grok / Harper / Benjamin / Lucas (implementation).
