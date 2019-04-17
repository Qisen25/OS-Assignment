EXEC = scheduler
UNITTEST = qTest
OBJTEST = UnitTestQueue.o Queue.o
OBJ = scheduler.o
CFLAGS = -Wall -g
CC = gcc

all : $(UNITTEST)

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

$(UNITTEST) : $(OBJTEST)
	$(CC) $(OBJTEST) -o $(UNITTEST) $(CFLAGS)

scheduler.o : scheduler.c scheduler.h
	$(CC) -c scheduler.c $(CFLAGS)

Queue.o : Queue.c Queue.h
	$(CC) -c Queue.c $(CFLAGS)

UnitTestQueue.o : UnitTestQueue.c Queue.h
	$(CC) -c UnitTestQueue.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ) $(OBJTEST) $(UNITTEST)
