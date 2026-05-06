# k0 compiler — generated x86-64 AT&T assembly
# Assemble and link: gcc <this_file> k0rt.c -o <program>

	.section	.rodata
.Lstr_0:
	.asciz	"x before {%lld}\n"
.Lstr_1:
	.asciz	"x after {%lld}\n"

	.section	.data

	.section	.text

	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$48, %rsp
	callq	__init
	movq	$10, %rax
	movq	%rax, -8(%rbp)
	movq	$20, %rax
	movq	%rax, -16(%rbp)
.Llab_2:
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	cmpq	%rcx, %rax
	jl	.Llab_1
	jmp	.Llab_3
.Llab_1:
	movq	-8(%rbp), %rax
	movq	$1, %rcx
	addq	%rcx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	jmp	.Llab_2
.Llab_3:
	leaq	.Lstr_0(%rip), %rdi
	movq	-8(%rbp), %rsi
	xorb	%al, %al
	callq	printf
	movq	%rax, -32(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	addq	%rcx, %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -8(%rbp)
	leaq	.Lstr_1(%rip), %rdi
	movq	-8(%rbp), %rsi
	xorb	%al, %al
	callq	printf
	movq	%rax, -48(%rbp)
	movq	$0, %rax
	leave
	ret
	xorq	%rax, %rax
	leave
	ret
	.size	main, .-main
