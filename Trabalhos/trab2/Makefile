all: compile run

compile:
	gcc -c ./src/*.c -Wall -Werror
	gcc -o prog *.o 
	rm *.o

run:
	./prog

debug: 
	gcc -g ./src/*c
	gdb a.out
	rm a.out
clear:
	rm *.o prog a.out
