a.out : main.o
	g++ -o a.out main.o -fopenmp

main.o : main.cpp
	g++ -c -o main.o main.cpp -fopenmp

clean :
	rm *.o a.out
