.globl _start
_start:
    mov r0, #0xD2      @ interrupt mode
    msr cpsr_c, r0
    mov sp, #0x8000    @ interrupt stack pointer
    mov r0, #0xD3      @ svc mode
    msr cpsr_c, r0      
    mov sp, #0x8000000 @ svc mode stack pointer
    mov fp, #0x0       @ svc mode frame pointer
    bl _cstart
hang: b hang
