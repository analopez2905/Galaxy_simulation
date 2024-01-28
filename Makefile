
CC = gcc
CFLAGS = -Wall
LFLAGS = -lm #banderas de ligadura
OBJS = main.o allvars.o funciones.o
EXEC = simulador_n_cuerpos.out

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(EXEC) #las LFLAGS siempre deben ir antes del -o
	rm $(OBJS)

clean:
	rm *~ *.out
