extern crate image;

use image::{DynamicImage, ImageBuffer, ImageOutputFormat, Rgb};
use std::io::Cursor;

#[cfg(target_arch = "wasm32")]
mod wasm;

pub struct Image {
    path: String,
    image: ImageBuffer<Rgb<u8>, Vec<u8>>,
}

impl Image {
    pub fn from_path(path: String) -> Result<Image, image::ImageError> {
        let image = image::open(path.clone())?.into_rgb();

        Ok(Image { path, image })
    }

    // Unlike from_path, this can be called in wasm32-unknown-unknown,
    // where the browser supplies the file bytes instead of a filesystem path.
    pub fn from_bytes(bytes: &[u8]) -> Result<Image, image::ImageError> {
        let image = image::load_from_memory(bytes)?.into_rgb();

        Ok(Image {
            path: "browser input".to_owned(),
            image,
        })
    }

    // Return an encoded image without writing to the filesystem.
    pub fn to_png_bytes(&self) -> Result<Vec<u8>, image::ImageError> {
        let mut output = Cursor::new(Vec::new());
        DynamicImage::ImageRgb8(self.image.clone())
            .write_to(&mut output, ImageOutputFormat::Png)?;
        Ok(output.into_inner())
    }

    pub fn to_tiff_bytes(&self) -> Result<Vec<u8>, image::ImageError> {
        let mut output = Cursor::new(Vec::new());
        image::codecs::tiff::TiffEncoder::new(&mut output)
            .encode(self.image.as_raw(), self.image.width(), self.image.height(), image::ColorType::Rgb8)?;
        Ok(output.into_inner())
    }

    pub fn subtract(&self, sub_image: &Image, invert: bool) -> Result<Image, &str> {
        if self.image.dimensions() != sub_image.image.dimensions() {
            return Err("Image dimensions do not match!");
        }

        let mut warn_about_channels = false;
        let mut output = Image {
            image: self.image.clone(),
            path: self.path.clone(),
        };

        for (x, y, pixel) in output.image.enumerate_pixels_mut() {
            if pixel[0] != pixel[1] && pixel[1] != pixel[2] {
                warn_about_channels = true;
            }
            let subt_pixel = sub_image.image.get_pixel(x, y);
            let r = pixel[0] as f64 - subt_pixel[0] as f64;
            let g = pixel[1] as f64 - subt_pixel[1] as f64;
            let b = pixel[2] as f64 - subt_pixel[2] as f64;

            let luminance = 0.30 * r + 0.59 * g + 0.11 * b;

            if luminance < 0.0 {
                let mut val = -luminance as u8;
                if invert {
                    val = 255 - val;
                    *pixel = Rgb([255, val, val]);
                } else {
                    *pixel = Rgb([val, 0, 0]);
                }
            } else {
                let mut val = luminance as u8;
                if invert {
                    val = 255 - val;
                    *pixel = Rgb([val, val, 255]);
                } else {
                    *pixel = Rgb([0, 0, val]);
                }
            }
        }

        // stderr is not available in a plain browser wasm build.
        #[cfg(not(target_arch = "wasm32"))]
        if warn_about_channels {
            eprintln!("WARNING: Image has pixel values that are not the same across RGB color channels!\n{}", self.path);
        }
        #[cfg(target_arch = "wasm32")]
        let _ = warn_about_channels;

        Ok(output)
    }

    pub fn set_path(&mut self, path: String) {
        self.path = path;
    }

    pub fn save(&self) -> image::ImageResult<()> {
        self.image.save(self.path.clone())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn solid(rgb: [u8; 3], width: u32, height: u32) -> Image {
        Image {
            path: "test".to_owned(),
            image: ImageBuffer::from_pixel(width, height, Rgb(rgb)),
        }
    }

    #[test]
    fn positive_difference_is_blue_and_negative_is_red() {
        let bright = solid([100, 100, 100], 1, 1);
        let dark = solid([0, 0, 0], 1, 1);

        assert_eq!(*bright.subtract(&dark, false).unwrap().image.get_pixel(0, 0), Rgb([0, 0, 100]));
        assert_eq!(*dark.subtract(&bright, false).unwrap().image.get_pixel(0, 0), Rgb([100, 0, 0]));
    }

    #[test]
    fn inverted_background_is_white() {
        let bright = solid([100, 100, 100], 1, 1);
        let dark = solid([0, 0, 0], 1, 1);

        assert_eq!(*bright.subtract(&dark, true).unwrap().image.get_pixel(0, 0), Rgb([155, 155, 255]));
        assert_eq!(*dark.subtract(&dark, true).unwrap().image.get_pixel(0, 0), Rgb([255, 255, 255]));
    }

    #[test]
    fn mismatched_dimensions_are_rejected() {
        assert!(solid([0, 0, 0], 1, 1).subtract(&solid([0, 0, 0], 2, 1), false).is_err());
    }

    #[test]
    fn memory_input_and_png_output_round_trip() {
        let original = solid([25, 50, 75], 2, 1);
        let encoded = original.to_png_bytes().unwrap();
        let decoded = Image::from_bytes(&encoded).unwrap();
        assert_eq!(decoded.image.dimensions(), (2, 1));
        assert_eq!(*decoded.image.get_pixel(1, 0), Rgb([25, 50, 75]));
    }

    #[test]
    fn memory_tiff_output_round_trip() {
        let original = solid([25, 50, 75], 2, 1);
        let encoded = original.to_tiff_bytes().unwrap();
        let decoded = Image::from_bytes(&encoded).unwrap();
        assert_eq!(decoded.image.dimensions(), (2, 1));
        assert_eq!(*decoded.image.get_pixel(1, 0), Rgb([25, 50, 75]));
    }
}
