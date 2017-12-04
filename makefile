bin/testbench : src/*.cpp src/headers/*.hpp
	mkdir -p bin
	g++ -g -Wall -std=c++11 src/*.cpp -o bin/simulator