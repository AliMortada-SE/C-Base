all:g++ remove run

cpp = src/*.cpp ../ProjectP/src/room.cpp ../ProjectP/src/utils.cpp
header = -Iinclude -I../ProjectP/include
flag = -std=c++20
g++:
	g++ $(flag) $(header) $(cpp)  -o test -lpthread

run:
	./test
remove:
	rm  school/*