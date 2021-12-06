global print_char
extern print_newline

section .data
    char_table: db "012345678901234567890123456780 !'#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

section .text
    print_char:
	;Код символа поступает в качестве аргумента в регистре rdi
	;Беру его из таблицы символов ACSII и 
	;затем печатаю его в stdout	
	
	mov rsi, rdi
	add rsi, 1
	add rsi, char_table

	mov rax, 1
	mov rdi, 1
	mov rdx, 1
	syscall

	;Вывел символ, перевожу строку
	call print_newline
	ret 
