RBDIFF_SRCS=rbdiff.cpp R2Image.cpp R2Pixel.cpp
RBDIFF_OBJS=$(RBDIFF_SRCS:.cpp=.o)
CC=g++
CPPFLAGS=-Wall -I. -Ijpeg/linux-src -g -DUSE_JPEG
LDFLAGS=-g
LIBS=R2/libR2.a jpeg/libjpeg.a

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

all: $(LIBS) rbdiff

rbdiff: $(LIB) $(RBDIFF_OBJS)
	$(CC) -o rbdiff $(CPPFLAGS) $(LDFLAGS) $(RBDIFF_OBJS) $(LIBS) -lm

R2/libR2.a:
	cd R2; make

jpeg/libjpeg.a:
	cd jpeg; make

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete
	rm -f rbdiff
