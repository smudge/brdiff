// Source file for image class



// Include files

#include "R2/R2.h"
#include "R2Pixel.h"
#include "R2Image.h"




////////////////////////////////////////////////////////////////////////
// Constructors/Destructors
////////////////////////////////////////////////////////////////////////


R2Image::
R2Image(void)
  : pixels(NULL),
    npixels(0),
    width(0),
    height(0)
{
}



R2Image::
R2Image(const char *filename)
  : pixels(NULL),
    npixels(0),
    width(0),
    height(0)
{
  // Read image
  Read(filename);
}



R2Image::
R2Image(int width, int height)
  : pixels(NULL),
    npixels(width * height),
    width(width),
    height(height)
{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);
}



R2Image::
R2Image(int width, int height, const R2Pixel *p)
  : pixels(NULL),
    npixels(width * height),
    width(width),
    height(height)
{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = p[i];
}



R2Image::
R2Image(const R2Image& image)
  : pixels(NULL),
    npixels(image.npixels),
    width(image.width),
    height(image.height)

{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = image.pixels[i];
}



R2Image::
~R2Image(void)
{
  // Free image pixels
  if (pixels) delete [] pixels;
}



R2Image& R2Image::
operator=(const R2Image& image)
{
  // Delete previous pixels
  if (pixels) { delete [] pixels; pixels = NULL; }

  // Reset width and height
  npixels = image.npixels;
  width = image.width;
  height = image.height;

  // Allocate new pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = image.pixels[i];

  // Return image
  return *this;
}



////////////////////////////////////////////////////////////////////////
// Image processing functions
// YOU IMPLEMENT THE FUNCTIONS IN THIS SECTION
////////////////////////////////////////////////////////////////////////

// Per-pixel Operations ////////////////////////////////////////////////

void R2Image::
Brighten(double factor)
{
  // Brighten the image by multiplying each pixel component by the factor.
  // This is implemented for you as an example of how to access and set pixels

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      Pixel(i,j) *= factor;
      Pixel(i,j).Clamp();
    }
  }
}



void R2Image::
AddNoise(double factor)
{
  // Add noise to an image.  The amount of noise is given by the factor
  // in the range [0.0..1.0].  0.0 adds no noise.  1.0 adds a lot of noise.

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      //Add an amount of noise specified by the factor, done to each channel
      Pixel(i,j).SetRed(  Pixel(i,j).Red()  *(1-factor)+(rand()*1.0/RAND_MAX)*factor);
      Pixel(i,j).SetGreen(Pixel(i,j).Green()*(1-factor)+(rand()*1.0/RAND_MAX)*factor);
      Pixel(i,j).SetBlue( Pixel(i,j).Blue() *(1-factor)+(rand()*1.0/RAND_MAX)*factor);
    }
  }
}


void R2Image::
Speckle(double percentage)
{
  // Add speckled noise to an image.  The percentage of pixels that are speckled
  // is given by the parameter in the range [0.0..1.0].  0.0 changes no pixels,
  // 1.0 changes all the pixels to a random value (a different random value for each)
  // 0.5 will change half of the pixels.

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      if ((rand()*1.0/RAND_MAX) < percentage) {
        Pixel(i,j).SetRed(  rand()*1.0/RAND_MAX );
        Pixel(i,j).SetGreen(rand()*1.0/RAND_MAX );
        Pixel(i,j).SetBlue( rand()*1.0/RAND_MAX );
      }
    }
  }
}

//for use with saturation and contrast -- scales values down or up
void scaleVals(double* red, double* green, double* blue) {

      if (*red < 0) {
        *red = fabs(*red);
      }
      if (*red > 1) {
        *red = 1;
      }
      if (*blue < 0) {
        *blue = fabs(*blue);
      }
      if (*blue > 1) {
        *blue = 1;
      }
      if (*green < 0) {
       *green = fabs(*green);
      }
      if (*green > 1) {
        *green = 1;
      }

}

void R2Image::
ChangeSaturation(double factor)
{
  // Changes the saturation of an image by interpolating between the
  // image and a gray level version of the image.  Interpolation
  // decreases saturation, extrapolation increases it, negative factors
  // preserve luminance  but invert the hue of the input image.

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      double luminance = Pixel(i,j).Luminance();

      Pixel(i,j).SetRed  ( Pixel(i,j).Red()   * factor + luminance * (1-factor)  );
      Pixel(i,j).SetGreen( Pixel(i,j).Green() * factor + luminance * (1-factor)  );
      Pixel(i,j).SetBlue ( Pixel(i,j).Blue()  * factor + luminance * (1-factor)  );
      Pixel(i,j).Clamp();
    }
  }

}

void R2Image::
ChangeContrast(double factor)
{
  // Change the contrast of an image by interpolating between the image
  // and a constant gray image with the average luminance.
  // Interpolation reduces constrast, extrapolation boosts constrast,
  // and negative factors generate inverted images.

  // Cycle through the pixels and calculate average luminance.
  double lumsum = 0;
  for (int x = 0; x < width; x++) {
    for (int y = 0;  y < height; y++) {
      lumsum += Pixel(x,y).Luminance();
    }
  }
  double avelum = lumsum / npixels;

  // Cycle through the pixels again, using this value
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {

      Pixel(i,j).SetRed  ( Pixel(i,j).Red()   * factor + avelum * (1-factor) );
      Pixel(i,j).SetGreen( Pixel(i,j).Green() * factor + avelum * (1-factor) );
      Pixel(i,j).SetBlue ( Pixel(i,j).Blue()  * factor + avelum * (1-factor) );
      Pixel(i,j).Clamp();
    }
  }
}

void R2Image::
Threshold(double value)
{
  // Set all of the pixels with values less than value to 0.0

  //NOTE:
  //I did this separately for each colour channel
  //I could also have done it by pixel luminance...
  //Is there a preferred method?
  //the instructions were not clear.

  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      if (Pixel(i,j).Red() < value) {
        Pixel(i,j).SetRed( 0.0 );
      }
      if (Pixel(i,j).Blue() < value) {
        Pixel(i,j).SetBlue( 0.0 );
      }
      if (Pixel(i,j).Green() < value) {
        Pixel(i,j).SetGreen( 0.0 );
      }
    }
  }
}

