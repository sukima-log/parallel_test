
main: main.o
	g++ -o main -O2 -mcmodel=medium -std=c++11 -Ofast -fopenmp main.o

main.o: main.cpp
	g++ -c -O2 -mcmodel=medium -std=c++11 -Ofast -fopenmp main.cpp

.PHONY: clean
clean:
	rm -f *.o main *.out
