all:l.o
	avr-objdump -S l.o
l.o: main.o 
	avr-ld -o l.o main.o
main.o: main.s
	avr-as -c -o main.o -mmcu=atmega328p main.s
main.s: main.c
	avr-gcc  -mmcu=atmega328p -S  main.c
