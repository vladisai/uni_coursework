org 0x7c00

jmp 0:start

print_char:
    mov ah, 0xe
    int 0x10
    ret

print_mem:
    mov ecx, 0
loop:
    cmp dx, cx
    je end
    mov al, byte [ecx + eax]
    call print_char
    inc ecx
    jmp loop
end:
    ret

start:
    xor ax, ax
    mov bx, 0x9001
    mov ah, 2
    mov al, 1
    mov dl, 0x80
    xor dh, dh
    mov cx, 2
    int 0x13

    jmp 0:0x9001

    times 510 - ($-$$) db 0
    dw 0xaa55
