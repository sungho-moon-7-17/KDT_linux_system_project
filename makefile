CC = gcc -g
CPP = g++ -g -std=c++17
DB = gdb

SRC = $(shell find ./ -name '*.c')
SRC_O = $(SRC:.c=.o)

SRC_CPP = $(shell find ./ -name '*.cpp')
SRC_O_CPP = $(SRC_CPP:.cpp=.o)

INC_DIRS = $(dir $(shell find . -name "*.h"))
INC = $(addprefix -I ,$(INC_DIRS))

ELF = toy_system

all : main

main : main_cc
	./$(ELF)

mdb : main_cc
	$(DB) ./$(ELF)

main_cc: $(SRC_O) $(SRC_O_CPP)
	$(CPP) $^ -o $(ELF)

%.o: %.c
	$(CC) -c $< -o $@ $(INC)

%.o: %.cpp
	$(CPP) -c $< -o $@ $(INC)

clean :
	rm -rf *.out *.dSYM main *.o $(SRC_O) filebrowser.db $(ELF)
