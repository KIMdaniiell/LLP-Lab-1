global exit

section .text
    exit:
	mov rax, 60
	;Код возврата поступате как аргумент в регистре rdi
	;xor rdi, rdi
	syscall
