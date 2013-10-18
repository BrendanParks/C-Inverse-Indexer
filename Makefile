y86emul : index.c
	gcc -Wall -ansi -g index.c -o index
test : test.c
	gcc -Wall -ansi -g test.c -o test	
clean :
	rm edit index
	rm edit test
run :
	make
	./index