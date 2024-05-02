CXX = $(NIX_CC)/bin/g++
CXXFLAGS = -Iinclude -std=c++20 -Wall -Wextra -g -O0 $(NIX_CFLAGS_COMPILE)
LDFLAGS = $(NIX_LDFLAGS_FOR_TARGET) -lcurl -lgit2


StRCS = $(wildcard src/*.cpp)
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

format:
	find . -regex ".*\.\(cpp\|h\)" -exec clang-format -i {} +

