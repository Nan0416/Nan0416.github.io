#define F_CPU 16000000UL
#include <avr/io.h>
void __init(){
	DDRB|=0x00;
}
int main(){
	DDRB |= 0x20;
	PORTB |= 0x20;
	return 0;
}
