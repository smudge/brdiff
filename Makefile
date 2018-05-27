RBDIFF_SRCS=rbdiff.cpp R2Image.cpp R2Pixel.cpp
RBDIFF_OBJS=$(RBDIFF_SRCS:.cpp=.o)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

all: rbdiff

rbdiff: $(LIB) $(RBDIFF_OBJS)
	g++ -ljpeg -o rbdiff -Wall -I. -g $(RBDIFF_OBJS) -lm

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete
	rm -f rbdiff
