;no args
print_nl:
	push ax

	mov ah, 0x0e
	mov al, 0x0a ; newline
	int 0x10
	mov al, 0x0d
	int 0x10

	pop ax

	ret

;arg in bx
print:
	push ax
	push bx

	mov ah, 0x0e
	start:
		mov al, [bx] ; if
		cmp al, 0
		je done

		int 0x10

		add bx, 1
		jmp start

	done:

		pop bx
		pop ax

	ret