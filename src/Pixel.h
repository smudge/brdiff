class Pixel {
public:

  // Constructor functions
  Pixel(void);
  Pixel(const Pixel& pixel);
  Pixel(double r,
          double g,
          double b,
          double a);
  Pixel(const double rgba[4]);

  // Component access functions/operators
  double  Red(void) const;
  double  Green(void) const;
  double  Blue(void) const;
  double  Alpha(void) const;
  double  operator[](int i) const;
  double& operator[](int i);
  double  Component(int i) const;
  double* Components(void);

  // Property functions/operators
  double  Luminance(void) const;
  bool    IsBlack(void) const;
  bool    IsWhite(void) const;
  bool    operator==(const Pixel& pixel) const;
  bool    operator!=(const Pixel& pixel) const;

  // Manipulation functions/operations
  void    SetRed(double red);
  void    SetGreen(double green);
  void    SetBlue(double blue);
  void    SetAlpha(double alpha);
  void    Reset(double red,
                double green,
                double blue,
                double alpha);
  void           Clamp(double maximum_value = 1.0);

  // Assignment operators
  Pixel      & operator=(const Pixel& pixel);
  Pixel      & operator+=(const Pixel& pixel);
  Pixel      & operator-=(const Pixel& pixel);
  Pixel      & operator*=(const Pixel& pixel);
  Pixel      & operator*=(double scale);
  Pixel      & operator/=(double scale);

  // Arithmetic operations
  friend Pixel operator+(const Pixel& pixel1,
                           const Pixel& pixel2);
  friend Pixel operator-(const Pixel& pixel1,
                           const Pixel& pixel2);
  friend Pixel operator*(const Pixel& pixel1,
                           const Pixel& pixel2);
  friend Pixel operator*(const Pixel& pixel,
                           double         scale);
  friend Pixel operator*(double         scale,
                           const Pixel& pixel);
  friend Pixel operator/(const Pixel& pixel,
                           double         scale);

private:

  double c[4];
};

extern Pixel null_pixel;
extern Pixel black_pixel;
extern Pixel red_pixel;
extern Pixel green_pixel;
extern Pixel blue_pixel;
extern Pixel yellow_pixel;
extern Pixel cyan_pixel;
extern Pixel magenta_pixel;
extern Pixel white_pixel;

inline double Pixel::
Red(void) const
{
  // Return red component of pixel
  return c[0];
}

inline double Pixel::
Green(void) const
{
  // Return green component of pixel
  return c[1];
}

inline double Pixel::
Blue(void) const
{
  // Return blue component of pixel
  return c[2];
}

inline double Pixel::
Alpha(void) const
{
  // Return alpha component of pixel
  return c[3];
}

inline double * Pixel::
Components(void)
{
  // Return pixel array
  return c;
}

inline double Pixel::
Component(int i) const
{
  assert((i >= 0) && (i <= 3));
  return c[i];
}

inline double Pixel::
operator[](int i) const
{
  return Component(i);
}

inline double& Pixel::
operator[](int i)
{
  assert((i >= 0) && (i <= 3));
  return c[i];
}

inline bool Pixel::
IsBlack(void) const
{
  // Return whether color is black
  return (c[0] == 0.0) && (c[1] == 0.0) && (c[2] == 0.0);
}

inline bool Pixel::
IsWhite(void) const
{
  // Return whether color is white
  return (c[0] == 1.0) && (c[1] == 1.0) && (c[2] == 1.0);
}

inline double Pixel::
Luminance(void) const
{
  // Return luminance
  return 0.30 * c[0] + 0.59 * c[1] + 0.11 * c[2];
}

inline void Pixel::
SetRed(double red)
{
  // Set red component
  c[0] = red;
}

inline void Pixel::
SetGreen(double green)
{
  // Set green component
  c[1] = green;
}

inline void Pixel::
SetBlue(double blue)
{
  // Set blue component
  c[2] = blue;
}

inline void Pixel::
SetAlpha(double alpha)
{
  // Set alpha component
  c[3] = alpha;
}

inline void Pixel::
Reset(double red, double green, double blue, double alpha)
{
  // Set all components
  c[0] = red;
  c[1] = green;
  c[2] = blue;
  c[3] = alpha;
}

inline void Pixel::
Clamp(double maximum_value)
{
  // Set all components
  if (c[0] > maximum_value) c[0] = maximum_value;

  if (c[1] > maximum_value) c[1] = maximum_value;

  if (c[2] > maximum_value) c[2] = maximum_value;

  if (c[3] > maximum_value) c[3] = maximum_value;

  for (int i = 0; i < 4; i++)
    if (c[i] < 0.0) c[i] = 0.0;
}

inline Pixel
operator+(const Pixel& pixel1, const Pixel& pixel2)
{
  // Add rgb components of two pixels
  double r = pixel1.c[0] + pixel2.c[0];
  double g = pixel1.c[1] + pixel2.c[1];
  double b = pixel1.c[2] + pixel2.c[2];
  double a = pixel1.c[3];

  return Pixel(r, g, b, a);
}

inline Pixel
operator-(const Pixel& pixel1, const Pixel& pixel2)
{
  // Subtract rgb components of two pixels
  double r = pixel1.c[0] - pixel2.c[0];
  double g = pixel1.c[1] - pixel2.c[1];
  double b = pixel1.c[2] - pixel2.c[2];
  double a = pixel1.c[3];

  return Pixel(r, g, b, a);
}

inline Pixel
operator*(const Pixel& pixel1, const Pixel& pixel2)
{
  // Multiply rgb components of two pixels
  double r = pixel1.c[0] * pixel2.c[0];
  double g = pixel1.c[1] * pixel2.c[1];
  double b = pixel1.c[2] * pixel2.c[2];
  double a = pixel1.c[3];

  return Pixel(r, g, b, a);
}

inline Pixel
operator*(const Pixel& pixel, double scale)
{
  // Scale rgb components by scalar
  double r = pixel.c[0] * scale;
  double g = pixel.c[1] * scale;
  double b = pixel.c[2] * scale;
  double a = pixel.c[3];

  return Pixel(r, g, b, a);
}

inline Pixel
operator*(double scale, const Pixel& pixel)
{
  return pixel * scale;
}

inline Pixel
operator/(const Pixel& pixel, double scale)
{
  // Divide rgb components by scalar
  assert(scale != 0);
  double r = pixel.c[0] / scale;
  double g = pixel.c[1] / scale;
  double b = pixel.c[2] / scale;
  double a = pixel.c[3];
  return Pixel(r, g, b, a);
}
