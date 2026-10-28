CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -Ithird_party -O2 -Wall

SRC = $(wildcard src/*.cpp)
OUT = langstat

all:
	$(CXX) $(SRC) $(CXXFLAGS) -o $(OUT)

clean:
	rm $(OUT)
