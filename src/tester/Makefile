# Makefile for g2p_tester

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET = g2p_tester

all: $(TARGET)

$(TARGET): g2p_tester.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) g2p_tester.cpp

clean:
	rm -f $(TARGET)

.PHONY: all clean
