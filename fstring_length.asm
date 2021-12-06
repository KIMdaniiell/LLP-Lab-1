global string_length

section .text
	;В качестве аргумента (rdi) выступает указатель на строку с символом [0] в конце.
	;Буду итерироваться по байтам-символам, пока не дойду до [0].
	;Предполагаю, что нуль-терминатор тоже считается в длинне строки

    string_length:
	xor rax, rax

    loop:
	cmp byte [rdi+rax], 0
	je end
	inc rax
	jmp loop
    end:
	ret
	
