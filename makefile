SRC = $(shell find ./ -name '*.c')
HEAD = ./include
SRC_O = $(SRC:.c=.o)
TEMP_SRC = $(shell find . -name 'temp*.c')

CC = gcc -g
DB = gdb

all : main

main : main_cc
	./main.out

mdb : main_cc
	$(DB) ./main.out

main_cc: $(SRC_O)
	$(CC) $^ -o main.out -I $(HEAD)

%.o: %.c
	$(CC) -c $< -o $@

clean :
	rm -rf *.out *.dSYM main *.o $(SRC_O)