void R2Image::
ApplyGamma(double exponent)
{
  // Apply a gamma correction with exponent to each pixel

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      //Add an amount of gamma specified by the exponent, done to each channel
      Pixel(i,j).SetRed  ( pow(Pixel(i,j).Red(),exponent)   );
      Pixel(i,j).SetGreen( pow(Pixel(i,j).Green(),exponent) );
      Pixel(i,j).SetBlue ( pow(Pixel(i,j).Blue(),exponent)  );
    }
  }
}

void R2Image::
BlackAndWhite(void)
{
  // Replace each pixel with its luminance value
  // Put this in each channel,  so the result is grayscale

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      double luminance = Pixel(i,j).Luminance();
      Pixel(i,j).SetRed(  luminance );
      Pixel(i,j).SetGreen(luminance );
      Pixel(i,j).SetBlue( luminance );
    }
  }
}

void R2Image::
ExtractChannel(int channel)
{
  // Extracts a channel of an image (e.g., R2_IMAGE_RED_CHANNEL).
  // Leaves the specified channel intact,
  // and sets all the other ones to zero.

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      //Add an amount of gamma specified by the exponent, done to each channel
      switch ( channel ) {
        case R2_IMAGE_RED_CHANNEL:
          Pixel(i,j).SetAlpha ( 0.0 );
          Pixel(i,j).SetGreen ( 0.0 );
          Pixel(i,j).SetBlue  ( 0.0 );
          break;
        case R2_IMAGE_GREEN_CHANNEL:
          Pixel(i,j).SetAlpha ( 0.0 );
          Pixel(i,j).SetRed   ( 0.0 );
          Pixel(i,j).SetBlue  ( 0.0 );
          break;
        case R2_IMAGE_BLUE_CHANNEL:
          Pixel(i,j).SetAlpha ( 0.0 );
          Pixel(i,j).SetRed   ( 0.0 );
          Pixel(i,j).SetGreen ( 0.0 );
          break;
        case R2_IMAGE_ALPHA_CHANNEL:
          Pixel(i,j).SetGreen ( 0.0 );
          Pixel(i,j).SetRed   ( 0.0 );
          Pixel(i,j).SetBlue  ( 0.0 );
          break;
      }
    }
  }
}

void R2Image::
EqualizeLuminanceHistogram(void)
{
  // Increase the global contrast by converting to HLS color space,
  // applying histogram equalization to the luminance channel, and
  // converting back to RGB.
  // See http://en.wikipedia.org/wiki/Histogram_equalization

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "EqualizeLuminanceHistogram() not implemented\n");
}


void Gaussian (R2Pixel *src, int i1, int j1, int wd1, int ht1, R2Pixel *dst, int i2, int j2, int wd2, int ht2, double *w, int w_size, int direction) {
      int stop;
      int coord;
      int edge0;
      int edge1;
      int xmult;
      switch (direction) {
        default:
        case 0:
          coord = i1;
          stop = wd1;
          edge0 = j1;
          edge1 = (wd1-1)*ht1 + j1;
          xmult = ht1;
          break;
        case 1:
          coord = j1;
          stop = ht1;
          edge0 = i1*ht1;
          edge1 = i1*ht1 + ht1-1;
          xmult = 1;
          break;
      }

      int dst_coord = i2*ht2+j2;
      int src_coord = i1*ht1+j1;

      //clear destination pixel
      dst[dst_coord] *= 0.0;

      // Make use of each window value
      for (int q = 0; q <= w_size; q++) {
        int x = q - (w_size-1)/2;


        // IF we're not outside of the image: Apply the value stored in "window"
        if ((coord+x >= 0) && (coord+x < stop))
          dst[dst_coord] += src[src_coord+x*xmult]*w[q];


        // ELSE: Account for edges
        else if (coord+x < 0)
          dst[dst_coord] += src[edge0]*w[q];
        else if (coord+x >= stop)
          dst[dst_coord] += src[edge1]*w[q];
      }

}

// Linear filtering ////////////////////////////////////////////////
void R2Image::
Blur(double sigma) {
  // Blur an image with a Gaussian filter with a given sigma.

  // Allocate new pixels to store values
  R2Pixel *pixels2 = new R2Pixel [ npixels ];
  assert(pixels2);
  // Create the window array
  int win_size = ceil(6 * sigma);
  double window[ win_size+1 ];
  // Calculate guassian distribution
  for (int q = 0; q <= win_size; q++) {
    int x = q - (win_size-1)/2;
    window[q] = pow(sqrt(2*PI*pow(sigma,2)),-1)*pow(E,-(pow(x,2)/(2*pow(sigma,2))));
  }
  // ROWS
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      Gaussian(pixels, i, j, width, height, pixels2, i, j, width, height, window, win_size, 0);
    }
  }
  // COLUMNS
  for (int j = 0;  j < height; j++) {
    for (int i = 0; i < width; i++) {
      Gaussian(pixels2, i, j, width, height, pixels, i, j, width, height, window, win_size, 1);
    }
  }
  //free up all the pixels (before they attack!)
  if (pixels2) delete [] pixels2;
}


void R2Image::
Sharpen()
{
  // Sharpen an image using a linear filter

 // Allocate new pixels to store values
  R2Pixel *pixels2 = new R2Pixel [ npixels ];
  assert(pixels2);
  //create window
  double window[3][3] = {{-1, -1, -1},
                      {-1, 9, -1},
                      {-1, -1,-1}};

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      //cycle through window
      R2Pixel pixelsum = R2Pixel();
      for(int m = 0; (m < 3)&&((i+(m-1))<width); m++) {
        for(int n = 0; (n < 3)&&((j+(n-1))<height); n++) {
          if (((i+(m-1))>=0)&&((j+(n-1))>=0))
            pixelsum += Pixel(i+(m-1),j+(n-1))*window[m][n];
        }
      }
      pixelsum.Clamp();
      pixels2[i*height+j] = ((pixelsum)+Pixel(i,j))/2;
    }
  }

  //free up all the pixels (before they attack!)
  if (pixels) delete [] pixels;
  pixels = pixels2;
}


