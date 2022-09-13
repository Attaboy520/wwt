#ifndef _KERNEL_KEYBOARD_H_
#define _KERNEL_KEYBOARD_H_

#define KB_CNTRL_ADDRESS 0x60

void keyboard_intr_handler();
void init_keyboard();

#endif /* _KERNEL_KEYBOARD_H_ */