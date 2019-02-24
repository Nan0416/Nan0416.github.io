all:main.flash.hex
	avrdude -p m328p -c arduino -b 57600 -P /dev/ttyUSB0 -U flash:w:main.flash.hex
main.flash.hex: main.o
	avr-objcopy -j .text -j .data -O ihex main.o main.flash.hex
main.o: test.o
	avr-ld -o main.o test.o
test.o: test.s
	avr-as -mmcu=atmega328p -c -o test.o  test.s
