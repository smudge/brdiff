class Image {
public:

  Image(void);
  Image(const char *filename);
  Image(int width, int height);
  Image(int width, int height, const Pixel *pixels);
  Image(const Image& image);
  ~Image(void);

  int NPixels(void) const;
  int Width(void) const;
  int Height(void) const;

  Pixel       & GetPixel(int x, int y);
  Pixel       * Pixels(void);
  Pixel       * Pixels(int row);
  Pixel       * operator[](int row);
  const Pixel * operator[](int row) const;
  void          SetPixel(int x, int y, const Pixel& pixel);

  Image& operator=(const Image& image);

  void BlackAndWhite();
  void Subtract(const Image& image);

  int Read(const char *filename);
  int ReadBMP(const char *filename);
  int ReadPPM(const char *filename);
  int ReadJPEG(const char *filename);
  int Write(const char *filename) const;
  int WriteBMP(const char *filename) const;
  int WritePPM(const char *filename, int ascii = 0) const;
  int WriteJPEG(const char *filename) const;

private:

  Pixel *pixels;
  int npixels;
  int width;
  int height;
};
