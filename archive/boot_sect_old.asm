mov ax, 0x7c0
mov ds, ax
mov es, ax
mov bp, 0x8000
mov sp, bp
mov bx, 0x8001
mov dh, 2
call disk_load		;bios sets dl to boot drive nr.

mov dx, [0x8001]
call print_hex

call print_nl

mov dx, [0x8201]	;0x9000 + 512
call print_hex

jmp $

%include "boot_sect_print.asm"
%include "boot_sect_print_hex.asm"
%include "boot_sect_disk.asm"

times 510 -($-$$) db 0
dw 0xaa55

;since qemu loads this file into an emulated hdd, add two data sectors
times 256 dw 0xdada
times 256 dw 0xface