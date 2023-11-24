all: compile run

compile:
	gcc -c src/*.c
	gcc -o tictactoe game_engine.o server.o client.o main.o
	rm *.o

run: 	compile
	./tictactoe

debug:
	gcc -g src/*.c
	gdb a.out

clear:
	rm *.o a.out tictactoe
