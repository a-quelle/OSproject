[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ;Put Kernel here in memory, link kernel with this offset too

mov [BOOT_DRIVE], dl    ;Save boot disk id to be sure
mov bp, 0x7000
mov sp, bp

mov bx, MSG_REAL_MODE
call print
call print_nl

call load_kernel
call switch_to_pm
jmp $

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/32bit_print.asm"
%include "boot/switch_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    ;read 32 sectors of disk from BOOT_DRIVE into KERNEL_OFFSET
    mov bx, KERNEL_OFFSET
    mov dh, 32
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
	;call clear_screen
	mov ebx, MSG_PROT_MODE
	call print_string_pm
    call KERNEL_OFFSET
	jmp $

BOOT_DRIVE db 0     ;For value of dl at startup
MSG_REAL_MODE db "Started in 16-bit real mode", 0	;org added to calculated value of MSG_REAL_MODE
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0  ;org added
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

times 510-($-$$) db 0
dw 0xaa55