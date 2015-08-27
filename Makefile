# MAKE file for POS entropy analysis
# Author: Yachen Sun

CC = gcc
CFLAGS = -Wall -pedantic -std=c11

EXEC = entropy
OBJS = obj/entropy.o obj/hashtable.o
SRCS = src/entropy.c src/hashtable.c src/hashtable.h src/utils.h

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)
	mkdir obj
	mv entropy.o obj/
	mv hashtable.o obj/
debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -DDEBUG -c $(SRCS)
	mkdir obj
	mv entropy.o obj/
	mv hashtable.o obj/
	$(CC) $(CFLAGS) -g -ggdb -DDEBUG -o $(EXEC) $(OBJS)
auto:
	chmod 700 ent.sh
	./ent.sh
clean:
	rm -f *~
	rm -f *#
	rm -f ./entropy
	rm -rf obj/
	rm -f core*
	cd src; rm -f *.gch