void R2Image::
EdgeDetect(void)
{
  // Detect edges in an image.

  //NOTE, THESE KERNELS ARE BASED AROUND THE SOBEL OPERATOR
  //BUT IS ALSO IN PART A RESULT OF MY OWN EXPERIMENTATION

  // Allocate new pixels to store values
  R2Pixel *pixels2 = new R2Pixel [ npixels ];
  assert(pixels2);
  //create window
  int window[3][3] = {{-1, 0, 1},
                      {-2, 0, 2},
                      {-1, 0, 1}};

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      //cycle through window
      R2Pixel pixelsum = R2Pixel();
      for(int m = 0; m < 3; m++) {
        for(int n = 0; n < 3; n++) {
          if (((i+(m-1))>=0)&&((j+(n-1))>=0)&&((i+(m-1))<width)&&((j+(n-1))<height)) {
            pixelsum += Pixel(i+(m-1),j+(n-1))*window[m][n];
          }
        }
      }
      pixelsum.Clamp();
      pixels2[i*height+j] = pixelsum;
    }
  }

  int window2[3][3] = {{-1, -2, -1},
                       {0, 0, 0},
                       {1, 2, 1}};

  // Cycle through the pixels
    for (int j = 0;  j < height; j++) {
  for (int i = 0; i < width; i++) {
      //cycle through window
      R2Pixel pixelsum = R2Pixel();
      for(int m = 0; m < 3; m++) {
        for(int n = 0; n < 3; n++) {
          if (((i+(m-1))>=0)&&((j+(n-1))>=0)&&((i+(m-1))<width)&&((j+(n-1))<height)) {
            pixelsum += pixels2[(i+(m-1))*height+j+(n-1)]*window2[m][n];
          }
        }
      }
      pixelsum.Clamp();
      Pixel(i,j) = (pixelsum+pixels2[i*height+j])/2;
    }
  }

  //free up all the pixels (before they attack!)
  if (pixels2) delete [] pixels2;
}



// Non-Linear filtering ////////////////////////////////////////////////

//Compare function for qsort!
int compare_doubles (const void *X, const void *Y)
{
  double x = *((double *)X);
  double y = *((double *)Y);
  if (x > y)
    return 1;
  else if (x < y)
    return -1;
  else
    return 0;
}


void R2Image::
MedianFilter(double window_w) {
  //rounding this up (why not?) it's supposed to be a pixel value
  //so this prevents errors.
  window_w = ceil(window_w);

  // Perform median filtering with a given width
  int window_w_2 = (int)floor((window_w)/2);

  // Allocate new pixels to store values
  R2Pixel *pixels2 = new R2Pixel [ npixels ];
  assert(pixels2);

  // Create the window array
  double window[4][(int)window_w*(int)window_w];

  // ITERATE ACROSS ALL PIXELS
  for (int x = 0; x < width; x++) {
    for (int y = 0;  y < height; y++) {

      // FILL WINDOW SURROUNDING PIXEL X,Y
      int q = 0;
      for (int i = 0; i < window_w; i++) {
        for (int j = 0; j < window_w; j++) {
          int x2 = i - window_w_2;
          int y2 = j - window_w_2;
          int x3;
          int y3;
          if (x+x2 < 0)
            x3 = 0;
          else if (x+x2 >= width)
            x3 = width-1;
          else
            x3 = x + x2;
          if (y+y2 < 0)
            y3 = 0;
          else if (y+y2 >= height)
            y3 = height-1;
          else
            y3 = y + y2;

          R2Pixel pix = Pixel(x3,y3);
          window[0][q] = pix.Red();
          window[1][q] = pix.Green();
          window[2][q] = pix.Blue();
          window[3][q] = pix.Alpha();
          q++;
        }
      }
      // sort windows
      for (int c = 0; c <= 3; c++)
        qsort(window[c], q, sizeof(double), compare_doubles);
      // set values at medians

      pixels2[x*height+y].SetRed  (window[0][window_w_2]);
      pixels2[x*height+y].SetGreen(window[1][window_w_2]);
      pixels2[x*height+y].SetBlue (window[2][window_w_2]);
      pixels2[x*height+y].SetAlpha(window[3][window_w_2]);
    }
  }

  //set all of the values back

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      R2Pixel pix2 = pixels2[x*height+y];
      Pixel(x,y).SetRed(   pix2.Red()   );
      Pixel(x,y).SetGreen( pix2.Green() );
      Pixel(x,y).SetBlue(  pix2.Blue()  );
      Pixel(x,y).SetAlpha( pix2.Alpha() );
    }
  }

  //free up all the pixels (before they attack!)
  if (pixels2) delete [] pixels2;
}

void R2Image::
BilateralFilter(double rangesigma, double domainsigma)
{
  // Perform bilateral filtering with a given range and domain widths.

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "BilateralFilter(%g, %g) not implemented\n", rangesigma, domainsigma);
}


// Resampling operations  ////////////////////////////////////////////////

void solveEdges(int*u, int*v, int height, int width) {
      if (*u >= width)
        *u = width-1;
      if (*v >= height)
        *v = height-1;
      if (*u < 0)
        *u = 0;
      if (*v < 0)
        *v = 0;
}

