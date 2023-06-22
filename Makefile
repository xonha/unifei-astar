all: 
	@gcc *.c -Wall -pedantic-errors -o programa -lm

run:
	./programa
