global print_string
extern string_length, print_newline

section .text
	;В качестве аргумента приходит адрес начала строки (rdi).
	;Изветсно, что строка нуль-терминирована.
	;Необходимо вывести строку в stout.
	print_string:
		;Нужно вызвать следующие фунции:
		;	-string_lenght чтобы получить длину строки
		;	-print_newline чтобы перевести строку
		;обе используют rax rdi rsi rdx
		;поэтому временно сохраню аргумент в стек
		push rdi
		call string_length
		mov rdx, rax
		pop rdi
		mov rsi, rdi
		mov rdi, 1
		mov rax, 1
		syscall
		call print_newline
		ret
