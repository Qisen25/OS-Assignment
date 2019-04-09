EXEC = scheduler
OBJ = scheduler.o
CFLAGS = -Wall -g
CC = gcc

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

scheduler.o : scheduler.c scheduler.h
	$(CC) -c scheduler.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ)
