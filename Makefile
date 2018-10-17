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

# Compile: create object files from C source files.
state.o: $(SRC)/state.c
	$(CC) -c $(CFLAGS) $< -o $@

menu.o: $(SRC)/menu.c $(SRC)/menu.h $(SRC)/tetris.h $(SRC)/state.h $(UCFK)/drivers/navswitch.h $(UCFK)/utils/tinygl.h $(UCFK)/drivers/avr/ir_uart.h $(UCFK)/utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

tetris.o: $(SRC)/tetris.c $(SRC)/tetris.h $(SRC)/physics.h $(UCFK)/utils/pacer.h $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@

physics.o: $(SRC)/physics.c $(SRC)/physics.h $(SRC)/tetris.h
	$(CC) -c $(CFLAGS) $< -o $@

game.o: $(SRC)/game.c $(SRC)/game.h $(UCFK)/drivers/avr/system.h $(UCFK)/utils/pacer.h $(UCFK)/drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

graphics.o: $(SRC)/graphics.c $(SRC)/graphics.h $(SRC)/tetris.h $(SRC)/physics.h $(SRC)/game.h $(UCFK)/drivers/ledmat.h $(UCFK)/drivers/display.h $(UCFK)/utils/pacer.h $(UCFK)/drivers/avr/pio.h $(UCFK)/fonts/font5x7_1.h $(UCFK)/utils/font.h $(UCFK)/utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

randomiser.o: $(SRC)/randomiser.c $(SRC)/randomiser.h $(SRC)/tetris.h $(UCFK)/drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

comms.o: $(SRC)/comms.c $(SRC)/comms.h $(SRC)/tetris.h $(UCFK)/drivers/avr/ir_uart.h $(UCFK)/drivers/led.h $(UCFK)/utils/pacer.h
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
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: $(UCFK)/drivers/avr/ir_uart.c $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/avr/ir_uart.h $(UCFK)/drivers/avr/timer0.h $(UCFK)/drivers/avr/pio.h $(UCFK)/drivers/avr/delay.h 
	$(CC) -c $(CFLAGS) $< -o $@

#possible problem with include <avr/io.h>
usart1.o: $(UCFK)/drivers/avr/usart1.c $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: $(UCFK)/drivers/avr/timer0.c $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/avr/bits.h $(UCFK)/drivers/avr/prescale.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: $(UCFK)/drivers/avr/prescale.c $(UCFK)/drivers/avr/system.h $(UCFK)/drivers/avr/prescale.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o:  $(UCFK)/utils/tinygl.c $(UCFK)/drivers/avr/system.h  $(UCFK)/drivers/display.h  $(UCFK)/utils/font.h  $(UCFK)/utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: $(UCFK)/utils/font.c $(UCFK)/drivers/avr/system.h $(UCFK)/utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
game.out: game.o system.o timer.o tetris.o state.o menu.o physics.o randomiser.o comms.o pio.o display.o ledmat.o pacer.o navswitch.o timer.o led.o button.o ir_uart.o usart1.o timer0.o prescale.o graphics.o font.o tinygl.o
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