R2Pixel R2Image::Sample(double u, double v,  int sampling_method, double sigma){
  if ((u < 0)||(v < 0)||(u >= width)||(v >= width)) {
    return R2Pixel(1.0,1.0,1.0,1.0); //return white
  }
  else {
    switch (sampling_method) {
      default:
      case 0: {
        int iu = round(u);
        int iv = round(v);
        solveEdges(&iu,&iv,height,width);
        return Pixel(iu,iv);
        }
        break;
      case 1: {
        int u1 = floor(u);
        int u2 = ceil(u);
        int v1 = floor(v);
        int v2 = ceil(v);
        solveEdges(&u1,&v1,height,width);
        solveEdges(&u2,&v2,height,width);
        double ublw = (fabs(u-u1) == 0) ? 0.5 : fabs(u-u1);
        double uabv = (fabs(u2-u) == 0) ? 0.5 : fabs(u2-u);
        double vblw = (fabs(v-v1) == 0) ? 0.5 : fabs(v-v1);
        double vabv = (fabs(v2-v) == 0) ? 0.5 : fabs(v2-v);

        R2Pixel a = Pixel(u1,v1)*uabv + Pixel(u2,v1)*ublw;
        R2Pixel b = Pixel(u1,v2)*uabv + Pixel(u2,v2)*ublw;
        return (a*vabv + b*vblw);
      }
        break;
      case 2: {

        //The size of the blur filter is the inverse of the minification factor, rounded up to the closest odd number greater than or equal to 3.

        double w = 3;

        double ulo = u - w;
        double uhi = u + w;
        double vlo = v - w;
        double vhi = v + w;



        //sum up values in window:
        R2Pixel a = R2Pixel();
        double weightsum = 0;
        for (double iu = ceil(ulo); iu <= floor(uhi); iu++) {
          for (double iv = ceil(vlo); iv <= floor(vhi); iv++) {

            double d = sqrt((u-iu)*(u-iu)+(v-iv)*(v-iv));

            //printf("(%f,%f)(%f,%f) %f\n",u,v,iu,iv,d);
            int x = iu;
            int y = iv;
            solveEdges(&x,&y,height,width);
            double weight = pow(2*PI*pow(sigma,2.0),-1)*pow(E,-(pow(d,2.0)/(2*pow(sigma,2))));
            a += Pixel(x,y) * weight;

            //* pow(4,-pow((d/sigma),2));

            //printf("pixel: %f, %f | point: %f %f\n", u, v, iu, iv);
            weightsum += weight;
          }
        }


        return a/weightsum; //normalized
      }
        break;
    }
  }
}


void R2Image::
Scale(double sx, double sy, int sampling_method) {
  // Scale an image in x by sx, and y by sy.

  if ((sx != 1)||(sx != 1)) {

    if ((sampling_method == 2)&&(sx<1)&&(sy<1)) {
      Blur(1/((sx+sy)));
      sampling_method = 0;
    }

    int dwidth = round(width*sx);
    int dheight = round(height*sy);

    // Allocate new pixels to store values
    R2Pixel *pixels2 = new R2Pixel [ dwidth*dheight ];
    assert(pixels2);

    for (int x = 0; x < dwidth; x++) {
      for (int y = 0; y < dheight; y++) {
        pixels2[ x*dheight+y ] = Sample(x*1.0/sx,y*1.0/sy,sampling_method, 0.7);
      }
    }
    //set new height and width
    npixels = dwidth * dheight;
    width = dwidth;
    height = dheight;

    //swap the pixel data
    if (pixels) delete [] pixels;
    pixels = pixels2;
  }

}


void R2Image::
Rotate(double angle, int sampling_method)
{
  // Rotate an image by the given angle.

  // Allocate new pixels to store values
  R2Pixel *pixels2 = new R2Pixel [ npixels*7 ];
  assert(pixels2);


  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      //convert to (0,0) at center
      double x = 2*i-width;
      double y = 2*j-height;
      //convert to radial coordinates
      double r = sqrt(x*x + y*y);
      double phi = atan2(y,x);
      //apply rotation
      phi = phi + angle/180*PI;
      //convert to cartesian coordinates
      x = r*cos(phi);
      y = r*sin(phi);
      //convert to (0,0) at usual place
      double i2 = (x + width)/2;
      double j2 = (y + height)/2;

      pixels2[i*height+j] = Sample(i2,j2,sampling_method, 0.5);
    }
  }


  //swap the pixel data
  if (pixels) delete [] pixels;
  pixels = pixels2;

}


void R2Image::
MotionBlur(R2Vector& movement, int sampling_method)
{
  // Perform Motion Blur as if the camera translated along the given vector

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "MotionBlur(%g %g) not implemented for method %d\n", movement.X(), movement.Y(), sampling_method);
}



void R2Image::
Fun(int sampling_method)
{
  // Warp an image using a creative filter of your choice.

  // Allocate new pixels to store values
  R2Pixel *pixels2 = new R2Pixel [ npixels*7 ];
  assert(pixels2);

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      //convert to (0,0) at center
      double x = 2*i-width;
      double y = 2*j-height;

      //convert to radial coordinates
      double r = sqrt(x*x + y*y);
      double phi = atan2(y,x);

      //apply crazy warp
      phi = pow(phi,2);

      //convert to cartesian coordinates
      x = r * cos(phi);
      y = r * sin(phi);

      //convert to (0,0) at usual place
      double i2 = (x + width)/2;
      double j2 = (y + height)/2;


      pixels2[i*height+j] = Sample(i2,j2,sampling_method, 0.7);
    }
  }


  //swap the pixel data
  if (pixels) delete [] pixels;
  pixels = pixels2;


}


// Dither operations ////////////////////////////////////////////////

void R2Image::
Quantize (int nbits)
{
  // Quantizes an image with "nbits" bits per channel.

  double factor = (pow(2,nbits)-1);

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = height-1;  j >= 0; j--) {
      double old_red = Pixel(i,j).Red();
      double new_red = round(old_red*factor)/factor;

      double old_grn = Pixel(i,j).Green();
      double new_grn = round(old_grn*factor)/factor;

      double old_blu = Pixel(i,j).Blue();
      double new_blu = round(old_blu*factor)/factor;

      double old_alp = Pixel(i,j).Alpha();
      double new_alp = round(old_alp*factor)/factor;

      Pixel(i,j).SetRed(  new_red);
      Pixel(i,j).SetGreen(new_grn);
      Pixel(i,j).SetBlue( new_blu);
      Pixel(i,j).SetAlpha(new_alp);
      Pixel(i,j).Clamp();
    }
  }
}



void R2Image::
RandomDither(int nbits)
{
  // Converts and image to nbits per channel using random dither.

  double factor = (pow(2,nbits)-1);

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = height-1;  j >= 0; j--) {
      double old_red = Pixel(i,j).Red();
      double new_red = round(old_red*factor + (rand()*1.0/RAND_MAX)-0.5)/factor;

      double old_grn = Pixel(i,j).Green();
      double new_grn = round(old_grn*factor + (rand()*1.0/RAND_MAX)-0.5)/factor;

      double old_blu = Pixel(i,j).Blue();
      double new_blu = round(old_blu*factor + (rand()*1.0/RAND_MAX)-0.5)/factor;

      double old_alp = Pixel(i,j).Alpha();
      double new_alp = round(old_alp*factor + (rand()*1.0/RAND_MAX)-0.5)/factor;

      Pixel(i,j).Reset(new_red,new_grn,new_blu,new_alp);
      Pixel(i,j).Clamp();
    }
  }

}



