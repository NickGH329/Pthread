run: main.o cell.o 2darray.o init.o io.o barrier.o
	gcc -lpthread -o run main.o barrier.o cell.o 2darray.o init.o io.o
main.o: main.c RBcomputation.h
	gcc -c main.c
barrier.o: barrier.c
	gcc -c barrier.c
2darray.o: 2darray.c
	gcc -c 2darray.c
cell.o: cell.c
	gcc -c cell.c
init.o: init.c
	gcc -c init.c
io.o: io.c
	gcc -c io.c
clean: 
	rm run main.o 2darray.o cell.o init.o io.o barrier.o