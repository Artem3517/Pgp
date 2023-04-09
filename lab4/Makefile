FLAGS = -Werror -Wall -Wextra
LIB = -lX11 -lm
EXE = lab4

all: main.o window.o drawing.o
	gcc main.o window.o drawing.o -o $(EXE) $(LIB)
main.o: main.c
	gcc $(FLAGS) -c main.c -o main.o $(LIB)

window.o: window.c
	gcc $(FLAGS) -c window.c -o window.o $(LIB)

drawing.o: drawing.c
	gcc $(FLAGS) -c drawing.c -o drawing.o $(LIB)

clean:
	rm -f *.o $(EXE)

rebuild: clean all
