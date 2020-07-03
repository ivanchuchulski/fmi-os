ifndef CC
	CC=gcc
endif
CFLAGS=-std=c11 -Werror -Wall -Wpedantic -Wextra -pthread
SRCS=main.c
OBJS=$(subst .c,.o,$(SRCS))
RM=rm -f

foo: main.o
	$(CC) $(CFLAGS) -o main main.c 

clean:
	$(RM) $(OBJS) main wrong wrong.o

bar: wrong.o
	$(CC) $(CFLAGS) -o wrong wrong.c 


