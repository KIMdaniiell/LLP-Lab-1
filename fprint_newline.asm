global print_newline

section .date
    nl: db 10

section .text
    print_newline:

	mov rax, 1
	mov rdi, 1
	mov rsi, nl
	mov rdx, 1

	syscall
	ret 
