CXX := g++
CXXFLAGS := -Wall -Weffc++ -pedantic -g
CXXFLAGS += -MD -MP
LDFLAGS := -ljpeg -ltiff
SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: brdiff

brdiff: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	find . -type f \( -name '*.o' -o -name '*.a' -o -name '*.d' \) -delete
	rm -f brdiff

-include $(OBJ_FILES:%.o=%.d)
