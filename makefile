bin/simulator : src/*.cpp src/headers/*.hpp
	mkdir -p bin
	g++ -g -Wall -std=c++11 src/*.cpp -o bin/simulator
simulator : bin/simulator

bin/mips_testbench: src/testbench/*.cpp
	mkdir -p bin
	g++ -g -Wall -std=c++11 src/testbench/*.cpp -o bin/mip_testbench
testbench: bin/mips_testbench
