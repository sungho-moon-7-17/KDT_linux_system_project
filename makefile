CC = gcc -g
DB = gdb

SRC = $(shell find ./ -name '*.c')
SRC_O = $(SRC:.c=.o)

INC_DIRS = $(dir $(shell find . -name "*.h"))
INC = $(addprefix -I ,$(INC_DIRS))

ELF = toy_system

all : main

main : main_cc
	./$(ELF)

mdb : main_cc
	$(DB) ./$(ELF)

main_cc: $(SRC_O)
	$(CC) $^ -o $(ELF)

%.o: %.c
	$(CC) -c $< -o $@ $(INC)

clean :
	rm -rf *.out *.dSYM main *.o $(SRC_O) filebrowser.db $(ELF)
