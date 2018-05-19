// Source file for the R2Pixel class



// Include files

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include "R2Pixel.h"



// Public variables

R2Pixel R2null_pixel(0.0, 0.0, 0.0, 1.0);
R2Pixel R2black_pixel(0.0, 0.0, 0.0, 1.0);
R2Pixel R2red_pixel(1.0, 0.0, 0.0, 1.0);
R2Pixel R2green_pixel(0.0, 1.0, 0.0, 1.0);
R2Pixel R2blue_pixel(0.0, 0.0, 1.0, 1.0);
R2Pixel R2yellow_pixel(1.0, 1.0, 0.0, 1.0);
R2Pixel R2cyan_pixel(0.0, 1.0, 1.0, 1.0);
R2Pixel R2magenta_pixel(1.0, 0.0, 1.0, 1.0);
R2Pixel R2white_pixel(1.0, 1.0, 1.0, 1.0);



// Public functions

R2Pixel::
R2Pixel(void)
{
  // Initialize components to zero
  c[0] = 0;
  c[1] = 0;
  c[2] = 0;
  c[3] = 0;
}



R2Pixel::
R2Pixel(const R2Pixel& pixel)
{
  // Copy components
  c[0] = pixel.c[0];
  c[1] = pixel.c[1];
  c[2] = pixel.c[2];
  c[3] = pixel.c[3];
}



R2Pixel::
R2Pixel(double red, double green, double blue, double alpha)
{
  // Set components
  c[0] = red;
  c[1] = green;
  c[2] = blue;
  c[3] = alpha;
}



R2Pixel::
R2Pixel(const double rgba[4])
{
  // Set components
  c[0] = rgba[0];
  c[1] = rgba[1];
  c[2] = rgba[2];
  c[3] = rgba[3];
}



bool R2Pixel::
operator==(const R2Pixel& pixel) const
{
  // Return whether pixel is equal
  return ((c[0] == pixel.c[0]) && (c[1] == pixel.c[1]) && (c[2] == pixel.c[2]) && (c[3] == pixel.c[3]));
}



bool R2Pixel::
operator!=(const R2Pixel& pixel) const
{
  // Return whether pixel is not equal
  return ((c[0] != pixel.c[0]) || (c[1] != pixel.c[1]) || (c[2] != pixel.c[2]) || (c[3] != pixel.c[3]));
}



R2Pixel& R2Pixel::
operator=(const R2Pixel& pixel)
{
  // Copy components
  c[0] = pixel.c[0];
  c[1] = pixel.c[1];
  c[2] = pixel.c[2];
  c[3] = pixel.c[3];
  return *this;
}



R2Pixel& R2Pixel::
operator+=(const R2Pixel& pixel)
{
  c[0] += pixel.c[0];
  c[1] += pixel.c[1];
  c[2] += pixel.c[2];
  return *this;
}



R2Pixel& R2Pixel::
operator-=(const R2Pixel& pixel)
{
  c[0] -= pixel.c[0];
  c[1] -= pixel.c[1];
  c[2] -= pixel.c[2];
  return *this;
}



R2Pixel& R2Pixel::
operator*=(const R2Pixel& pixel)
{
  c[0] *= pixel.c[0];
  c[1] *= pixel.c[1];
  c[2] *= pixel.c[2];
  return *this;
}



R2Pixel& R2Pixel::
operator*=(double a)
{
  c[0] *= a;
  c[1] *= a;
  c[2] *= a;
  return *this;
}



R2Pixel& R2Pixel::
operator/=(double a)
{
  //  assert(!zero(a));
  c[0] /= a;
  c[1] /= a;
  c[2] /= a;
  return *this;
}
