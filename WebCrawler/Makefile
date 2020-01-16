CC = gcc
CFLAGS = -g -Wall -Werror
OBJS = crawler.o indexPage.o

all: webSearch
webSearch: webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@ -lm

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ webSearch