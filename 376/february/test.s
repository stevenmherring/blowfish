	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	leaq	L_.str(%rip), %rax
	movl	$0, -4(%rbp)
	movq	%rax, -16(%rbp)
	movl	$0, -20(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$0, -20(%rbp)
	setne	%al
	xorb	$1, %al
	testb	$1, %al
	jne	LBB0_2
	jmp	LBB0_3
LBB0_2:                                 ##   in Loop: Header=BB0_1 Depth=1
	leaq	L_.str1(%rip), %rdi
	movq	-16(%rbp), %rsi
	movb	$0, %al
	callq	_printf
	movl	%eax, -24(%rbp)         ## 4-byte Spill
	jmp	LBB0_1
LBB0_3:
	xorl	%edi, %edi
	callq	_exit
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Chris is GOD"

L_.str1:                                ## @.str1
	.asciz	"%s"


.subsections_via_symbols
