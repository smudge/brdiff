use brdiff::Image;
use std::{fs, process::Command};

fn jpeg_fixture() -> Vec<u8> {
    include_str!("fixtures/rgb8_jpeg.hex")
        .trim()
        .as_bytes()
        .chunks_exact(2)
        .map(|pair| {
            u8::from_str_radix(std::str::from_utf8(pair).expect("hex"), 16).expect("valid fixture")
        })
        .collect()
}

#[test]
fn jpeg_decodes_in_native_library_and_cli() {
    let jpg = jpeg_fixture();
    let base = Image::from_bytes(&jpg).expect("native library should decode JPEG bytes");
    base.subtract(&base, false).expect("JPEG image should subtract");

    let temp = std::env::temp_dir().join(format!("brdiff-jpeg-test-{}", std::process::id()));
    fs::create_dir_all(&temp).unwrap();
    let a = temp.join("a.jpg");
    let b = temp.join("b.jpg");
    let output = temp.join("out.png");
    fs::write(&a, &jpg).unwrap();
    fs::write(&b, &jpg).unwrap();

    let command = Command::new(env!("CARGO_BIN_EXE_brdiff"))
        .args([a.as_os_str(), b.as_os_str(), output.as_os_str()])
        .output()
        .expect("run native CLI");
    assert!(command.status.success(), "CLI failed: {:?}", String::from_utf8_lossy(&command.stderr));
    let decoded = image::open(&output).expect("CLI should create valid PNG");
    assert_eq!(decoded.dimensions(), (16, 16));
    fs::remove_dir_all(temp).unwrap();
}
