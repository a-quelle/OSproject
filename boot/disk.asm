;params: bx is read buffer address
;		 dl is drive number to read from
;		 dh is number of sectors to read
;reads dh sectors, starting at sector 2 of cylinder 0 of drive dl

disk_load:
	push ax
	push bx
	push cx
	push dx
	mov ah, 0x02 	;disk read with int 0x13
	mov al, dh 		;al is number of sectors to read
	mov cl, 0x02 	;cl is start sector
	mov ch, 0x00 	;ch is cylinder to read
	mov dh, 0x00	;dh is head number to use
	int 0x13
	jc disk_error

	pop dx
	cmp al, dh 		;al contains number of sector read
	jne sectors_error
	pop cx
	pop bx
	pop ax
	ret

disk_error:
	mov bx, DISK_ERROR
	call print
	call print_nl
	mov dh, ah 		;ah contains error code
	call print_hex	;will print error code and drive number
	jmp disk_loop

sectors_error:
	mov bx, SECTORS_ERROR
	call print

disk_loop:
	jmp $

DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0