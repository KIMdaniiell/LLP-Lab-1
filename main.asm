global _start
extern exit, print_char, print_newline, string_length, print_string

section .data
    message: db "=====The End=====",0
    instr: db "ottff",0
section .text
    _start:
	print:
	    mov rdi, message
	    call print_string	    

	    mov rdi, instr
	    call string_length
	    mov rdi, rax
	    call print_char
		
	    call exit
