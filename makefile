bin/simulator : src/*.cpp src/headers/*.hpp
	g++ src/*.cpp -Wall -o bin/simulator
