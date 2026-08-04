//! Raw FFI bindings to PhotonicSort C11 (v1.3.1-c).
//!
//! Prefer the safe [`photonic-sort`](https://github.com/HeywoodGeblomi/PhotonicSort)
//! wrapper crate for application code.
//!
//! # Safety
//! All functions are `unsafe`. Pointers must be valid for `n` elements;
//! `photonic_sort_i64` mutates in place.

#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(clippy::all)]

use std::os::raw::{c_char, c_int, c_void};

/// GyroRank residual route codes (observe → gate).
pub const PHOTONIC_ROUTE_STRUCTURE: c_int = 0;
pub const PHOTONIC_ROUTE_PATTERNED: c_int = 1;
pub const PHOTONIC_ROUTE_RANDOM: c_int = 2;
pub const PHOTONIC_ROUTE_LOW_CARD: c_int = 3;
pub const PHOTONIC_ROUTE_LOW_DISORDER: c_int = 4;

pub const PHOTONIC_SORT_VERSION_MAJOR: u32 = 1;
pub const PHOTONIC_SORT_VERSION_MINOR: u32 = 3;
pub const PHOTONIC_SORT_VERSION_PATCH: u32 = 1;

/// Mirrors `photonic_probe_t` in `photonic_sort.h`.
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct photonic_probe_t {
    pub n: usize,
    pub inv_ratio: f64,
    pub max_run: usize,
    pub run_count: usize,
    pub direction_changes: usize,
    pub equal_count: usize,
    pub confidence: f64,
    pub group_delay_proxy: f64,
    pub sortedness: f64,
    pub is_negative_delay: c_int,
    pub monotone_sign: c_int,
    pub route: c_int,
    pub pilot_aborted: c_int,
    pub pilot_samples: usize,
    pub unique_est: usize,
    pub sample_min: i64,
    pub sample_max: i64,
}

/// Comparator type for the generic sort path.
pub type photonic_cmp_fn =
    Option<unsafe extern "C" fn(a: *const c_void, b: *const c_void) -> c_int>;

extern "C" {
    pub fn photonic_probe_i64(
        a: *const i64,
        n: usize,
        out: *mut photonic_probe_t,
    );

    /// In-place PhotonicSort for `int64_t`.
    /// Returns path code: 0=trivial, 1=structure, 2=residual, -1=alloc fail.
    pub fn photonic_sort_i64(a: *mut i64, n: usize) -> c_int;

    pub fn photonic_sort_i64_force_collapse(a: *mut i64, n: usize) -> c_int;

    pub fn photonic_sort_i64_copy(
        src: *const i64,
        dst: *mut i64,
        n: usize,
    ) -> c_int;

    pub fn photonic_probe_generic(
        base: *const c_void,
        n: usize,
        size: usize,
        cmp: photonic_cmp_fn,
        out: *mut photonic_probe_t,
    );

    pub fn photonic_sort(
        base: *mut c_void,
        n: usize,
        size: usize,
        cmp: photonic_cmp_fn,
    ) -> c_int;

    pub fn photonic_is_sorted_i64(a: *const i64, n: usize) -> c_int;

    pub fn photonic_sort_version() -> *const c_char;
}
