all: g++

g++:
	cp src/cbase.cpp build && cd build && g++ -std=c++20 cbase.cpp -o ../test