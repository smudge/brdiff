# brdiff

Subtract two images to create a new blue-red difference image.

Pixels will appear blue in places where the first image's _luminance_ was greater than the second,
and will appear red when the opposite holds true. If the subtracted pixels are close or equal in
value, the resulting pixel will approach black.

Such an image subtraction function may be useful when performing [multispectral image subtraction](https://mci.si.edu/multispectral-and-hyperspectral-imaging)
to help emphasize areas of an object containing a specific material. Unlike a normal differencing
function (resulting in a greyscale output), a blue-red difference will distinguish positive signals
(blue) from the inverse (red).

## Browser proof of concept

An experimental [WebAssembly browser interface](web/README.md) uses this Rust
library to subtract two local images without sending their contents to a server.
It includes a downloadable PNG, an invert option, and a service worker for
reopening the page offline after the first visit.

The GitHub Pages demo will be available at **https://smudge.github.io/brdiff/**
once Pages is enabled under the repository's Settings → Pages and a deployment
has succeeded. Build, deployment and offline usage instructions are in the
[browser README](web/README.md).

## Installing

First ensure that Rust is installed by following [these instructions](https://doc.rust-lang.org/book/ch01-01-installation.html).

Then, run the following command:

```
cargo install brdiff
```

#### Without Cargo/Rust:

You may also [grab the latest release from here](https://github.com/smudge/brdiff/releases/latest) run it manually.

(With this method, you will need to call the command via `./brdiff` instead of `brdiff`, unless you also add its location to your $PATH.)

## Usage

To generate a blue-red difference image from two input images, enter this command,
substituting the three arguments with file names:

```
$ ./brdiff --help
usage:
  brdiff [--help] [--invert] <base_image> <subtraction_image> <difference_image>
```

Supported input and output formats include BMP, PPM, JPG/JPEG, and TIF/TIFF.

Here is an example command using `.tif` files:

```
./brdiff image_735nm.tif image_660nm.tif image_br.tif
```

To output an image with a white background (instead of black), use the `--invert` argument:

```
./brdiff --invert image_735nm.jpg image_660nm.jpg image_br_whitebg.jpg
```

The output image **will be overwritten** if it already exists, so **be careful, and keep backups**.

## Contributing

* Check the issue tracker and consider creating a new issue.
* Fork the project and create a new branch for your contribution.
* Write, commit, and push your contribution to your branch.
* Make sure the project builds (`make`) and functionality still works as expected.
* Submit a pull request.
