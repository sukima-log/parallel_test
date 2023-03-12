
omp_test: omp_test.o
	g++ -o omp_test -O2 -mcmodel=medium -std=c++11 -Ofast -fopenmp omp_test.o

omp_test.o: omp_test.cpp
	g++ -c -O2 -mcmodel=medium -std=c++11 -Ofast -fopenmp omp_test.cpp

.PHONY: clean
clean:
	rm -f *.o omp_test *.out
