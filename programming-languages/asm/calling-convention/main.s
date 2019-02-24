	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	main
	.type	main, @function
main:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	call func
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	main, .-main
.global	func
	.type	func, @function
func:
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 3 */
.L__stack_usage = 3
	ldi r24,lo8(10)
	std Y+1,r24
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+1,r24
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+1,r24
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	func, .-func
	.ident	"GCC: (GNU) 4.9.2"
