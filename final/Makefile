# 	@file   player.h
#   @author F. van Dorsser
#   @author K. Moore
#   @date   12 October 2021
#   @brief  Implements player character movements

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: pacer.c pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@	

display.o: ../../drivers/display.c ../../drivers/display.h
	$(CC) -c $(CFLAGS) $< -o $@

player.o: player.c player.h
	$(CC) -c $(CFLAGS) $< -o $@

obstacle.o: obstacle.c ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../utils/tinygl.h ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../utils/font.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

uint8toa.o: ../../utils/uint8toa.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@


	



# Link: create ELF output file from object files.
game.out: game.o system.o pio.o navswitch.o pacer.o display.o ledmat.o player.o obstacle.o tinygl.o font.o uint8toa.o
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


