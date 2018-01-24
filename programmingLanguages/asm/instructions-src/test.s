.equ DDRB, 0x24
.equ PORTB, 0x25

.text
	.global main
main:
	ldi r16, 0x7f
	ldi r17, 0x01
	add r17, r16
	brpl .loop; N=0 goes to infinit loop
	ldi r17, 0xff
	sts DDRB,r17
	sts PORTB, r17

.loop:
	jmp .loop
