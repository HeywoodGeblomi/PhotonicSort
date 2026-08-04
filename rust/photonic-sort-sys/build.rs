// PhotonicSort sys build
// Prefer vendored C11 sources (crates.io / offline), fall back to monorepo ../../c.

use std::env;
use std::path::PathBuf;

fn main() {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());

    let vendor_dir = manifest_dir.join("vendor");
    let vendor_src = vendor_dir.join("photonic_sort.c");
    let vendor_hdr = vendor_dir.join("photonic_sort.h");

    let mono_dir = manifest_dir.join("../../c");
    let mono_src = mono_dir.join("photonic_sort.c");
    let mono_hdr = mono_dir.join("photonic_sort.h");

    let (c_dir, c_src, c_hdr) = if vendor_src.is_file() {
        (vendor_dir, vendor_src, vendor_hdr)
    } else if mono_src.is_file() {
        (mono_dir, mono_src, mono_hdr)
    } else {
        panic!(
            "photonic-sort-sys: missing C sources.\n\
             Expected vendor/photonic_sort.c (crates.io layout) or ../../c/photonic_sort.c (monorepo)."
        );
    };

    println!("cargo:rerun-if-changed={}", c_src.display());
    println!("cargo:rerun-if-changed={}", c_hdr.display());
    println!("cargo:rerun-if-changed=build.rs");
    println!("cargo:include={}", c_dir.display());

    let mut build = cc::Build::new();
    build
        .file(&c_src)
        .include(&c_dir)
        .std("c11")
        .warnings(false);

    let profile = env::var("PROFILE").unwrap_or_else(|_| "debug".into());
    if profile == "release" {
        build.opt_level(3);
    }

    build.compile("photonic_sort");
}
