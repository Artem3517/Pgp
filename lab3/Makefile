FLAGS = -Werror -Wall -Wextra
XLIB = -l X11
EXE = lab3

all: main.o window.o drawing.o
	gcc main.o window.o drawing.o -o $(EXE) $(XLIB)

main.o: main.c
	gcc $(FLAGS) -c main.c -o main.o $(XLIB)

window.o: window.c
	gcc $(FLAGS) -c window.c -o window.o $(XLIB)

drawing.o: drawing.c
	gcc $(FLAGS) -c drawing.c -o drawing.o $(XLIB)

clean:
	rm -f *.o $(EXE)

rebuild: cllean all