# note, tests run using the "check" library. install using "sudo apt-get install check" etc.
burp: main.c burp.c mm57109.c
	gcc -o burp mm57109.c burp.c main.c -lm

test: test.c burp.c mm57109.c
	gcc -o test mm57109.c burp.c test.c -lm -lcheck -lpthread -lrt

clean:
	rm -f *.o burp test

all: burp test
