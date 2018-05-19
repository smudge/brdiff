#
# Unix/Linux makefile for assignment #1
#



#
# List of source files
#

IMGPRO_SRCS=imgpro.cpp R2Image.cpp R2Pixel.cpp
IMGPRO_OBJS=$(IMGPRO_SRCS:.cpp=.o)



#
# Compile and link options
#

CC=g++
CPPFLAGS=-Wall -I. -Ijpeg/linux-src -g -DUSE_JPEG
LDFLAGS=-g



#
# Libraries
#

LIBS=R2/libR2.a jpeg/libjpeg.a



#
# Compile command
#

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@



#
# Make targets
#

all: $(LIBS) imgpro

imgpro: $(LIB) $(IMGPRO_OBJS)
	$(CC) -o imgpro $(CPPFLAGS) $(LDFLAGS) $(IMGPRO_OBJS) $(LIBS) -lm

R2/libR2.a:
	cd R2; make

jpeg/libjpeg.a:
	cd jpeg; make

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete
	rm -f imgpro
