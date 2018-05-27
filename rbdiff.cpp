#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Pixel.h"
#include "Image.h"


static void
ShowUsage(void)
{
  // Print usage message and exit
  fprintf(stderr, "Usage: rbdiff input_image_1 input_image_2 output_image\n");
  exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
  // Look for help
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-help")) {
      ShowUsage();
    }
  }

  // Read input and output image filenames
  if (argc < 3) ShowUsage();
  argv++, argc--; // First argument is program name
  char *input_image_1_name = *argv; argv++, argc--;
  char *input_image_2_name = *argv; argv++, argc--;
  char *output_image_name  = *argv; argv++, argc--;

  // Allocate image
  Image *image1 = new Image();

  if (!image1) {
    fprintf(stderr, "Unable to allocate image\n");
    exit(-1);
  }

  // Read input image
  if (!image1->Read(input_image_1_name)) {
    fprintf(stderr, "Unable to read image from %s\n", input_image_1_name);
    exit(-1);
  }

  // Allocate image
  Image *image2 = new Image();

  if (!image2) {
    fprintf(stderr, "Unable to allocate image\n");
    exit(-1);
  }

  // Read input image
  if (!image2->Read(input_image_2_name)) {
    fprintf(stderr, "Unable to read image from %s\n", input_image_2_name);
    exit(-1);
  }

  image1->Subtract(*image2);

  // Write output image
  if (!image1->Write(output_image_name)) {
    fprintf(stderr, "Unable to read image from %s\n", output_image_name);
    exit(-1);
  }

  // Delete image
  delete image1;
  delete image2;

  // Return success
  return EXIT_SUCCESS;
}
