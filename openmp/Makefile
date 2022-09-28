a.out : main.o
	gcc -o a.out main.o -fopenmp

main.o : main.c
	gcc -c -o main.o main.c -fopenmp

clean :
	rm *.o a.out
