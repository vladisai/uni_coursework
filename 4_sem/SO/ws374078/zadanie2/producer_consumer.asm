extern malloc

extern produce

extern consume

extern verhogen

extern proberen

global init

global producer

global consumer

section .data

memptr: ; data
    dq 0

prod: ; producer sem
    dq 0

cons: ; consumer sem
    dq 0

sz: ; used to store N
    dq 0

portion: ; used to store produced data
    dq 0

k: ; producer's pointer to data
    dq 0

kret: ; return value of produce
    dq 0

l:  ; consumer's pointer to data
    dq 0

lret: ; return value of consume
    dq 0

lprod: ; value being consumed
    dq 0

section .text

init:
    mov r8, rdi ; remember the input

    mov [prod], r8 ; assign N to producer sem
    mov r10, 0
    mov [cons], r10
    mov [sz], r8 ; remember N for later

    mov r9, 0x7fffffff
    cmp r8, r9; compare to 2^{31} - 1

    jbe test_0 
    mov rax, -1
    ret

test_0:
    mov r9, 0
    cmp r8, r9 ; compare to 0
    jne acceptable
    mov rax, -2
    ret

acceptable:
    mov rdi, r8
    shl rdi, 3
    call malloc
    mov [memptr], rax ; rax - returned value
    mov r9, 0 ; compare to 0
    cmp rax, r9

    jg created
    mov rax, -3
    ret

created:
    mov rax, 0
    ret

producer:
    mov rdi, portion
    call produce
    mov [kret], rax ; remember return value, if 0 we must stop

    mov rdi, prod ; acquire semaphore
    call proberen

    mov r10, [portion]

    mov r11, [memptr]

    mov rax, [k]
    mov qword [r11 + rax * 8], r10 ; write the produced stuff

    mov rdi, cons 
    call verhogen ; raise semaphore

    mov r9, [k]
    inc r9
    mov rax, [sz] 
    mov r10, 0
    cmpxchg r9, r10 ; % N
    mov [k], r9

    mov rax, [kret]
    test eax, eax

    jne producer 
    ret

consumer:
    mov rdi, cons ; acquire semaphore
    call proberen

    mov r8, [l]
    mov r9, [memptr]
    mov rdx, [r9 + r8 * 8] 
    mov [lprod], rdx; save the product

    mov rdi, prod 
    call verhogen; raise prod semaphore

    mov r9, [l] ; l = l + 1  mod N
    inc r9
    mov rax, [sz] 
    mov r10, 0
    cmpxchg r9, r10 ; % N
    mov [l], r9

    mov rdi, [lprod]
    call consume ; consume what we've got

    test eax, eax
    jne consumer

    ret
