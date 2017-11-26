CC = g++

CFLAGS = -g -Wall


TARGET = father


all: $(TARGET)

$(TARGET) : father.o registers.o mem.o pc.o
	$(CC) $(CFLAGS) -o father father.o registers.o mem.o pc.o

father.o : father.cpp headers/mem.hpp headers/pc.hpp headers/error_check.hpp
	$(CC) $(CFLAGS) -c father.cpp

registers.o : registers.cpp headers/registers.hpp
	$(CC) $(CFLAGS) -c registers.cpp

mem.o : mem.cpp headers/mem.hpp
	$(CC) $(CFLAGS) -c mem.cpp

pc.o : pc.cpp headers/pc.hpp
	$(CC) $(CFLAGS) -c pc.cpp

clean: 
	$(RM) father *.o*~