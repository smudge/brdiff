#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Pixel.h"
#include "Image.h"


static void
ShowUsage(void)
{
  fprintf(stderr, "usage:\n  brdiff [--help] <base_image> <subtraction_image> <difference_image>\n");
  exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "--help")) {
      ShowUsage();
    }
  }

  if (argc < 3) ShowUsage();
  argv++, argc--;
  char *input_image_1_name = *argv; argv++, argc--;
  char *input_image_2_name = *argv; argv++, argc--;
  char *output_image_name  = *argv; argv++, argc--;

  Image *image1 = new Image();

  if (!image1) {
    fprintf(stderr, "Unable to allocate image\n");
    exit(-1);
  }

  if (!image1->Read(input_image_1_name)) {
    fprintf(stderr, "Unable to read image from %s\n", input_image_1_name);
    exit(-1);
  }

  Image *image2 = new Image();

  if (!image2) {
    fprintf(stderr, "Unable to allocate image\n");
    exit(-1);
  }

  if (!image2->Read(input_image_2_name)) {
    fprintf(stderr, "Unable to read image from %s\n", input_image_2_name);
    exit(-1);
  }

  image1->Subtract(*image2);

  if (!image1->Write(output_image_name)) {
    fprintf(stderr, "Unable to read image from %s\n", output_image_name);
    exit(-1);
  }

  delete image1;
  delete image2;

  return EXIT_SUCCESS;
}
