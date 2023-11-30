all: compile run

compile:
	gcc -c ./src/*.c -Wall -Werror
	gcc -o prog *.o 
	rm *.o

run:
	./prog
