global proberen

global verhogen

section .data

section .text

proberen:
    mov rax, -1
    lock xadd qword [rdi], rax
    mov rax, 0
    cmp qword [rdi], rax
    jge finish
    mov rax, 1
    lock xadd qword [rdi], rax
    jmp proberen
finish:
    ret

verhogen:
    mov rax, 1
    lock xadd qword [rdi], rax
    ret
