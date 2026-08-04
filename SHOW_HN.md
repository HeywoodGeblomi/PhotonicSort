# Hacker News submission package

Submit at: https://news.ycombinator.com/submit  
Type: **Show HN**

## Title (copy exactly)

```text
Show HN: PhotonicSort – adaptive hybrid sort in C11 with structure early-exits
```

## URL

```text
https://heywoodgeblomi.github.io/PhotonicSort/
```

Fallback:

```text
https://github.com/HeywoodGeblomi/PhotonicSort
```

## First comment (post immediately after submitting)

```text
PhotonicSort is a classical adaptive sort: one O(n) disorder probe, then either a structure early-exit (sorted/reverse/long runs) or a stable residual mergesort.

- Production path: C11, libc only (`photonic_sort_i64` in-place)
- Python: pure stdlib reference (not a CPython C-extension; no GIL involvement)
- Worst case O(n log n); no P=NP claim; name is metaphor only

On n=1e6 int64, -O3, best of 5 vs libstdc++ std::sort:
  sorted   ~0.019 ms vs ~15 ms
  reverse  ~0.44 ms vs ~9 ms
  random   ~105 ms vs ~78 ms (std::sort wins on unstructured data, as expected)

Repo: https://github.com/HeywoodGeblomi/PhotonicSort
Benchmarks: https://github.com/HeywoodGeblomi/PhotonicSort/blob/main/BENCHMARKS.md
Build: cd c && make && make test

Happy to answer questions about the probe heuristics or the residual path.
```

## Notes

- Weekday morning US time tends to get more eyes; avoid hype words.
- Respond to technical comments; correct username digs once and move on.
