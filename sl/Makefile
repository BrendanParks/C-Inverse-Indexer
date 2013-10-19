COMPILER = gcc
CCFLAGS = -Wall -g

all: sl
sorted-list.o: sorted-list.c sorted-list.h
	${COMPILER}	${CCFLAGS}	-c sorted-list.c
main.o: main.c sorted-list.h
	${COMPILER}	${CCFLAGS}	-c main.c
libsl.a: sorted-list.o 
	ar rvs libsl.a sorted-list.o
sl: main.o libsl.a
	${COMPILER}	${CCFLAGS} -o sl main.o libsl.a
clean:
	rm -f s1
	rm -f *.o