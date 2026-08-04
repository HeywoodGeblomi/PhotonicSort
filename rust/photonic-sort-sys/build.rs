// PhotonicSort sys build — compile vendored C11 core from ../../c
// Single source of truth: do not duplicate algorithm sources here.

use std::env;
use std::path::PathBuf;

fn main() {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    // rust/photonic-sort-sys → repo root c/
    let c_dir = manifest_dir.join("../../c");
    let c_src = c_dir.join("photonic_sort.c");
    let c_hdr = c_dir.join("photonic_sort.h");

    if !c_src.is_file() {
        panic!(
            "photonic-sort-sys: missing {} — expected repo c/photonic_sort.c",
            c_src.display()
        );
    }

    println!("cargo:rerun-if-changed={}", c_src.display());
    println!("cargo:rerun-if-changed={}", c_hdr.display());
    println!("cargo:rerun-if-changed=build.rs");

    let mut build = cc::Build::new();
    build
        .file(&c_src)
        .include(&c_dir)
        .std("c11")
        .warnings(false);

    // Release-class flags when cargo is in release / optimized profile.
    let profile = env::var("PROFILE").unwrap_or_else(|_| "debug".into());
    if profile == "release" {
        build.opt_level(3);
        // Keep portable by default; consumers can set CFLAGS / RUSTFLAGS for native.
        // build.flag_if_supported("-march=native");
    }

    // MSVC vs gcc/clang: cc crate maps std(c11) appropriately.
    build.compile("photonic_sort");
}
