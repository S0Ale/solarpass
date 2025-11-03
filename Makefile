CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = solarpass

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) -lsodium
