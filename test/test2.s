	.text
	.file	"iwbasic_module"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	leaq	.Lfmt(%rip), %rdi
	leaq	.Lstr(%rip), %rsi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$42, 4(%rsp)
	leaq	.Lfmt.1(%rip), %rdi
	movl	$42, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.Lfmt,@object                   # @fmt
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt:
	.asciz	"%s\n"
	.size	.Lfmt, 4

	.type	.Lstr,@object                   # @str
.Lstr:
	.asciz	"Hello, World!"
	.size	.Lstr, 14

	.type	.Lfmt.1,@object                 # @fmt.1
.Lfmt.1:
	.asciz	"%d\n"
	.size	.Lfmt.1, 4

	.section	".note.GNU-stack","",@progbits
