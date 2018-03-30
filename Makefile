SRC_DIR := ./src
OBJ_DIR := ./obj
HDR_FILES := $(wildcard $(SRC_DIR)/*.h)
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
CPPFLAGS=-fopenmp --std=c++11 -Wall
LDFLAGS=

DEBUG=n

ifeq ($(DEBUG),n)
	CPPFLAGS+= -O3
else
	CPPFLAGS+= -O0 -g -DDEBUG
endif

main: $(OBJ_FILES)
	g++ $(CPPFLAGS) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDR_FILES )
	g++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf main ./obj/*.o
