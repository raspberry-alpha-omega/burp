
burp: main.c burp.c mm57109.c
	gcc -o burp mm57109.c burp.c main.c


clean:
	rm -f *.o burp
