.text

.global _x86_64_asm_lgdt
_x86_64_asm_lgdt:

  cli
  lgdtq (%rdi)
  movw	$16, %ax
  movl	%eax, %es
  movl	%eax, %gs
  movl	%eax, %ds
  movl	%eax, %fs
  popq	%rdi
  movl	$8, %eax
  pushq	%rax
  pushq	%rdi
  lretq
