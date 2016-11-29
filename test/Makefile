
CC = gcc

headertest: headertest.o
	$(CC) headertest.o -o headertest

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f headertest *.o

