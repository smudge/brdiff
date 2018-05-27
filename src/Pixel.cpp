#include "Pixel.h"

Pixel::
Pixel(void)
{
  c[0] = 0;
  c[1] = 0;
  c[2] = 0;
  c[3] = 0;
}

Pixel::
Pixel(const Pixel& pixel)
{
  c[0] = pixel.c[0];
  c[1] = pixel.c[1];
  c[2] = pixel.c[2];
  c[3] = pixel.c[3];
}

Pixel::
Pixel(double red, double green, double blue, double alpha)
{
  c[0] = red;
  c[1] = green;
  c[2] = blue;
  c[3] = alpha;
}

Pixel::
Pixel(const double rgba[4])
{
  c[0] = rgba[0];
  c[1] = rgba[1];
  c[2] = rgba[2];
  c[3] = rgba[3];
}

double Pixel::
Red(void) const
{
  return c[0];
}

double Pixel::
Green(void) const
{
  return c[1];
}

double Pixel::
Blue(void) const
{
  return c[2];
}

double Pixel::
Alpha(void) const
{
  return c[3];
}

double Pixel::
Luminance(void) const
{
  return 0.30 * c[0] + 0.59 * c[1] + 0.11 * c[2];
}

void Pixel::
SetRed(double red)
{
  c[0] = red;
}

void Pixel::
SetGreen(double green)
{
  c[1] = green;
}

void Pixel::
SetBlue(double blue)
{
  c[2] = blue;
}

void Pixel::
SetAlpha(double alpha)
{
  c[3] = alpha;
}

Pixel
operator-(const Pixel& pixel1, const Pixel& pixel2)
{
  double r = pixel1.c[0] - pixel2.c[0];
  double g = pixel1.c[1] - pixel2.c[1];
  double b = pixel1.c[2] - pixel2.c[2];
  double a = pixel1.c[3];

  return Pixel(r, g, b, a);
}
