TARGET = Network.o main

all: ${TARGET}

main: main.cpp Network.o
	g++ -o main main.cpp Network.o

Network.o: Network.cpp  Network.hh
	g++ -c Network.cpp

clean:
	rm -f *.o
	rm main
	rm graphInstance.txt
	rm graph.png