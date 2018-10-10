# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
SRC = ./src
UCFK = ..
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I$(SRC) -I $(UCFK)/utils -I $(UCFK)/fonts -I $(UCFK)/drivers -I $(UCFK)/drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
state.o: $(SRC)/state.c
	$(CC) -c $(CFLAGS) $< -o $@

menu.o: $(SRC)/menu.c
	$(CC) -c $(CFLAGS) $< -o $@

tetris.o: $(SRC)/tetris.c
	$(CC) -c $(CFLAGS) $< -o $@

pieceRandomiser.o: $(SRC)/pieceRandomiser.c
	$(CC) -c $(CFLAGS) $< -o $@

physics.o: $(SRC)/physics.c
	$(CC) -c $(CFLAGS) $< -o $@

game.o: $(SRC)/game.c $(UCFK)/drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

showScreen.o: $(SRC)/showScreen.c
	$(CC) -c $(CFLAGS) $< -o $@


# Drivers/System stuff:
timer.o: $(UCFK)/drivers/avr/timer.c $(UCFK)/drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: $(UCFK)/drivers/avr/system.c $(UCFK)/drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@



# Link: create ELF output file from object files.
game.out: game.o system.o timer.o tetris.o state.o pieceRandomiser.o menu.o physics.o showScreen.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


