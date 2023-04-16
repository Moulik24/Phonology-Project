CC := gcc
CFLAGS := -Wall -Werror

main: main.c
	$(CC) -o main main.c -lfoma $(CFLAGS)