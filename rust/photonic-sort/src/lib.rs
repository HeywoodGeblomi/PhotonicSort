//! Safe Rust API for PhotonicSort (C11 adaptive hybrid sort).
//!
//! Brand contract: **probe → structure early-exit → residual talent menu**.
//!
//! The algorithm core lives in the verified C11 sources (`c/photonic_sort.c`).
//! This crate is a thin, safe wrapper — not a pure-Rust reimplementation.
//!
//! # Example
//!
//! ```
//! use photonic_sort::{sort_i64, is_sorted_i64, version};
//!
//! let mut v = vec![5_i64, 1, 4, 2, 3];
//! let path = sort_i64(&mut v).expect("sort");
//! assert!(is_sorted_i64(&v));
//! assert!(!version().is_empty());
//! let _ = path;
//! ```

use photonic_sort_sys as sys;

/// Path taken by the adaptive sort.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum PathCode {
    /// `n <= 1` — nothing to do.
    Trivial,
    /// Fully sorted or fully reverse (O(n) early-exit).
    Structure,
    /// Residual talent menu (counting / insertion / run-merge / radix / pdq).
    Residual,
}

impl PathCode {
    fn from_c(code: i32) -> Result<Self, SortError> {
        match code {
            0 => Ok(PathCode::Trivial),
            1 => Ok(PathCode::Structure),
            2 => Ok(PathCode::Residual),
            -1 => Err(SortError::AllocFailed),
            other => Err(SortError::UnknownPath(other)),
        }
    }
}

/// GyroRank residual route selected by the probe.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum Route {
    Structure,
    Patterned,
    Random,
    LowCard,
    LowDisorder,
    Unknown(i32),
}

impl Route {
    fn from_c(code: i32) -> Self {
        match code {
            sys::PHOTONIC_ROUTE_STRUCTURE => Route::Structure,
            sys::PHOTONIC_ROUTE_PATTERNED => Route::Patterned,
            sys::PHOTONIC_ROUTE_RANDOM => Route::Random,
            sys::PHOTONIC_ROUTE_LOW_CARD => Route::LowCard,
            sys::PHOTONIC_ROUTE_LOW_DISORDER => Route::LowDisorder,
            other => Route::Unknown(other),
        }
    }
}

/// Probe snapshot (mirrors `photonic_probe_t`).
#[derive(Debug, Clone, Copy)]
pub struct Probe {
    pub n: usize,
    pub inv_ratio: f64,
    pub max_run: usize,
    pub run_count: usize,
    pub direction_changes: usize,
    pub equal_count: usize,
    pub confidence: f64,
    pub group_delay_proxy: f64,
    pub sortedness: f64,
    pub is_negative_delay: bool,
    pub monotone_sign: i32,
    pub route: Route,
    pub pilot_aborted: bool,
    pub pilot_samples: usize,
    pub unique_est: usize,
    pub sample_min: i64,
    pub sample_max: i64,
}

impl From<sys::photonic_probe_t> for Probe {
    fn from(p: sys::photonic_probe_t) -> Self {
        Self {
            n: p.n,
            inv_ratio: p.inv_ratio,
            max_run: p.max_run,
            run_count: p.run_count,
            direction_changes: p.direction_changes,
            equal_count: p.equal_count,
            confidence: p.confidence,
            group_delay_proxy: p.group_delay_proxy,
            sortedness: p.sortedness,
            is_negative_delay: p.is_negative_delay != 0,
            monotone_sign: p.monotone_sign,
            route: Route::from_c(p.route),
            pilot_aborted: p.pilot_aborted != 0,
            pilot_samples: p.pilot_samples,
            unique_est: p.unique_est,
            sample_min: p.sample_min,
            sample_max: p.sample_max,
        }
    }
}

/// Errors from the C core.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SortError {
    /// Temporary buffer allocation failed inside a residual path.
    AllocFailed,
    /// Unexpected path code from C (should not happen).
    UnknownPath(i32),
}

