[bits 32]

VIDEO_MEMORY equ 0xb8000	;start of VGA memory, org is not added
WHITE_ON_BLACK equ 0x0f		;color for char in text mode

;parameters		ebx contains address of char to print
print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY	;addresses take 32bit in 32bit mode 0_0
	mov ah, WHITE_ON_BLACK

	print_string_pm_loop:
		mov al, [ebx]
		cmp al, 0
		je print_string_pm_done

		mov [edx], ax
		add ebx, 1
		add edx, 2

		jmp print_string_pm_loop

	print_string_pm_done:
	popa
	ret

clear_screen:
	pusha
	mov edx, VIDEO_MEMORY
	mov ax, 0
	mov ebx, 0

	clear_screen_loop:
		mov [edx], ax
		add ebx, 1
		add edx, 2
		cmp ebx, 2000
		jl clear_screen_loop
	
	popa 
	ret
