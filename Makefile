SRC_DIR := ./src
OBJ_DIR := ./obj
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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f main ./obj/*.o
