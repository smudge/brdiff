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

Grab the [latest release](https://github.com/smudge/brdiff/releases/latest). As of now, this is
built for *macOS only* and has no GUI. You'll want to make the binary executable by running this
command:

```
chmod +x brdiff
```

You may also choose to build from source (instructions below).

## Usage

To run the command, enter this command, substituting the three arguments with file names:

```
$ ./brdiff --help
usage:
  brdiff <base_image> <subtracted_image> <difference_image>
```

Supported input and output formats include BMP, PPM, JPG/JPEG, and TIF/TIFF.

Here is an example command using TIF files:

```
./brdiff image_735nm.tif image_660nm.tif image_br.tif
```

You may even mix and match different file types (this is an experimental feature):

```
./brdiff image_735nm.jpg image_660nm.bmp image_br.tif
```

The output image **will be overwritten** if it already exists, so be careful, and keep backups.

## Building Yourself

To build brdiff locally, simply run `make`. Ensure that you have `libjpeg` and `libtiff` installed
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
