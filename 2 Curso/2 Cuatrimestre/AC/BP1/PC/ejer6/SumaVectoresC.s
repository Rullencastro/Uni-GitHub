	.file	"SumaVectoresC.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"Faltan n\272 componentes del vector"
.LC1:
	.string	"Tama\361o Vectores:%u (%u B)\n"
	.align 8
.LC2:
	.string	"No hay suficiente espacio para los vectores "
	.align 8
.LC5:
	.string	"Tiempo:%11.9f\t / Tama\361o Vectores:%u\n"
	.align 8
.LC6:
	.string	"/ V1[%d]+V2[%d]=V3[%d](%8.6f+%8.6f=%8.6f) /\n"
	.align 8
.LC7:
	.string	"Tiempo:%11.9f\t / Tama\361o Vectores:%u\t/ V1[0]+V2[0]=V3[0](%8.6f+%8.6f=%8.6f) / / V1[%d]+V2[%d]=V3[%d](%8.6f+%8.6f=%8.6f) /\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movl	%edi, -100(%rbp)
	movq	%rsi, -112(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	cmpl	$1, -100(%rbp)
	jg	.L2
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$-1, %edi
	call	exit@PLT
.L2:
	movq	-112(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -84(%rbp)
	movl	-84(%rbp), %eax
	movl	$4, %edx
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-84(%rbp), %eax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -80(%rbp)
	movl	-84(%rbp), %eax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -72(%rbp)
	movl	-84(%rbp), %eax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -64(%rbp)
	cmpq	$0, -80(%rbp)
	je	.L3
	cmpq	$0, -72(%rbp)
	je	.L3
	cmpq	$0, -72(%rbp)
	jne	.L4
.L3:
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	movl	$-2, %edi
	call	exit@PLT
.L4:
	movl	$0, -88(%rbp)
	jmp	.L5
.L10:
	movl	-84(%rbp), %eax
	testq	%rax, %rax
	js	.L6
	cvtsi2sdq	%rax, %xmm0
	jmp	.L7
.L6:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L7:
	movsd	.LC3(%rip), %xmm1
	mulsd	%xmm0, %xmm1
	cvtsi2sd	-88(%rbp), %xmm2
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-80(%rbp), %rax
	addq	%rdx, %rax
	addsd	%xmm1, %xmm0
	movsd	%xmm0, (%rax)
	movl	-84(%rbp), %eax
	testq	%rax, %rax
	js	.L8
	cvtsi2sdq	%rax, %xmm0
	jmp	.L9
.L8:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L9:
	movsd	.LC3(%rip), %xmm1
	mulsd	%xmm1, %xmm0
	cvtsi2sd	-88(%rbp), %xmm2
	movsd	.LC3(%rip), %xmm1
	mulsd	%xmm2, %xmm1
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	subsd	%xmm1, %xmm0
	movsd	%xmm0, (%rax)
	addl	$1, -88(%rbp)
.L5:
	movl	-88(%rbp), %eax
	cmpl	%eax, -84(%rbp)
	ja	.L10
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	clock_gettime@PLT
	movl	$0, -88(%rbp)
	jmp	.L11
.L12:
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-80(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm1
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	addsd	%xmm1, %xmm0
	movsd	%xmm0, (%rax)
	addl	$1, -88(%rbp)
.L11:
	movl	-88(%rbp), %eax
	cmpl	%eax, -84(%rbp)
	ja	.L12
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	clock_gettime@PLT
	movq	-32(%rbp), %rdx
	movq	-48(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm1
	movq	-24(%rbp), %rdx
	movq	-40(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC4(%rip), %xmm2
	divsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -56(%rbp)
	cmpl	$9, -84(%rbp)
	ja	.L13
	movl	-84(%rbp), %eax
	movsd	-56(%rbp), %xmm0
	movl	%eax, %esi
	leaq	.LC5(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movl	$0, -88(%rbp)
	jmp	.L14
.L15:
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm2
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm1
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-80(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	movl	-88(%rbp), %ecx
	movl	-88(%rbp), %edx
	movl	-88(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC6(%rip), %rdi
	movl	$3, %eax
	call	printf@PLT
	addl	$1, -88(%rbp)
.L14:
	movl	-88(%rbp), %eax
	cmpl	%eax, -84(%rbp)
	ja	.L15
	jmp	.L16
.L13:
	movl	-84(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %eax
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm6
	movl	-84(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %eax
	leaq	0(,%rax,8), %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm5
	movl	-84(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %eax
	leaq	0(,%rax,8), %rdx
	movq	-80(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm4
	movl	-84(%rbp), %eax
	leal	-1(%rax), %esi
	movl	-84(%rbp), %eax
	leal	-1(%rax), %ecx
	movl	-84(%rbp), %eax
	leal	-1(%rax), %edx
	movq	-64(%rbp), %rax
	movsd	(%rax), %xmm3
	movq	-72(%rbp), %rax
	movsd	(%rax), %xmm2
	movq	-80(%rbp), %rax
	movsd	(%rax), %xmm1
	movl	-84(%rbp), %eax
	movsd	-56(%rbp), %xmm0
	movl	%esi, %r8d
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$7, %eax
	call	printf@PLT
.L16:
	movq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L18
	call	__stack_chk_fail@PLT
.L18:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC3:
	.long	2576980378
	.long	1069128089
	.align 8
.LC4:
	.long	0
	.long	1104006501
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
