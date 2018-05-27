CXX := g++
CXXFLAGS := -Wall -Weffc++ -pedantic -g
CXXFLAGS += -MD -MP
LDFLAGS := -ljpeg
RBDIFF_SRCS := $(wildcard *.cpp)
RBDIFF_OBJS := $(RBDIFF_SRCS:.cpp=.o)

all: rbdiff

rbdiff: $(RBDIFF_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	find . -type f \( -name '*.o' -o -name '*.a' -o -name '*.d' \) -delete
	rm -f rbdiff

-include $(SRC:%.c=%.d)
