#define F_CPU 16000000UL
#include <avr/io.h>
int main(){
	DDRB |= 0x20;
	PORTB |= 0x20;
	PORTB &= 0xDF;
	return 0;
}
