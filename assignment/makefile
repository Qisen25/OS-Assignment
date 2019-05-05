EXEC = scheduler
UNITTEST = qUnitTest
OBJTEST = UnitTestQueue.o Queue.o
OBJ = scheduler.o fileIO.o Queue.o
CFLAGS = -Wall -g -lpthread
CC = gcc

all : $(UNITTEST) $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

$(UNITTEST) : $(OBJTEST)
	$(CC) $(OBJTEST) -o $(UNITTEST) $(CFLAGS)

scheduler.o : scheduler.c scheduler.h fileIO.h Queue.h
	$(CC) -c scheduler.c $(CFLAGS)

Queue.o : Queue.c Queue.h
	$(CC) -c Queue.c $(CFLAGS)

fileIO.o : fileIO.c fileIO.h scheduler.h
	$(CC) -c fileIO.c $(CFLAGS)

UnitTestQueue.o : UnitTestQueue.c Queue.h
	$(CC) -c UnitTestQueue.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ) $(OBJTEST) $(UNITTEST)
