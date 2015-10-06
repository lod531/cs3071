	.syntax unified
	.arch armv7-a
	.eabi_attribute 27, 3
	.eabi_attribute 28, 1
	.fpu vfpv3-d16
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.thumb
	.file	"lab1.c"
	.text
	.align	2
	.global	insertionSort
	.thumb
	.thumb_func
	.type	insertionSort, %function
insertionSort:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r1, #0
	ble	.L11
	mov	ip, #0
	add	ip, ip, #1
	cmp	ip, r1
	push	{r4, r5, r6, r7, r8, lr}
	add	r8, r0, #4
	beq	.L1
.L14:
	cmp	ip, #0
	ble	.L5
	ldr	r2, [r0, #4]
	mov	r7, r8
	ldr	r4, [r8, #-4]
	cmp	r2, r4
	bge	.L5
	mov	r6, r0
	subs	r3, r0, #4
	mov	lr, r0
	mov	r5, ip
	b	.L8
.L13:
	ldr	r2, [r3, #4]
	subs	r3, r3, #4
	ldr	r4, [r6, #-4]!
	cmp	r2, r4
	bge	.L5
.L8:
	subs	r5, r5, #1
	str	r2, [lr]
	str	r4, [r7]
	mov	lr, r3
	mov	r7, r6
	bne	.L13
.L5:
	add	ip, ip, #1
	add	r8, r8, #4
	cmp	ip, r1
	add	r0, r0, #4
	bne	.L14
.L1:
	pop	{r4, r5, r6, r7, r8, pc}
.L11:
	bx	lr
	.size	insertionSort, .-insertionSort
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	movs	r0, #40
	bl	malloc
	movs	r2, #10
	mov	r1, r2
	movs	r4, #9
	stmia	r0, {r2, r4}
	movs	r4, #8
	movs	r2, #7
	str	r4, [r0, #8]
	str	r2, [r0, #12]
	movs	r4, #6
	movs	r2, #5
	str	r4, [r0, #16]
	str	r2, [r0, #20]
	movs	r4, #4
	movs	r2, #3
	str	r4, [r0, #24]
	str	r2, [r0, #28]
	movs	r4, #2
	movs	r2, #1
	str	r4, [r0, #32]
	str	r2, [r0, #36]
	bl	insertionSort
	movs	r0, #0
	pop	{r4, pc}
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.8.2-16ubuntu4) 4.8.2"
	.section	.note.GNU-stack,"",%progbits
