y86emul : index.c
	gcc -g index.c -o index
test : test.c
	gcc -Wall -g test.c -o test	
fts : fts.h
	gcc -c -g fts.h -o fts.o
	ar rcs fts.a	fts.o
clean :
	rm edit index
	rm edit test
run :
	make
	./index