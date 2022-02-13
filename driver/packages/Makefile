
SRC=pcre.c develop.c parser.c contrib.c sockets.c 
OBJ=pcre.$(O) develop.$(O) parser.$(O) contrib.$(O) sockets.$(O) 

.c.$(O):
	$(CC) $(CFLAGS) -I.. -c $*.c

all: $(OBJ)
	$(AR) rcu packages.$(A) $(OBJ)
	$(RANLIB) packages.$(A)

db.$(O): db.c
	$(CC) $(CFLAGS) -I.. -I/usr/include/mysql -c $*.c

clean:
	rm -f *.$(O)
	rm -f packages.$(A)
