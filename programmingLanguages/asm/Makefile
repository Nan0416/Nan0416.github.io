all:main.flash.hex
	avrdude -p m328p -c arduino -b 57600 -P /dev/ttyUSB0 -U flash:w:main.flash.hex
main.flash.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.flash.hex
main.elf: main.o 
	avr-g++ -std=c++11 -mmcu=atmega328p -o main.elf main.o
main.o: main.s
	avr-as -c -o main.o -mmcu=atmega328p main.s
main.s: main.c
	avr-gcc  -mmcu=atmega328p -S  main.c
