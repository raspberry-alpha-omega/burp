
burp: main.c burp.c mm57109.c
	gcc -o burp mm57109.c burp.c main.c

test: test.c burp.c mm57109.c
	gcc -o test mm57109.c burp.c test.c
	./test

clean:
	rm -f *.o burp
