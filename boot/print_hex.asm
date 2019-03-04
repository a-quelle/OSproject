;arg in dx
print_hex:
	pusha

	mov cx, 0 ;index of string
	mov bx, HEX_OUT + 6

	hex_loop:
		cmp cx, 4
		je end

		mov ax, dx ;data in dx, ax is scratch
		and ax, 0x000f ;take last digit of ax
		add al, 0x30
		cmp al, 0x39   ;if al > 0x39=57, add 7 to get A-F
		jle append_string
		add al, 7

		append_string:
		sub bx, 1
		mov [bx], al
		ror dx, 4 ;rotate dx right by 4 bits, or one hex digit
		add cx, 1
		jmp hex_loop

	end:
	call print
	call print_nl
	popa
	ret

HEX_OUT: db '0x0000', 0 ;the string "0x0000"