[org 0x7c00]
mov ah, 0x0e
mov bx, defined_var ;stores a pointer to defined var in bx register

print_string:
    mov al, [bx] ; moves the value out of the pointer bx into al. this does not affext bx, it just takes the value
    cmp al, 0
    je input
    int 0x10
    inc bx ; increments the pointer. value in al is unaffected.
    jmp print_string

defined_var:
    db "Hello, World!", 0

input:
    mov ah, 0
    int 0x16
    mov ah, 0x0e
    int 0x10
    jmp input

exit:
    jmp $

times 510-($-$$) db 0
db 0x55, 0xaa
