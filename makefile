CC=gcc
CFLAGS = -Wall -Werror -O0 -g

myshell: myshell.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f myshell
