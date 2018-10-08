# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../utils -I../fonts -I../drivers -I../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
state.o: state.c
	$(CC) -c $(CFLAGS) $< -o $@

menu.o: menu.c
	$(CC) -c $(CFLAGS) $< -o $@

tetris.o: tetris.c
	$(CC) -c $(CFLAGS) $< -o $@

pieceRandomiser.o: pieceRandomiser.c
	$(CC) -c $(CFLAGS) $< -o $@

physics.o: physics.c
	$(CC) -c $(CFLAGS) $< -o $@

game.o: game.c ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

showScreen.o: showScreen.c
	$(CC) -c $(CFLAGS) $< -o $@


# Drivers/System stuff:
timer.o: ../drivers/avr/timer.c ../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../drivers/avr/system.c ../drivers/avr/system.h
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


