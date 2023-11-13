CC = gcc -g
DB = gdb

SRC = $(shell find ./ -name '*.c')
SRC_O = $(SRC:.c=.o)

INC_DIRS = $(dir $(shell find . -name "*.h"))
INC = $(addprefix -I ,$(INC_DIRS))

all : main

main : main_cc
	./main.out

mdb : main_cc
	$(DB) ./main.out

main_cc: $(SRC_O)
	$(CC) $^ -o main.out

%.o: %.c
	$(CC) -c $< -o $@ $(INC)

clean :
	rm -rf *.out *.dSYM main *.o $(SRC_O)
