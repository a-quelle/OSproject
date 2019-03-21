[extern isr_handler]
[extern irq_handler]

isr_common_stub:
    pusha   ;pushes adi, esi, ebp, esp, ebx, edx, ecx, eax, completes registers_t
    mov ax, ds
    push eax
    mov ax, 0x10    ;offset of data segment in gdt
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp ; registers_t*. Do not pass by value since c compiler can edit args and corrupt stack.
    cld ; cdecl requires resetting direction flag (used for movs, stos, scas and possible others)

    call isr_handler

    pop eax ; clean registers_t* from stack
    pop eax
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    popa
    add esp, 8 ;clean up stack of isrX
    iret ;pop cs, eip, eflags, ss and esp pushed at interrupt, eflags contains interrupt flag

irq_common_stub: ;same as irs_common_stub except call irq_handler and pop ebx after call
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp;
    cld

    call irq_handler

    pop ebx
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    iret

;explicitly declare global variables for linker
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

isr0:   ;interrupts now disabled using always0 from idt_gate_t
    push 0 ;push 32bit, since in 32-bit mode
    push 0
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr1:
    push 0
    push 1
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr2:
    push 0
    push 2
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr3:
    push 0
    push 3
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr4:
    push 0
    push 4
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr5:
    push 0
    push 5
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr6:
    push 0
    push 6
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr7:
    push 0
    push 7
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr8:
    ;error code pushed for this interrupt
    push 8
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr9:
    push 0
    push 9
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr10:
    ;error code pushed for this interrupt
    push 10
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr11:
    ;error code pushed for this interrupt
    push 11
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr12:
    ;error code pushed for this interrupt
    push 12
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr13:
    ;error code pushed for this interrupt
    push 13
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr14:
    ;error code pushed for this interrupt
    push 14
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr15:
    push 0
    push 15
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr16:
    push 0
    push 16
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr17:
    ;error code pushed for this interrupt
    push 17
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr18:
    push 0
    push 18
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr19:
    push 0
    push 19
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr20:
    push 0
    push 20
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr21:
    push 0
    push 21
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr22:
    push 0
    push 22
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr23:
    push 0
    push 23
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr24:
    push 0
    push 24
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr25:
    push 0
    push 25
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr26:
    push 0
    push 26
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr27:
    push 0
    push 27
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr28:
    push 0
    push 28
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr29:
    push 0
    push 29
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr30:
    ;error code pushed for this interrupt
    push 30
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

isr31:
    push 0
    push 31
    jmp isr_common_stub ;do not use call, it modifies the stack, which would change registers_t

irq0:
	push 0
	push 32
	jmp irq_common_stub

irq1:
	push 1
	push 33
	jmp irq_common_stub

irq2:
	push 2
	push 34
	jmp irq_common_stub

irq3:
	push 3
	push 35
	jmp irq_common_stub

irq4:
	push 4
	push 36
	jmp irq_common_stub

irq5:
	push 5
	push 37
	jmp irq_common_stub

irq6:
	push 6
	push 38
	jmp irq_common_stub

irq7:
	push 7
	push 39
	jmp irq_common_stub

irq8:
	push 8
	push 40
	jmp irq_common_stub

irq9:
	push 9
	push 41
	jmp irq_common_stub

irq10:
	push 10
	push 42
	jmp irq_common_stub

irq11:
	push 11
	push 43
	jmp irq_common_stub

irq12:
	push 12
	push 44
	jmp irq_common_stub

irq13:
	push 13
	push 45
	jmp irq_common_stub

irq14:
	push 14
	push 46
	jmp irq_common_stub

irq15:
	push 15
	push 47
	jmp irq_common_stub