void R2Image::
OrderedDither(int nbits)
{
  // Converts an image to nbits per channel using ordered dither,
  // with a 4x4 Bayer's pattern matrix.

  double factor = (pow(2,nbits)-1);
  int Bayer4[4][4] = {{15, 7, 13, 5},
                      {3, 11, 1,  9},
                      {12, 4, 14, 6},
                      {0,  8, 2,  10}};

  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {

      int n = i % 4;
      int g = j % 4;

      double old_red = Pixel(i,j).Red();
      double new_red = round(old_red * factor - 0.5 + Bayer4[n][g] / 15.0) / factor;

      double old_grn = Pixel(i,j).Green();
      double new_grn = round(old_grn * factor - 0.5 + Bayer4[n][g] / 15.0) / factor;

      double old_blu = Pixel(i,j).Blue();
      double new_blu = round(old_blu * factor - 0.5 + Bayer4[n][g] / 15.0) / factor;

      double old_alp = Pixel(i,j).Alpha();
      double new_alp = round(old_alp * factor - 0.5 + Bayer4[n][g] / 15.0) / factor;

      Pixel(i,j).Reset(new_red, new_grn, new_blu, new_alp);
      Pixel(i,j).Clamp();
    }
  }

}

void addQuant(R2Pixel* pixels, int i, int j, double err_red, double err_grn, double err_blu, double err_alp, int width, int height) {
  if ((i>=0)&&(i<width)&&(j>=0)&&(j<height)) {
    pixels[i*height+j].SetRed(    pixels[i*height+j].Red()   + err_red);
    pixels[i*height+j].SetGreen(  pixels[i*height+j].Green() + err_grn);
    pixels[i*height+j].SetBlue(   pixels[i*height+j].Blue()  + err_blu);
    pixels[i*height+j].SetAlpha(  pixels[i*height+j].Alpha() + err_alp);
    pixels[i*height+j].Clamp();
  }
}

void R2Image::
FloydSteinbergDither(int nbits)
{
  // Converts an image to nbits per channel using Floyd-Steinberg dither.
  // with error diffusion.

  double factor = (pow(2,nbits)-1);

  // Cycle through the pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      double old_red = Pixel(i,j).Red();
      double new_red = round(old_red*factor)/factor;
      double err_red = old_red - new_red;

      double old_grn = Pixel(i,j).Green();
      double new_grn = round(old_grn*factor)/factor;
      double err_grn = old_grn - new_grn;

      double old_blu = Pixel(i,j).Blue();
      double new_blu = round(old_blu*factor)/factor;
      double err_blu = old_blu - new_blu;

      double old_alp = Pixel(i,j).Alpha();
      double new_alp = round(old_alp*factor)/factor;
      double err_alp = old_alp - new_alp;

      Pixel(i,j).Reset(new_red, new_grn, new_blu, new_alp);
      Pixel(i,j).Clamp();

      addQuant(pixels, i+1, j,   err_red*7/16, err_grn*7/16, err_blu*7/16, err_alp*7/16, width, height);
      addQuant(pixels, i-1, j+1, err_red*3/16, err_grn*3/16, err_blu*3/16, err_alp*3/16, width, height);
      addQuant(pixels, i,   j+1, err_red*5/16, err_grn*5/16, err_blu*5/16, err_alp*5/16, width, height);
      addQuant(pixels, i+1, j+1, err_red*1/16, err_grn*1/16, err_blu*1/16, err_alp*1/16, width, height);
    }
  }

}



// Miscellaneous operations ////////////////////////////////////////////////

void R2Image::
Crop(int x, int y, int w, int h)
{
  // Extracts a sub image from the image,
  // at position (x, y), width w, and height h.

  // Allocate new pixels to store values
  R2Pixel *pixels2 = new R2Pixel [npixels];
  assert(pixels2);

  if (x < 0)
    x = 0;
  if (x >= width)
    x = width -1;
  if (y < 0)
    y = 0;
  if (y >= height)
    y = height -1;
  if (w + x >= width) {
    w = (width - 1) - x;
  }
  if (h + y >= height) {
    h = (height - 1) - y;
  }

  for (int j = y;  j < h+y; j++) {
    for (int i = x; i < w+x; i++) {

      pixels2[(i-x)*h+(j-y)] = Pixel(i,j);

    }
  }

  //set new values
  width = w;
  height = h;
  npixels = w*h;

  //swap the pixel data
  if (pixels) delete [] pixels;
  pixels = pixels2;

}

void R2Image::
Composite(const R2Image& top, int operation)
{
  // Composite passed image on top of this one using operation (e.g., OVER)

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "Composite not implemented\n");
}

void R2Image::
CopyChannel(const R2Image& from_image, int from_channel, int to_channel)
{
  // Copies one channel of an image (e.g., R2_IMAGE_RED_CHANNEL).
  // to another channel

  // MAY FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE) (NO CREDIT FOR ASSIGNMENT)
  fprintf(stderr, "CopyChannel not implemented\n");
}

void R2Image::
Add(const R2Image& image)
{
  // Add passed image pixel-by-pixel.

  // MAY FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE) (NO CREDIT FOR ASSIGNMENT)
  fprintf(stderr, "Add not implemented\n");
}

void R2Image::
Subtract(const R2Image& image)
{
  if ((width != image.width) || (height != image.height)) {
    fprintf(stderr, "Image height and width do not match\n");
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      const R2Pixel& pixel = (*this)[i][j];
      if ((pixel.Red() != pixel.Blue()) || (pixel.Red() != pixel.Green())) {
        fprintf(stderr, "Pixel values not the same across all color channels!\n");
      }

      const R2Pixel& pixel2 = (image)[i][j];
      if ((pixel2.Red() != pixel2.Blue()) || (pixel2.Red() != pixel2.Green())) {
        fprintf(stderr, "Pixel values not the same across all color channels!\n");
      }

      double value = pixel.Red() - pixel2.Red();

      Pixel(i,j).SetGreen(0);

      if (value >= 0) {
        Pixel(i,j).SetRed(0);
        Pixel(i,j).SetBlue(value);
      }

      if (value < 0) {
        Pixel(i,j).SetRed(-1 * value);
        Pixel(i,j).SetBlue(0);
      }
    }
  }
}

