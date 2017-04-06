org 0x8000 

jmp 0:start

ENTER_NAME_MSG: db 'Enter your name ', 0xd, 0xa, 0x0

WELCOME_MSG: db 'Hello ', 0x0

END_MSG: db 0xd, 0xa, 0x0

BACKSPACE: db 0x8

SPACE: db 0x20

ENTER: db 0x0d

NAME: TIMES 32 db 0x0

print_char:
    mov ah, 0xe
    int 0x10
    ret

print_num:
    mov ah, 0xe
    mov bl, '0'
    add al, bl
    int 0x10
    ret

process_enter:
    cmp ecx, 3
    jl read_loop
    mov al, [ENTER]
    call print_char
    jmp end_read

process_backspace:
    cmp ecx, 0
    jle read_loop
    dec ecx
    mov al, [BACKSPACE]
    call print_char
    mov al, [SPACE]
    call print_char
    mov al, [BACKSPACE]
    call print_char
    jmp read_loop

print_str:
    mov ecx, 0
    mov cx, ax
loop:
    mov al, byte [ecx]
    test al, al
    je end
    mov al, byte [ecx]
    inc ecx
    call print_char
    jmp loop
end:
    ret

read_name:
    mov ecx, 0
read_loop:
    push ecx
    mov ah, 0x0
    int 0x16
    pop ecx
    cmp al, [ENTER]
    je process_enter
    cmp al, [BACKSPACE]
    je process_backspace
    cmp ecx, 12
    je read_loop
    push ecx
    call print_char
    pop ecx
    mov byte [NAME + ecx], al
    inc ecx
    jmp read_loop
end_read:
    mov al, 0xa
    call print_char
    mov byte [NAME + ecx], 0x0
    ret

print_mem:
    mov ecx, 0
loop_mem:
    cmp dx, cx
    je end_mem
    mov al, byte [ecx + eax]
    call print_char
    inc ecx
    jmp loop_mem
end_mem:
    ret


start:
    xor ax, ax
    mov ss, ax     ; po tej instrukcji procesor blokuje przerwania na czas wykonania kolejnej instrukcji
    mov es, ax
    mov ds, ax
    mov sp, 0x8000 ; rejestr sp musi być załadowany natychmiast po załadowaniu rejestru ss

    mov ax, ENTER_NAME_MSG 
    call print_str

    call read_name

    mov ax, WELCOME_MSG
    call print_str

    mov ax, NAME
    call print_str

    mov ax, END_MSG
    call print_str

    xor ax, ax
    mov bx, NAME
    mov ah, 0x03
    mov al, 0x01
    mov dl, 0x80
    xor dh, dh
    mov cx, 0x0008
    int 0x13 ; write the name to disk

    xor ax, ax
    mov bx, 0x7c00
    mov ah, 0x02
    mov al, 0x01
    mov dl, 0x80
    xor dh, dh
    mov cx, 0x0003
    int 0x13 ; read

    mov cx, 0fh
    mov dx, 8480h
    mov ah, 86h
    int 15h ; sleep

    jmp 0:0x7c00

    times 512 - ($-$$) db 0