impl std::fmt::Display for SortError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            SortError::AllocFailed => write!(f, "photonic_sort: allocation failed"),
            SortError::UnknownPath(c) => write!(f, "photonic_sort: unknown path code {c}"),
        }
    }
}

impl std::error::Error for SortError {}

/// In-place PhotonicSort for `i64`.
///
/// Returns the adaptive path taken, or [`SortError::AllocFailed`] if a residual
/// buffer could not be allocated (rare; falls back paths usually avoid this).
pub fn sort_i64(a: &mut [i64]) -> Result<PathCode, SortError> {
    let n = a.len();
    // SAFETY: pointer is valid for `n` contiguous i64 elements; C sorts in place.
    let code = unsafe { sys::photonic_sort_i64(a.as_mut_ptr(), n) };
    PathCode::from_c(code)
}

/// Stable-ish residual via force_collapse (mergesort residual).
pub fn sort_i64_force_collapse(a: &mut [i64]) -> Result<PathCode, SortError> {
    let n = a.len();
    let code = unsafe { sys::photonic_sort_i64_force_collapse(a.as_mut_ptr(), n) };
    PathCode::from_c(code)
}

/// Copy `src` into `dst` then sort `dst` (lengths must match).
pub fn sort_i64_copy(src: &[i64], dst: &mut [i64]) -> Result<PathCode, SortError> {
    assert_eq!(src.len(), dst.len(), "src/dst length mismatch");
    let n = src.len();
    let code = unsafe {
        sys::photonic_sort_i64_copy(src.as_ptr(), dst.as_mut_ptr(), n)
    };
    PathCode::from_c(code)
}

/// Run the GyroRank-style probe without sorting.
pub fn probe_i64(a: &[i64]) -> Probe {
    let mut out = std::mem::MaybeUninit::<sys::photonic_probe_t>::uninit();
    unsafe {
        sys::photonic_probe_i64(a.as_ptr(), a.len(), out.as_mut_ptr());
        Probe::from(out.assume_init())
    }
}

/// `true` if non-decreasing.
pub fn is_sorted_i64(a: &[i64]) -> bool {
    unsafe { sys::photonic_is_sorted_i64(a.as_ptr(), a.len()) != 0 }
}

/// C library version string (e.g. `"1.3.1-c"`).
pub fn version() -> &'static str {
    unsafe {
        let p = sys::photonic_sort_version();
        if p.is_null() {
            return "";
        }
        std::ffi::CStr::from_ptr(p)
            .to_str()
            .unwrap_or("")
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn version_nonempty() {
        let v = version();
        assert!(v.contains('1'), "version={v}");
    }

    #[test]
    fn sort_random_small() {
        let mut v = vec![7_i64, 2, 9, 1, 5, 3, 8, 4, 6, 0];
        let path = sort_i64(&mut v).unwrap();
        assert!(is_sorted_i64(&v));
        assert_ne!(path, PathCode::Trivial);
    }

    #[test]
    fn sort_already_sorted() {
        let mut v: Vec<i64> = (0..100).collect();
        let path = sort_i64(&mut v).unwrap();
        assert!(is_sorted_i64(&v));
        assert!(matches!(path, PathCode::Structure | PathCode::Trivial));
    }

    #[test]
    fn sort_reverse() {
        let mut v: Vec<i64> = (0..100).rev().collect();
        let path = sort_i64(&mut v).unwrap();
        assert!(is_sorted_i64(&v));
        assert_eq!(path, PathCode::Structure);
    }

    #[test]
    fn probe_empty() {
        let p = probe_i64(&[]);
        assert_eq!(p.n, 0);
        assert_eq!(p.route, Route::Structure);
    }

    #[test]
    fn few_unique() {
        let mut v: Vec<i64> = (0..10_000).map(|i| i % 7).collect();
        sort_i64(&mut v).unwrap();
        assert!(is_sorted_i64(&v));
    }
}
