# rbdiff

Subtract two images to create a new red-blue difference image.

Pixels will appear blue in places where the first image's _luminance_ was greater than the second,
and will appear red when the opposite holds true. If the subtracted pixels are close or equal in
value, the resulting pixel will approach black.

Such an image subtraction function may be useful when performing [multispectral image subtraction](https://www.si.edu/MCIImagingStudio/Multispectral)
to help emphasize areas of an object containing a specific material. Unlike a normal differencing
function (resulting in a greyscale output), a red-blue difference will distinguish positive signals
(blue) from their inverse (red).

## Usage

```
usage: rbdiff [--help] <input_image> <subtraction_image> <output_image>
```

Supported input and output formats include BMP, PPM, JPG/JPEG, and TIF/TIFF:

```
./rbdiff image_1.jpg image_2.bmp output_image.tif
```

The output image **will be overwritten** if it already exists, so be careful, and keep backups.

## Building Yourself

To build rbdiff locally, simply run `make`. Ensure that you have `libjpeg` and `libtiff` installed
on your system.

### Building on OS X

On OS X, you can run the following command to install the Xcode Command Line Tools:

```
xcode-select --install
```

Then, if you have [homebrew](https://brew.sh/) installed, just run:

```
brew bundle
```

This should install `libjpeg` and `libtiff` for you.

## Contributing

* Check the issue tracker and consider creating a new issue.
* Fork the project and create a new branch for your contribution.
* Write, commit, and push your contribution to your branch.
* Make sure the project builds (`make`) and functionality still works as expected.
* Submit a pull request.
