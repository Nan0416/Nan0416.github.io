.include "./m329Pdef.inc"
	.section .__vectors
.global _RESET
jmp _RESET



_RESET:
	ldi r17, 0;
	
	.text
.global main
main:
	ldi r17, 0xff
	sts DDRB, r17
	sts PORTB, r17



;; 1 + (1 + 1 + 3 + 325632 + 3) * 49 + (1 + 2) * 1 =  15956364
;; 15956364 * 1/16M = 0.9972727275





.L3:
	ldi r19, 49
.L31:
	dec r19
	breq main
	jmp .L2
.L32:
	jmp .L31;

;; 0.020532
;; 1 + (1 + 1 + 3 + 1274 + 3) * 254 + (1 + 2) * 1 = 325632
;; 325632 * 1/16M = 0.020352s

.L2:
	ldi r17, 0xff ; 1
.L21:
	dec r17	; 1
	breq .L32; 1 or 2
	jmp .L1
.L22:
	jmp .L21; 3


;; 79.625 us
;; 1 + (1 + 1 + 3) * 254 + (1 + 2) * 1 = 1 + 1270 + 3 = 1274 
;; 1274 * 1/16M =  79.625 us (10^-6)
.L1:
	ldi r18, 0xff ; 1
.L11:
	dec r18 ; 1
	breq .L22; 1 or 2
	jmp .L1; 3
	
