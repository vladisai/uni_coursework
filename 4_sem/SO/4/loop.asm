org 0x7c00 

jmp start

ENTER_NAME_MSG: db 'Enter your name ', 0xd, 0xa, 0x0

WELCOME_MSG: db 'Hello ', 0x0

END_MSG: db 0xd, 0xa, 0x0

BACKSPACE: db 0x8

SPACE: db 0x20

ENTER: db 0x0d

NAME: TIMES 15 db 0x0

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

start:
    xor ax, ax
    mov ss, ax     ; po tej instrukcji procesor blokuje przerwania na czas wykonania kolejnej instrukcji
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

    jmp 0x7e00

    times 510 - ($-$$) db 0
    dw 0xaa55
