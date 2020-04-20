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

    pub fn subtract(&self, sub_image: &Image) -> Result<Image, &str> {
        if self.image.dimensions() != sub_image.image.dimensions() {
            return Err("Image dimensions do not match!");
        }

        let mut output = Image {
            image: self.image.clone(),
            path: self.path.clone(),
        };

        for (x, y, pixel) in output.image.enumerate_pixels_mut() {
            let subt_pixel = sub_image.image.get_pixel(x, y);
            let r = pixel[0] as f64 - subt_pixel[0] as f64;
            let g = pixel[1] as f64 - subt_pixel[1] as f64;
            let b = pixel[2] as f64 - subt_pixel[2] as f64;

            let luminance = 0.30 * r + 0.59 * g + 0.11 * b;

            if luminance >= 0.0 {
                *pixel = Rgb([0, 0, luminance as u8]);
            } else {
                let luminance = (-luminance) as u8;
                *pixel = Rgb([luminance, 0, 0]);
            }
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
