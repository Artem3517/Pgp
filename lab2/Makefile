EXE = lab2
FLAGS = -Werror -Wall -Wextra
OBJ = geometry.o display.o control.o main.o

all: $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(EXE) -lX11

main.o:	main.c graph.h
	gcc $(FLAGS) -c main.c -o main.o -lX11

geometry.o:	geometry.c graph.h
	gcc $(FLAGS) -c geometry.c -o geometry.o -lX11

display.o: display.c graph.h
	gcc $(FLAGS) -c display.c -o display.o -lX11

control.o: control.c graph.h
	gcc $(FLAGS) -c control.c -o control.o -lX11

clean:
	rm -f $(EXE) $(OBJ)

rebuild: clean all