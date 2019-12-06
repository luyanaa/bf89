all: bf89.o
	cc -o bf89 bf89.o -std=c89
bf89.o: bf89.c 
	cc -c bf89.c -std=c89
clean: 
	rm -rf *.o bf89
	rm -rf ./examples/*.test
check: 
	./bf89 ./examples/helloworld.b ./examples/helloworld.in ./examples/helloworld.test
	diff ./examples/helloworld.out ./examples/helloworld.test
	./bf89 ./examples/counter.b ./examples/counter.in ./examples/counter.test
	diff ./examples/counter.out ./examples/counter.test
