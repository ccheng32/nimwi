SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))
CPPFLAGS=-fopenmp --std=c++11 -Wall
LDFLAGS=



main: $(OBJ_FILES)
	g++ $(CPPFLAGS) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f main *.o
