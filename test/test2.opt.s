	.text
	.file	"iwbasic_module"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	pushq	%rax
	leaq	.Lstr(%rip), %rdi
	callq	puts@PLT
	leaq	.Lfmt.1(%rip), %rdi
	movl	$42, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rcx
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.type	.Lstr,@object                   # @str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"Hello, World!"
	.size	.Lstr, 14

	.type	.Lfmt.1,@object                 # @fmt.1
.Lfmt.1:
	.asciz	"%d\n"
	.size	.Lfmt.1, 4

	.section	".note.GNU-stack","",@progbits
