bin/mips_simulator : src/simulator.cpp src/mem.cpp src/registers.cpp src/pc.cpp src/headers/*.hpp
	mkdir -p bin
	g++ -g -Wall -std=c++11 src/simulator.cpp src/registers.cpp  src/mem.cpp src/pc.cpp -o bin/mips_simulator
simulator : bin/mips_simulator

bin/mips_testbench: src/testbench.cpp
	mkdir -p bin
	g++ -g -Wall -std=c++11 src/testbench.cpp -o bin/mips_testbench
testbench: bin/mips_testbench
