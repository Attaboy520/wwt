#
# gdt.s
#
#  Created on: Dec 29, 2010
#      Author: cds
#

.text

.global _x86_64_asm_lgdt
_x86_64_asm_lgdt:

  lgdt (%rdi)

  pushq %rsi                  # push code selector
  movabsq $.done, %r10
  pushq %r10                  # push return address
  lretq                       # far-return to new cs descriptor ( the retq below )
.done:
  movq %rdx, %es
  movq %rdx, %fs
  movq %rdx, %gs
  movq %rdx, %ds
  movq %rdx, %ss
  retq


.global _x86_64_asm_ltr
_x86_64_asm_ltr:
  ltr %di
  retq
