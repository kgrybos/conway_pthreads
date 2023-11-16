CC = gcc
CFLAGS = -Wall

all: life
	
life: grid.c life.c worker.c
	$(CC) $(CFLAGS) grid.c life.c worker.c -o life -lncursesw -pthread
	
clean:
	rm -f life
