section .text
 
 
; Принимает код возврата и завершает текущий процесс
; -> rdi - код возврата
exit: 
    mov rax, 60
    syscall
    ret
    

; Принимает указатель на нуль-терминированную строку, возвращает её длину
; -> rdi - указатель на нуль-терминированную строку
; <- rax -длина строки
string_length:
    xor rax, rax
    .loop:
	cmp byte [rdi+rax], 0 
	je .end
	inc rax
	jne .loop
    .end:
        ret
	

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
; -> rdi - указатель на нуль-терминированную строку
print_string:
    push rdi
    call string_length
    pop rdi
    mov rsi, rdi
    mov rdx, rax
    mov rax, 1
    mov rdi, 1
    syscall
    ret
    

; Принимает код символа и выводит его в stdout
; -> rdi - код символа
print_char:
    push rdi
    mov rax, 1
    mov rdi, 1
    mov rsi, rsp
    mov rdx, 1
    syscall
    pop rdi
    ret


; Переводит строку (выводит символ с кодом 0xA ~ 10)
print_newline:
    mov rdi, 10
    call print_char
    ret
    

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
; -> rdi - 8-байтовое беззнаковое число 
print_uint:
    mov rax, rdi
    mov r8, 10
    
    mov rdi, rsp			
    sub rsp, 20+1	;8-байт число занимает не более 20 символов в 10 формате + 1 байт на 0-терм.

    dec rdi
    mov byte[rdi], 0
    .loop:
	xor rdx,rdx
	div r8
	add dl, '0'
	dec rdi
	mov byte[rdi], dl
	test rax, rax
	jnz .loop

    call print_string

    add rsp, 21
    ret
    

; Выводит знаковое 8-байтовое число в десятичном формате
; -> rdi - 8-байтовое знаковое число 
print_int:
    cmp rdi, 0
    jge .print
    .change_sign:
	push rdi
	mov rdi, 45		; ASCII(45) ~ '-'
	call print_char
	pop rdi
	neg rdi
    .print:
	call print_uint
    ret



; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
; -> rdi - указателя на первую нуль-терминированную строку
; -> rsi - указателя на вторую нуль-терминированную строку
; <- rax - 1 / 0
string_equals:
    xor rax, rax
    xor r8, r8
    .loop:
	mov al, byte[rdi]
	mov r8b, byte[rsi]
	cmp al, r8b
	jne .fail

	test al, al
	jz .end
	inc rdi
	inc rsi
	jmp .loop
    .fail:
	xor rax, rax
	ret
    .end:
	mov rax, 1
	ret
	

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
; <- rax - символ / 0
read_char:
    push byte 0
    mov rax, 0
    mov rdi, 0
    mov rsi, rsp
    mov rdx, 1
    syscall

    cmp rax, -1
    jne .end
    mov rax, 0
    ret
    .end:
	pop rax
	ret
	

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор
; -> rdi - адрес начала буфера
; -> rsi - размер буфера
; <- rax - адрес буффера / 0
; <- rdx - длинна слова
read_word:
    mov rcx, 0
    .read_spacing:
	push rdi
    	push rsi
	push rcx 
	call read_char
	pop rcx
	pop rsi
	pop rdi

	cmp rax, 0x20
	je .read_spacing
	cmp rax, 0x9
	je .read_spacing
	cmp rax, 0xA
	je .read_spacing
    .read:
	test rax, rax
	jz .end
	cmp rax, 0x20
	je .end
	cmp rax, 0x9
	je .end
	cmp rax, 0xA
	je .end
	
	dec rsi
	jz .fail
	
	mov byte[rdi+rcx], al
	inc rcx

	push rdi
    	push rsi
	push rcx 
	call read_char
	pop rcx
	pop rsi
	pop rdi
	jmp .read
    .fail:
	xor rax, rax
	ret
    .end:
	mov byte[rdi+rcx], 0
	mov rdx, rcx
	mov rax, rdi
	ret
 

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
; -> rdi - указатель на строку
; <- rax - число
; <- rdx - длина в символах / 0
parse_uint:
    xor rax, rax
    xor rcx, rcx
    xor rsi, rsi
    mov r8, 10
    .loop:	
	mov sil, [rdi+rcx]

	test sil, sil
	jz .end
	cmp sil, '0'
	jl .end
	cmp sil, '9'
	jg .end

	sub sil, '0'
	mul r8
	add rax, rsi
	inc rcx
	jmp .loop
    .end:
	mov rdx, rcx
	ret


; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    xor rax, rax
    ret 

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
    xor rax, rax
    ret
