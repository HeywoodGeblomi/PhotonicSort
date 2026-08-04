# PhotonicSort C 1.3.1-c

**Ship release** — Plan A residuals + cache-local insertion.

## Highlights

- **Plan A residual menu** wins the previous `std::sort` war paths:
  - `LOW_CARD` counting → sawtooth / few-unique (**~13–14×** vs `std::sort` @ n=1e6)
  - `LOW_DISORDER` insertion/pdq → almost-sorted (**~1.3×**)
  - GyroRank pilot still routes pure random → LSD radix (**~2.5×**)
- **Cache-local insertion**: early-continue on ordered pairs + single backward shift (no find-then-memmove double-touch).
- Brand contract unchanged: **probe → structure early-exit → residual menu**.

## Routes

| Code | Name | Residual |
|-----:|------|----------|
| 0 | STRUCTURE | O(n) exit / reverse |
| 1 | PATTERNED | Run-merge (≤32 runs) |
| 2 | RANDOM | Radix / introsort |
| 3 | LOW_CARD | Counting sort |
| 4 | LOW_DISORDER | Insertion (n≤4096) / pdq |

## Verify

```bash
cd c && make test
```

## Contributors

**Heywood Geblomi** · Grok (xAI) collaboration
