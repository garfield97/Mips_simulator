CC = g++

CFLAGS = -g -Wall


TARGET = bin/father


all: $(TARGET)

$(TARGET) : src/father.o src/registers.o src/mem.o src/pc.o
	$(CC) $(CFLAGS) -o bin/father src/father.o src/registers.o src/mem.o src/pc.o

src/father.o : src/father.cpp src/headers/mem.hpp src/headers/pc.hpp src/headers/error_check.hpp
	$(CC) $(CFLAGS) -c src/father.cpp

src/registers.o : src/registers.cpp src/headers/registers.hpp
	$(CC) $(CFLAGS) -c src/registers.cpp

src/mem.o : src/mem.cpp src/headers/mem.hpp
	$(CC) $(CFLAGS) -c src/mem.cpp

src/pc.o : src/pc.cpp src/headers/pc.hpp
	$(CC) $(CFLAGS) -c src/pc.cpp

clean:
	$(RM) bin/father src/*.o*~
