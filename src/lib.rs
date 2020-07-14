extern crate image;

use image::{ImageBuffer, Rgb};

pub struct Image {
    path: String,
    image: ImageBuffer<Rgb<u8>, Vec<u8>>,
}

impl Image {
    pub fn from_path(path: String) -> Result<Image, image::ImageError> {
        let image = image::open(path.clone())?.into_rgb();

        Ok(Image { path, image })
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

        if warn_about_channels {
            eprintln!("WARNING: Image has pixel values that are not the same across RGB color channels!\n{}", self.path);
        }
        Ok(output)
    }

    pub fn set_path(&mut self, path: String) {
        self.path = path;
    }

    pub fn save(&self) -> image::ImageResult<()> {
        self.image.save(self.path.clone())
    }
}
