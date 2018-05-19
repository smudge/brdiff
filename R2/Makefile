#
# Linux makefile for assignment #1
#



#
# List of files
#

CCSRCS=R2Distance.cpp \
  R2Segment.cpp R2Line.cpp R2Vector.cpp R2Point.cpp 
OBJS=$(CCSRCS:.cpp=.o)
INCS=$(HSRCS) $(CCSRCS:.cpp=.h)



#
# Options
#

CC=g++
CPPFLAGS=-Wall -g -I. -DUSE_JPEG 
LDFLAGS=-g



#
# Output library name
#

LIB=./libR2.a



#
# Make targets
#

$(LIB): $(OBJS)
	    - rm -f $(LIB)
	    ar ur $(LIB) $(OBJS) $(USER_OBJS)

clean:
	    -  rm -f *~ *.o *.obj *.pdb $(LIB)

