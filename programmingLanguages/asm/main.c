#define F_CPU 16000000UL
#include <avr/io.h>
//#include <util/delay.h>
//int qinnan = 10;
int main(){
	unsigned char a[] = {1,2,3};
	DDRB |= 0x20;
	
	PORTB |= 0x20;
	//_delay_ms(1000);
	PORTB &= 0xDF;
	//_delay_ms(1000);
	
	return 0;
}
