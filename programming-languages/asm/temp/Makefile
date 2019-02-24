all:main.flash.hex
	avrdude -p m328p -c arduino -b 57600 -P /dev/ttyUSB0 -U flash:w:main.flash.hex
main.flash.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.flash.hex
main.elf: main.o
	avr-gcc -std=c++11 -mmcu=atmega328p -o main.elf main.o
main.o: main.c
	avr-gcc -O1 -Wall -mmcu=atmega328p -c -o main.o  main.c
