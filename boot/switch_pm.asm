[bits 16]
switch_to_pm:
	cli							;disable interrupts	
	lgdt [gdt_descriptor]		;load the gdt descriptor
	mov eax, cr0
	or eax, 0x1					;set the 32-bit mode
	mov cr0, eax
	jmp CODE_SEG:init_pm		;Far jmp to absolute adress, needs org from init_pm

[bits 32]
init_pm:						;org added to calculated address
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	call BEGIN_PM