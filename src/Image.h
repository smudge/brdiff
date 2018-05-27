typedef enum {
  IMAGE_RED_CHANNEL,
  IMAGE_GREEN_CHANNEL,
  IMAGE_BLUE_CHANNEL,
  IMAGE_ALPHA_CHANNEL,
  IMAGE_NUM_CHANNELS
} ImageChannel;

typedef enum {
  IMAGE_POINT_SAMPLING,
  IMAGE_BILINEAR_SAMPLING,
  IMAGE_GAUSSIAN_SAMPLING,
  IMAGE_NUM_SAMPLING_METHODS
} ImageSamplingMethod;

typedef enum {
  IMAGE_OVER_COMPOSITION,
  IMAGE_IN_COMPOSITION,
  IMAGE_OUT_COMPOSITION,
  IMAGE_ATOP_COMPOSITION,
  IMAGE_XOR_COMPOSITION
} ImageCompositeOperation;

const double PI = 3.141592;
const double E  = 2.71828183;

class Image {
public:

  // Constructors/destructor
  Image(void);
  Image(const char *filename);
  Image(int width,
          int height);
  Image(int            width,
          int            height,
          const Pixel *pixels);
  Image(const Image& image);
  ~Image(void);

  // Image properties
  int             NPixels(void) const;
  int             Width(void) const;
  int             Height(void) const;

  // Pixel access/update
  Pixel       & GetPixel(int x, int y);
  Pixel       * Pixels(void);
  Pixel       * Pixels(int row);
  Pixel       * operator[](int row);
  const Pixel * operator[](int row) const;
  void            SetPixel(int            x,
                           int            y,
                           const Pixel& pixel);

  // Image processing
  Image& operator=(const Image& image);

  void     BlackAndWhite();
  void Subtract(const Image& image);

  // File reading/writing
  int Read(const char *filename);
  int ReadBMP(const char *filename);
  int ReadPPM(const char *filename);
  int ReadJPEG(const char *filename);
  int Write(const char *filename) const;
  int WriteBMP(const char *filename) const;
  int WritePPM(const char *filename,
               int         ascii = 0) const;
  int WriteJPEG(const char *filename) const;

private:

  Pixel *pixels;
  int npixels;
  int width;
  int height;
};


// Inline functions

inline int Image::
NPixels(void) const
{
  // Return total number of pixels
  return npixels;
}

inline int Image::
Width(void) const
{
  // Return width
  return width;
}

inline int Image::
Height(void) const
{
  // Return height
  return height;
}

inline Pixel& Image::
GetPixel(int x, int y)
{
  // Return pixel value at (x,y)
  // (pixels start at lower-left and go in row-major order)
  return pixels[x * height + y];
}

inline Pixel * Image::
Pixels(void)
{
  // Return pointer to pixels for whole image
  // (pixels start at lower-left and go in row-major order)
  return pixels;
}

inline Pixel * Image::
Pixels(int x)
{
  // Return pixels pointer for row at x
  // (pixels start at lower-left and go in row-major order)
  return &pixels[x * height];
}

inline Pixel * Image::
operator[](int x)
{
  // Return pixels pointer for row at x
  return Pixels(x);
}

inline const Pixel * Image::
operator[](int x) const
{
  // Return pixels pointer for row at x
  // (pixels start at lower-left and go in row-major order)
  return &pixels[x * height];
}

inline void Image::
SetPixel(int x, int y, const Pixel& pixel)
{
  // Set pixel
  pixels[x * height + y] = pixel;
}
