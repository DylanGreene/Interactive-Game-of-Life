CC = gcc
MAIN = life
EXEC = $(MAIN)
FLAGS = -lm -lX11

all: $(MAIN).c gfx_custom.c
	$(CC) $(MAIN).c gfx_custom.c $(FLAGS) -o $(EXEC)

clean:
	rm $(EXEC)

