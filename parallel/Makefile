FLAGS=-O3

CC=mpicc

RM=rm -f

EXEC=tsp

all: $(EXEC)

$(EXEC):
	$(CC) $(FLAGS) $(EXEC).c -c -o $(EXEC).o -lm
	$(CC) $(FLAGS) $(EXEC).o -o $(EXEC) -lm

run:
	./$(EXEC)

clean:
	$(RM) $(EXEC).o $(EXEC)
