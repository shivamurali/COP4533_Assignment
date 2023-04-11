CC=g++
CFLAGS=-I.

all: main.cpp
	$(CC) main.cpp -o Stocks

clean:
	rm -f Stocks
