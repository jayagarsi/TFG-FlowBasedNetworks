TARGET = Network.o BaseFlowGame.o main dynamics

all: ${TARGET}

main: main.cpp Network.o BaseFlowGame.o
	g++ -o main main.cpp Network.o BaseFlowGame.o

dynamics: dynamics.cpp Network.o BaseFlowGame.o
	g++ -o dynamics dynamics.cpp Network.o BaseFlowGame.o

Network.o: Network.cpp  Network.hh
	g++ -c Network.cpp

BaseFlowGame.o: BaseFlowGame.cpp BaseFlowGame.hh Network.hh
	g++ -c BaseFlowGame.cpp

clearSpace:
	rm -f *.txt *.png

clean:
	rm -f *.o
	rm -rf __pycache__
	rm -f main dynamics *.txt *.png *.csv echo