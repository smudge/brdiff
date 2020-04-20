# brdiff

Subtract two images to create a new blue-red difference image.

Pixels will appear blue in places where the first image's _luminance_ was greater than the second,
and will appear red when the opposite holds true. If the subtracted pixels are close or equal in
value, the resulting pixel will approach black.

Such an image subtraction function may be useful when performing [multispectral image subtraction](https://www.si.edu/MCIImagingStudio/Multispectral)
to help emphasize areas of an object containing a specific material. Unlike a normal differencing
function (resulting in a greyscale output), a blue-red difference will distinguish positive signals
(blue) from the inverse (red).

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
  brdiff <base_image> <subtracted_image> <difference_image>
```

Supported input and output formats include BMP, PPM, JPG/JPEG, and TIF/TIFF.

Here is an example command using `.tif` files:

```
./brdiff image_735nm.tif image_660nm.tif image_br.tif
```

You may even mix and match different file types (an experimental feature!):

```
./brdiff image_735nm.jpg image_660nm.bmp image_br.tif
```

The output image **will be overwritten** if it already exists, so be careful, and keep backups.

## Contributing

* Check the issue tracker and consider creating a new issue.
* Fork the project and create a new branch for your contribution.
* Write, commit, and push your contribution to your branch.
* Make sure the project builds (`make`) and functionality still works as expected.
* Submit a pull request.
