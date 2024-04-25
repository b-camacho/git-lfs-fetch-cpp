CXX = g++
CXXFLAGS = -Iinclude -std=c++20 -Wall -Wextra -O2 -I${NIX_STORE}/nlohmann_json/include
LDFLAGS = -lcurl

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = libgitlfsfetch.a

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) test/main

test: $(TARGET)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o test/main test/main.cpp $(TARGET)
	./test/main


