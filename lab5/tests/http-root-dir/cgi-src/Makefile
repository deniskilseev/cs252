# For gcc
CC= gcc
# For ANSI compilers
#CC= cc

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -fPIC -g

RM= /bin/rm -f
#--- You shouldn't have to edit anything else. ---

.c.o: 
	$(CC) -c $(CFLAGS) $<

all: env jj hello.so jj-mod.so date.so

jj: jj.o util.o
	$(CC) $(CFLAGS) jj.o util.o -o ../cgi-bin/jj

hello.so: hello.c
	$(CC) -c -fPIC $^
	ld -G -o ../cgi-bin/hello.so hello.o

jj-mod.so: jj-mod.c util.c
	$(CC) -c -fPIC $^
	ld -G -o ../cgi-bin/jj-mod.so jj-mod.o util.o

date.so: date-mod.c
	$(CC) -c -fPIC $^
	ld -G -o ../cgi-bin/date.so date-mod.o

env: env.o
	$(CC) $(CFLAGS) env.o -o ../cgi-bin/env

clean:
	rm -f *.o ../cgi-bin/jj
