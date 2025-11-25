section .asm

global insb
global insw
global outb
global outw

global inl
global outl

inl:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp+8] 
    in eax, dx       

    pop ebp
    ret

outl:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12] 
    mov edx, [ebp+8]  
    out dx, eax       

    pop ebp
    ret
insb:
    push ebp
    mov ebp, esp

    XOR eax, eax
    mov edx, [ebp+8]
    in al, dx

    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp+8]
    in ax, dx
    
    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax

    pop ebp
    ret