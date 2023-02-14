TARGET = Network.o main

all: ${TARGET}

main: main.cpp Network.o
	g++ -o main main.cpp Network.o

Network.o: Network.cpp  Network.hh
	g++ -c Network.cpp

clean:
	rm -f *.o
	rm -rf __pycache__
	rm -f main graphInstance.txt graph.png