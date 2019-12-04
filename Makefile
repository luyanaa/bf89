all: bf89.o
	cc -o bf89 bf89.o
bf89.o: bf89.c 
	cc -c bf89.c
