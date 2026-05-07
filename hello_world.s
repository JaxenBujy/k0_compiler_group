# k0 compiler — generated x86-64 AT&T assembly
# Assemble and link: gcc <this_file> k0rt.c -o <program>

	.section	.rodata
.Lstr_0:
	.asciz	"z: %lld\n"
.Lstr_1:
	.asciz	"x is bigger than y"
.Lstr_2:
	.asciz	"x is smaller than y"
.Lstr_3:
	.asciz	"x: %lld\n"
.Lstr_4:
	.asciz	"sum = %lld\n"
.Lstr_5:
	.asciz	"final sum is %lld\n"

	.section	.data

	.section	.text

	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$80, %rsp
	callq	__init
	movq	$5, %rax
	movq	%rax, -8(%rbp)
	movq	$3, %rax
	movq	%rax, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	add
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -24(%rbp)
	leaq	.Lstr_0(%rip), %rdi
	movq	-24(%rbp), %rsi
	xorb	%al, %al
	callq	printf
	movq	%rax, -40(%rbp)
.Llab_5:
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	cmpq	%rcx, %rax
	jg	.Llab_3
	jmp	.Llab_4
.Llab_3:
	leaq	.Lstr_1(%rip), %rdi
	callq	println
	movq	%rax, -48(%rbp)
	jmp	.Llab_9
.Llab_4:
	leaq	.Lstr_2(%rip), %rdi
	callq	println
	movq	%rax, -56(%rbp)
.Llab_9:
.Llab_9:
	movq	-8(%rbp), %rax
	movq	$10, %rcx
	cmpq	%rcx, %rax
	jl	.Llab_7
	jmp	.Llab_10
.Llab_7:
	leaq	.Lstr_3(%rip), %rdi
	movq	-8(%rbp), %rsi
	xorb	%al, %al
	callq	printf
	movq	%rax, -64(%rbp)
	movq	-8(%rbp), %rax
	movq	$1, %rcx
	addq	%rcx, %rax
	movq	%rax, -72(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, -8(%rbp)
	jmp	.Llab_9
.Llab_10:
	callq	loop
	movq	%rax, -80(%rbp)
	xorq	%rax, %rax
	leave
	ret
	.size	main, .-main

	.globl	add
	.type	add, @function
add:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	addq	%rcx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	leave
	ret
	xorq	%rax, %rax
	leave
	ret
	.size	add, .-add

	.globl	loop
	.type	loop, @function
loop:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$64, %rsp
	movq	$0, %rax
	movq	%rax, -8(%rbp)
	movq	$1, %rax
	movq	%rax, -16(%rbp)
	movq	$10, %rax
	movq	%rax, -24(%rbp)
.Llab_15:
	movq	-16(%rbp), %rax
	movq	-24(%rbp), %rcx
	cmpq	%rcx, %rax
	jle	.Llab_16
	jmp	.Llab_17
.Llab_16:
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	addq	%rcx, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -8(%rbp)
	leaq	.Lstr_4(%rip), %rdi
	movq	-8(%rbp), %rsi
	xorb	%al, %al
	callq	printf
	movq	%rax, -40(%rbp)
	movq	-16(%rbp), %rax
	movq	$1, %rcx
	addq	%rcx, %rax
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, -16(%rbp)
	jmp	.Llab_15
.Llab_17:
	leaq	.Lstr_5(%rip), %rdi
	movq	-8(%rbp), %rsi
	xorb	%al, %al
	callq	printf
	movq	%rax, -56(%rbp)
	xorq	%rax, %rax
	leave
	ret
	.size	loop, .-loop
