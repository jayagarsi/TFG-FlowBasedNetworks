TARGET = Network.o main dynamics

all: ${TARGET}

main: main.cpp Network.o
	g++ -o main main.cpp Network.o

dynamics: dynamics.cpp Network.o
	g++ -o dynamics dynamics.cpp Network.o

Network.o: Network.cpp  Network.hh
	g++ -c Network.cpp

clearSpace:
	rm -f *.txt *.png

clean:
	rm -f *.o
	rm -rf __pycache__
	rm -f main dynamics *.txt *.png *.csv echo