void R2Image::
Morph(const R2Image& target,
  R2Segment *source_segments, R2Segment *target_segments, int nsegments,
  double t, int sampling_method)
{
  // Morph this source image towards a passed target image by t using pairwise line segment correspondences

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "Morph not implemented\n");
}


////////////////////////////////////////////////////////////////////////
// I/O Functions
////////////////////////////////////////////////////////////////////////

int R2Image::
Read(const char *filename)
{
  // Initialize everything
  if (pixels) { delete [] pixels; pixels = NULL; }
  npixels = width = height = 0;

  // Parse input filename extension
  char *input_extension;
  if (!(input_extension = (char*)strrchr(filename, '.'))) {
    fprintf(stderr, "Input file has no extension (e.g., .jpg).\n");
    return 0;
  }

  // Read file of appropriate type
  if (!strncmp(input_extension, ".bmp", 4)) return ReadBMP(filename);
  else if (!strncmp(input_extension, ".ppm", 4)) return ReadPPM(filename);
  else if (!strncmp(input_extension, ".jpg", 4)) return ReadJPEG(filename);
  else if (!strncmp(input_extension, ".jpeg", 5)) return ReadJPEG(filename);

  // Should never get here
  fprintf(stderr, "Unrecognized image file extension");
  return 0;
}



int R2Image::
Write(const char *filename) const
{
  // Parse input filename extension
  char *input_extension;
  if (!(input_extension = (char*)strrchr(filename, '.'))) {
    fprintf(stderr, "Input file has no extension (e.g., .jpg).\n");
    return 0;
  }

  // Write file of appropriate type
  if (!strncmp(input_extension, ".bmp", 4)) return WriteBMP(filename);
  else if (!strncmp(input_extension, ".ppm", 4)) return WritePPM(filename, 1);
  else if (!strncmp(input_extension, ".jpg", 5)) return WriteJPEG(filename);
  else if (!strncmp(input_extension, ".jpeg", 5)) return WriteJPEG(filename);

  // Should never get here
  fprintf(stderr, "Unrecognized image file extension");
  return 0;
}



////////////////////////////////////////////////////////////////////////
// BMP I/O
////////////////////////////////////////////////////////////////////////

#if (RN_OS == RN_LINUX) && !WIN32

