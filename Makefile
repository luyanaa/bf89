all: bf89.o
	cc -o bf89 bf89.o -std=c89
bf89.o: bf89.c 
	cc -c bf89.c -std=c89
clean: 
	rm -rf *.o bf89
	rm -rf ./examples/*.test
	rm -rf *.TMP
check: 
	time ./bf89 ./examples/helloworld.b ./examples/helloworld.in ./examples/helloworld.test
	diff ./examples/helloworld.out ./examples/helloworld.test
	time ./bf89 ./examples/beer.b ./examples/beer.in ./examples/beer.test
	diff ./examples/beer.out ./examples/beer.test
	time ./bf89 ./examples/prime.b ./examples/prime.in ./examples/prime.test
	diff ./examples/prime.test ./examples/prime.out
	time ./bf89 ./examples/666.bf ./examples/666.in ./examples/666.test
	diff ./examples/666.out ./examples/666.test
	time ./bf89 ./examples/sierpinski.bf ./examples/sierpinski.in ./examples/sierpinski.test
	diff ./examples/sierpinski.out ./examples/sierpinski.test
	time ./bf89 ./examples/dbfi.bf ./examples/dbfi.in ./examples/dbfi.test
	diff ./examples/dbfi.test ./examples/beer.out
