# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
SRC = ./src
UCFK = ./lib
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I$(SRC) -I $(UCFK)/utils -I $(UCFK)/fonts -I $(UCFK)/drivers -I $(UCFK)/drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out

test:
	make -f Makefile.test all


# Compile: create object files from C source files.
state.o: $(SRC)/state.c
	$(CC) -c $(CFLAGS) $< -o $@

menu.o: $(SRC)/menu.c
	$(CC) -c $(CFLAGS) $< -o $@

tetris.o: $(SRC)/tetris.c $(UCFK)/utils/pacer.h $(UCFK)/drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

physics.o: $(SRC)/physics.c
	$(CC) -c $(CFLAGS) $< -o $@

game.o: $(SRC)/game.c $(UCFK)/drivers/avr/system.h $(UCFK)/utils/pacer.h $(UCFK)/drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

showScreen.o: $(SRC)/showScreen.c $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/ledmat.h $(UCFK)/drivers/display.h $(UCFK)/utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@


# Drivers/System stuff:
timer.o: $(UCFK)/drivers/avr/timer.c $(UCFK)/drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: $(UCFK)/drivers/avr/system.c $(UCFK)/drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: $(UCFK)/drivers/ledmat.c $(UCFK)/drivers/avr/pio.h $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: $(UCFK)/drivers/display.c $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/display.h $(UCFK)/drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: $(UCFK)/drivers/avr/pio.c $(UCFK)/drivers/avr/pio.h $(UCFK)/drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: $(UCFK)/utils/pacer.c $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/avr/timer.h $(UCFK)/utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@


button.o: $(UCFK)/drivers/button.c $(UCFK)/drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: $(UCFK)/drivers/led.c $(UCFK)/drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: $(UCFK)/drivers/navswitch.c $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/navswitch.h $(UCFK)/drivers/avr/delay.h $(UCFK)/drivers/avr/pio.h 
	$(CC) -c $(CFLAGS) $< -o $@=




# Link: create ELF output file from object files.
game.out: game.o system.o timer.o tetris.o state.o menu.o physics.o showScreen.o pio.o display.o ledmat.o pacer.o navswitch.o timer.o led.o button.o
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