typedef struct tagBITMAPFILEHEADER {
  unsigned short int bfType;
  unsigned int bfSize;
  unsigned short int bfReserved1;
  unsigned short int bfReserved2;
  unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  unsigned int biSize;
  int biWidth;
  int biHeight;
  unsigned short int biPlanes;
  unsigned short int biBitCount;
  unsigned int biCompression;
  unsigned int biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBTRIPLE {
  unsigned char rgbtBlue;
  unsigned char rgbtGreen;
  unsigned char rgbtRed;
} RGBTRIPLE;

typedef struct tagRGBQUAD {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD;

#endif

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

#define BMP_BF_TYPE 0x4D42 /* word BM */
#define BMP_BF_OFF_BITS 54 /* 14 for file header + 40 for info header (not sizeof(), but packed size) */
#define BMP_BI_SIZE 40 /* packed size of info header */


static unsigned short int WordReadLE(FILE *fp)
{
  // Read a unsigned short int from a file in little endian format
  unsigned short int lsb, msb;
  lsb = getc(fp);
  msb = getc(fp);
  return (msb << 8) | lsb;
}



static void WordWriteLE(unsigned short int x, FILE *fp)
{
  // Write a unsigned short int to a file in little endian format
  unsigned char lsb = (unsigned char) (x & 0x00FF); putc(lsb, fp);
  unsigned char msb = (unsigned char) (x >> 8); putc(msb, fp);
}



static unsigned int DWordReadLE(FILE *fp)
{
  // Read a unsigned int word from a file in little endian format
  unsigned int b1 = getc(fp);
  unsigned int b2 = getc(fp);
  unsigned int b3 = getc(fp);
  unsigned int b4 = getc(fp);
  return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



static void DWordWriteLE(unsigned int x, FILE *fp)
{
  // Write a unsigned int to a file in little endian format
  unsigned char b1 = (x & 0x000000FF); putc(b1, fp);
  unsigned char b2 = ((x >> 8) & 0x000000FF); putc(b2, fp);
  unsigned char b3 = ((x >> 16) & 0x000000FF); putc(b3, fp);
  unsigned char b4 = ((x >> 24) & 0x000000FF); putc(b4, fp);
}



static int LongReadLE(FILE *fp)
{
  // Read a int word from a file in little endian format
  int b1 = getc(fp);
  int b2 = getc(fp);
  int b3 = getc(fp);
  int b4 = getc(fp);
  return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



static void LongWriteLE(int x, FILE *fp)
{
  // Write a int to a file in little endian format
  char b1 = (x & 0x000000FF); putc(b1, fp);
  char b2 = ((x >> 8) & 0x000000FF); putc(b2, fp);
  char b3 = ((x >> 16) & 0x000000FF); putc(b3, fp);
  char b4 = ((x >> 24) & 0x000000FF); putc(b4, fp);
}



int R2Image::
ReadBMP(const char *filename)
{
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  /* Read file header */
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = WordReadLE(fp);
  bmfh.bfSize = DWordReadLE(fp);
  bmfh.bfReserved1 = WordReadLE(fp);
  bmfh.bfReserved2 = WordReadLE(fp);
  bmfh.bfOffBits = DWordReadLE(fp);

  /* Check file header */
  assert(bmfh.bfType == BMP_BF_TYPE);
  /* ignore bmfh.bfSize */
  /* ignore bmfh.bfReserved1 */
  /* ignore bmfh.bfReserved2 */
  assert(bmfh.bfOffBits == BMP_BF_OFF_BITS);

  /* Read info header */
  BITMAPINFOHEADER bmih;
  bmih.biSize = DWordReadLE(fp);
  bmih.biWidth = LongReadLE(fp);
  bmih.biHeight = LongReadLE(fp);
  bmih.biPlanes = WordReadLE(fp);
  bmih.biBitCount = WordReadLE(fp);
  bmih.biCompression = DWordReadLE(fp);
  bmih.biSizeImage = DWordReadLE(fp);
  bmih.biXPelsPerMeter = LongReadLE(fp);
  bmih.biYPelsPerMeter = LongReadLE(fp);
  bmih.biClrUsed = DWordReadLE(fp);
  bmih.biClrImportant = DWordReadLE(fp);

  // Check info header
  assert(bmih.biSize == BMP_BI_SIZE);
  assert(bmih.biWidth > 0);
  assert(bmih.biHeight > 0);
  assert(bmih.biPlanes == 1);
  assert(bmih.biBitCount == 24);  /* RGB */
  assert(bmih.biCompression == BI_RGB);   /* RGB */
  int lineLength = bmih.biWidth * 3;  /* RGB */
  if ((lineLength % 4) != 0) lineLength = (lineLength / 4 + 1) * 4;
  assert(bmih.biSizeImage == (unsigned int) lineLength * (unsigned int) bmih.biHeight);

  // Assign width, height, and number of pixels
  width = bmih.biWidth;
  height = bmih.biHeight;
  npixels = width * height;

  // Allocate unsigned char buffer for reading pixels
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = bmih.biSizeImage;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Read buffer
  fseek(fp, (long) bmfh.bfOffBits, SEEK_SET);
  if (fread(buffer, 1, bmih.biSizeImage, fp) != bmih.biSizeImage) {
    fprintf(stderr, "Error while reading BMP file %s", filename);
    return 0;
  }

  // Close file
  fclose(fp);

  // Allocate pixels for image
  pixels = new R2Pixel [ width * height ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Assign pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      double b = (double) *(p++) / 255;
      double g = (double) *(p++) / 255;
      double r = (double) *(p++) / 255;
      R2Pixel pixel(r, g, b, 1);
      SetPixel(i, j, pixel);
    }
  }

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return success
  return 1;
}



int R2Image::
WriteBMP(const char *filename) const
{
  // Open file
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Compute number of bytes in row
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;

  // Write file header
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = BMP_BF_TYPE;
  bmfh.bfSize = BMP_BF_OFF_BITS + rowsize * height;
  bmfh.bfReserved1 = 0;
  bmfh.bfReserved2 = 0;
  bmfh.bfOffBits = BMP_BF_OFF_BITS;
  WordWriteLE(bmfh.bfType, fp);
  DWordWriteLE(bmfh.bfSize, fp);
  WordWriteLE(bmfh.bfReserved1, fp);
  WordWriteLE(bmfh.bfReserved2, fp);
  DWordWriteLE(bmfh.bfOffBits, fp);

  // Write info header
  BITMAPINFOHEADER bmih;
  bmih.biSize = BMP_BI_SIZE;
  bmih.biWidth = width;
  bmih.biHeight = height;
  bmih.biPlanes = 1;
  bmih.biBitCount = 24;       /* RGB */
  bmih.biCompression = BI_RGB;    /* RGB */
  bmih.biSizeImage = rowsize * (unsigned int) bmih.biHeight;  /* RGB */
  bmih.biXPelsPerMeter = 2925;
  bmih.biYPelsPerMeter = 2925;
  bmih.biClrUsed = 0;
  bmih.biClrImportant = 0;
  DWordWriteLE(bmih.biSize, fp);
  LongWriteLE(bmih.biWidth, fp);
  LongWriteLE(bmih.biHeight, fp);
  WordWriteLE(bmih.biPlanes, fp);
  WordWriteLE(bmih.biBitCount, fp);
  DWordWriteLE(bmih.biCompression, fp);
  DWordWriteLE(bmih.biSizeImage, fp);
  LongWriteLE(bmih.biXPelsPerMeter, fp);
  LongWriteLE(bmih.biYPelsPerMeter, fp);
  DWordWriteLE(bmih.biClrUsed, fp);
  DWordWriteLE(bmih.biClrImportant, fp);

  // Write image, swapping blue and red in each pixel
  int pad = rowsize - width * 3;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      const R2Pixel& pixel = (*this)[i][j];
      double r = 255.0 * pixel.Red();
      double g = 255.0 * pixel.Green();
      double b = 255.0 * pixel.Blue();
      if (r >= 255) r = 255;
      if (g >= 255) g = 255;
      if (b >= 255) b = 255;
      fputc((unsigned char) b, fp);
      fputc((unsigned char) g, fp);
      fputc((unsigned char) r, fp);
    }

    // Pad row
    for (int i = 0; i < pad; i++) fputc(0, fp);
  }

  // Close file
  fclose(fp);

  // Return success
  return 1;
}



////////////////////////////////////////////////////////////////////////
// PPM I/O
////////////////////////////////////////////////////////////////////////

int R2Image::
ReadPPM(const char *filename)
{
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Read PPM file magic identifier
  char buffer[128];
  if (!fgets(buffer, 128, fp)) {
    fprintf(stderr, "Unable to read magic id in PPM file");
    fclose(fp);
    return 0;
  }

  // skip comments
  int c = getc(fp);
  while (c == '#') {
    while (c != '\n') c = getc(fp);
    c = getc(fp);
  }
  ungetc(c, fp);

  // Read width and height
  if (fscanf(fp, "%d%d", &width, &height) != 2) {
    fprintf(stderr, "Unable to read width and height in PPM file");
    fclose(fp);
    return 0;
  }

  // Read max value
  double max_value;
  if (fscanf(fp, "%lf", &max_value) != 1) {
    fprintf(stderr, "Unable to read max_value in PPM file");
    fclose(fp);
    return 0;
  }

  // Allocate image pixels
  pixels = new R2Pixel [ width * height ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for PPM file");
    fclose(fp);
    return 0;
  }

  // Check if raw or ascii file
  if (!strcmp(buffer, "P6\n")) {
    // Read up to one character of whitespace (\n) after max_value
    int c = getc(fp);
    if (!isspace(c)) putc(c, fp);

    // Read raw image data
    // First ppm pixel is top-left, so read in opposite scan-line order
    for (int j = height-1; j >= 0; j--) {
      for (int i = 0; i < width; i++) {
        double r = (double) getc(fp) / max_value;
        double g = (double) getc(fp) / max_value;
        double b = (double) getc(fp) / max_value;
        R2Pixel pixel(r, g, b, 1);
        SetPixel(i, j, pixel);
      }
    }
  }
  else {
    // Read asci image data
    // First ppm pixel is top-left, so read in opposite scan-line order
    for (int j = height-1; j >= 0; j--) {
      for (int i = 0; i < width; i++) {
	// Read pixel values
	int red, green, blue;
	if (fscanf(fp, "%d%d%d", &red, &green, &blue) != 3) {
	  fprintf(stderr, "Unable to read data at (%d,%d) in PPM file", i, j);
	  fclose(fp);
	  return 0;
	}

	// Assign pixel values
	double r = (double) red / max_value;
	double g = (double) green / max_value;
	double b = (double) blue / max_value;
        R2Pixel pixel(r, g, b, 1);
        SetPixel(i, j, pixel);
      }
    }
  }

  // Close file
  fclose(fp);

  // Return success
  return 1;
}



int R2Image::
WritePPM(const char *filename, int ascii) const
{
  // Check type
  if (ascii) {
    // Open file
    FILE *fp = fopen(filename, "w");
    if (!fp) {
      fprintf(stderr, "Unable to open image file: %s", filename);
      return 0;
    }

    // Print PPM image file
    // First ppm pixel is top-left, so write in opposite scan-line order
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    for (int j = height-1; j >= 0 ; j--) {
      for (int i = 0; i < width; i++) {
        const R2Pixel& p = (*this)[i][j];
        int r = (int) (255 * p.Red());
        int g = (int) (255 * p.Green());
        int b = (int) (255 * p.Blue());
        fprintf(fp, "%-3d %-3d %-3d  ", r, g, b);
        if (((i+1) % 4) == 0) fprintf(fp, "\n");
      }
      if ((width % 4) != 0) fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    // Close file
    fclose(fp);
  }
  else {
    // Open file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
      fprintf(stderr, "Unable to open image file: %s", filename);
      return 0;
    }

    // Print PPM image file
    // First ppm pixel is top-left, so write in opposite scan-line order
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    for (int j = height-1; j >= 0 ; j--) {
      for (int i = 0; i < width; i++) {
        const R2Pixel& p = (*this)[i][j];
        int r = (int) (255 * p.Red());
        int g = (int) (255 * p.Green());
        int b = (int) (255 * p.Blue());
        fprintf(fp, "%c%c%c", r, g, b);
      }
    }

    // Close file
    fclose(fp);
  }

  // Return success
  return 1;
}



////////////////////////////////////////////////////////////////////////
// JPEG I/O
////////////////////////////////////////////////////////////////////////


// #define USE_JPEG
#ifdef USE_JPEG
  extern "C" {
#   define XMD_H // Otherwise, a conflict with INT32
#   undef FAR // Otherwise, a conflict with windows.h
#   include "jpeg/jpeglib.h"
  };
#endif



int R2Image::
ReadJPEG(const char *filename)
{
#ifdef USE_JPEG
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Initialize decompression info
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, fp);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  // Remember image attributes
  width = cinfo.output_width;
  height = cinfo.output_height;
  npixels = width * height;
  int ncomponents = cinfo.output_components;

  // Allocate pixels for image
  pixels = new R2Pixel [ npixels ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Allocate unsigned char buffer for reading image
  int rowsize = ncomponents * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = rowsize * height;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for JPEG file");
    fclose(fp);
    return 0;
  }

  // Read scan lines
  // First jpeg pixel is top-left, so read pixels in opposite scan-line order
  while (cinfo.output_scanline < cinfo.output_height) {
    int scanline = cinfo.output_height - cinfo.output_scanline - 1;
    unsigned char *row_pointer = &buffer[scanline * rowsize];
    jpeg_read_scanlines(&cinfo, &row_pointer, 1);
  }

  // Free everything
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  // Close file
  fclose(fp);

  // Assign pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      double r, g, b, a;
      if (ncomponents == 1) {
        r = g = b = (double) *(p++) / 255;
        a = 1;
      }
      else if (ncomponents == 1) {
        r = g = b = (double) *(p++) / 255;
        a = 1;
        p++;
      }
      else if (ncomponents == 3) {
        r = (double) *(p++) / 255;
        g = (double) *(p++) / 255;
        b = (double) *(p++) / 255;
        a = 1;
      }
      else if (ncomponents == 4) {
        r = (double) *(p++) / 255;
        g = (double) *(p++) / 255;
        b = (double) *(p++) / 255;
        a = (double) *(p++) / 255;
      }
      else {
        fprintf(stderr, "Unrecognized number of components in jpeg image: %d\n", ncomponents);
        return 0;
      }
      R2Pixel pixel(r, g, b, a);
      SetPixel(i, j, pixel);
    }
  }

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return success
  return 1;
