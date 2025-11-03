all: solarpass

solarpass: main.cpp
	g++ -Wall main.cpp -o solarpass
