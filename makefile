CC = g++

CFLAGS = -g -Wall


TARGET = father


all: $(TARGET)

$(TARGET) : father.o registers.o mem.o pc.o
	$(CC) $(CFLAGS) -o father father.o registers.o mem.o pc.o

father.o : src/father.cpp src/headers/mem.hpp src/headers/pc.hpp src/headers/error_check.hpp
	$(CC) $(CFLAGS) -c src/father.cpp

registers.o : src/registers.cpp src/headers/registers.hpp
	$(CC) $(CFLAGS) -c src/registers.cpp

mem.o : src/mem.cpp src/headers/mem.hpp
	$(CC) $(CFLAGS) -c src/mem.cpp

pc.o : src/pc.cpp headers/pc.hpp
	$(CC) $(CFLAGS) -c src/pc.cpp

clean:
	$(RM) father *.o*~
