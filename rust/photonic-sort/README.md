# photonic-sort

Safe Rust API for **PhotonicSort** — adaptive hybrid sort (C11 core v1.3.1-c).

```rust
use photonic_sort::{sort_i64, is_sorted_i64};

let mut v = vec![5_i64, 1, 4, 2, 3];
sort_i64(&mut v).expect("sort");
assert!(is_sorted_i64(&v));
```

Brand contract: **probe → structure early-exit → residual talent menu**.

See the [workspace README](../README.md) and [PUBLISH.md](../PUBLISH.md).

Primary contributor: **Heywood Geblomi** · License: MIT
