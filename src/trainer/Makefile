# Makefile for g2p_trainer

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET = g2p_trainer

all: $(TARGET)

$(TARGET): g2p_trainer.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $<

g2p_rules.h: $(TARGET)
	./$< cmudict-0.7b

clean:
	rm -f $(TARGET) g2p_rules.h

.PHONY: all clean