#else
  fprintf(stderr, "JPEG not supported");
  return 0;
#endif
}




int R2Image::
WriteJPEG(const char *filename) const
{
#ifdef USE_JPEG
  // Open file
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Initialize compression info
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, fp);
  cinfo.image_width = width; 	/* image width and height, in pixels */
  cinfo.image_height = height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  cinfo.dct_method = JDCT_ISLOW;
  jpeg_set_defaults(&cinfo);
  cinfo.optimize_coding = TRUE;
  jpeg_set_quality(&cinfo, 75, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  // Allocate unsigned char buffer for reading image
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = rowsize * height;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for JPEG file");
    fclose(fp);
    return 0;
  }

  // Fill buffer with pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      const R2Pixel& pixel = (*this)[i][j];
      int r = (int) (255 * pixel.Red());
      int g = (int) (255 * pixel.Green());
      int b = (int) (255 * pixel.Blue());
      if (r > 255) r = 255;
      if (g > 255) g = 255;
      if (b > 255) b = 255;
      *(p++) = r;
      *(p++) = g;
      *(p++) = b;
    }
  }



  // Output scan lines
  // First jpeg pixel is top-left, so write in opposite scan-line order
  while (cinfo.next_scanline < cinfo.image_height) {
    int scanline = cinfo.image_height - cinfo.next_scanline - 1;
    unsigned char *row_pointer = &buffer[scanline * rowsize];
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

  // Free everything
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  // Close file
  fclose(fp);

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return number of bytes written
  return 1;
#else
  fprintf(stderr, "JPEG not supported");
  return 0;
#endif
}
