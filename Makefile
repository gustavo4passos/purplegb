CFLAGS = -std=c++14

all: purplegb
	g++ src/main.cpp build/purplegb.o -o bin/purplegb $(CFLAGS)

purplegb: src/core/purplegb.h src/core/purplegb.cpp
	g++ src/core/purplegb.cpp -c -o build/purplegb.o

clean:
	rm build/*.o 
