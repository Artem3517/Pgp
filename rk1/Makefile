EXE = rk1
FLAGS = -Werror -Wall -Wextra
OBJ = graphics.o control.o calculate.o main.o

all: $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(EXE) -lX11

main.o:	main.c
	gcc $(FLAGS) -c main.c -o main.o -lX11

graphics.o:	graphics.c
	gcc $(FLAGS) -c graphics.c -o graphics.o -lX11

control.o: control.c
	gcc $(FLAGS) -c control.c -o control.o -lX11

calculate.o: calculate.c
	gcc $(FLAGS) -c calculate.c -o calculate.o -lX11

clean:
	rm -f $(EXE) $(OBJ)

rebuild: clean all