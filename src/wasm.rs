use crate::Image;
use wasm_bindgen::prelude::*;

/// Decode two images in Rust, subtract their luminance, then encode a PNG.
/// wasm-bindgen copies JS Uint8Array inputs into wasm memory and returns a
/// Uint8Array containing the PNG. No filesystem or network access is used.
#[wasm_bindgen]
pub fn subtract_png(base_bytes: &[u8], subtraction_bytes: &[u8], invert: bool) -> Result<Vec<u8>, JsValue> {
    let base = Image::from_bytes(base_bytes)
        .map_err(|error| JsValue::from_str(&format!("Unable to decode base image: {}", error)))?;
    let subtraction = Image::from_bytes(subtraction_bytes)
        .map_err(|error| JsValue::from_str(&format!("Unable to decode subtraction image: {}", error)))?;
    let result = base.subtract(&subtraction, invert)
        .map_err(JsValue::from_str)?;

    result
        .to_png_bytes()
        .map_err(|error| JsValue::from_str(&format!("Unable to encode PNG: {}", error)))
}
