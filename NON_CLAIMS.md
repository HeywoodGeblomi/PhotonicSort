# Non-Claims — PhotonicSort

These statements apply to all public numbers and residual claims for this repository, including the Breakthrough Phase 2 freeze and the **pure residual freeze** (2026-08-10).

1. **Not a field-level breakthrough.**  
   Consistent outperformance of current SOTA sequential integer sorters (ska_sort-class, IPS4o sequential, advanced pdqsort variants, modern LSD/MSD radix) across a broad reproducible suite is **not claimed**. ska_sort remains stronger on pure high-entropy data (best pure HE residual ~1.02-1.09x isolated / ~1.11x full-menu vs ska). Residual floors remain on adversarial and sparse patterns versus a highly engineered pdqsort.

2. **Not a novel adaptive complexity theorem.**  
   No new asymptotic bound under realistic models is claimed.

3. **Residual floors are acknowledged (pure residual freeze, n=1e6).**  

   | Pattern | pure / pdqsort | Status |
   |---------|---------------:|--------|
   | adversarial_pivot | **~1.81x** | pure residual floor accepted |
   | db_pk_sparse | **~2.56x** | pure residual floor accepted |
   | equal_heavy | **~0.99x** | near parity |
   | HE vs ska | **~1.02-1.11x** | best pure HE to date; <=0.90x **not met** |

   Those gaps are residual-quality limits (ska-class write/permute microarchitecture; pure residual vs library pdqsort), not routing bugs. Pure residual work is **stopped** pending external leverage.

4. **Classical software only.**  
   PhotonicSort is a classical adaptive hybrid. It does **not** use photonic hardware, does **not** sort "at the speed of light," and does **not** claim retrocausal computation. Physics references in the README are design metaphor only.

5. **Worst case remains O(n log n).**  
   No P = NP claim. No information-theoretic miracle.

6. **EXTERNAL-clean residual path.**  
   Visible probe metrics only. No hidden irreversible learning state in the frozen residual menu.

7. **Reproducibility required.**  
   Independent reproduction on the published suite is required before any stronger language.

Honesty primary.  
THE BEASTIE BOYZ · 2026-08-10
