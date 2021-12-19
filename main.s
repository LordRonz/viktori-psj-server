	.file	"main.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"SIGINT Received\nTerminating..."
	.text
	.globl	sigint_handler
	.type	sigint_handler, @function
sigint_handler:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %esi
	movl	$2, %edi
	call	signal@PLT
	movb	$1, exit_requested(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	sigint_handler, .-sigint_handler
	.section	.rodata
	.align 8
.LC1:
	.string	"Usage:  %s <Server Port> <Server Address (OPTIONAL)>\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	leaq	sigint_handler(%rip), %rax
	movq	%rax, %rsi
	movl	$2, %edi
	call	signal@PLT
	cmpl	$2, -4(%rbp)
	je	.L3
	cmpl	$3, -4(%rbp)
	je	.L3
	movq	-16(%rbp), %rax
	movq	(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC1(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
	jmp	.L4
.L3:
	movq	-16(%rbp), %rdx
	movl	-4(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	run@PLT
	movl	$0, %eax
.L4:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (GNU) 11.1.0"
	.section	.note.GNU-stack,"",@progbits
