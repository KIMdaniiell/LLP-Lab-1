fexit.o: fexit.asm
	nasm -g fexit.asm -felf64 -o fexit.o

fprint_char.o: fprint_char.asm
	nasm -g fprint_char.asm -felf64 -o fprint_char.o

fprint_newline.o: fprint_newline.asm
	nasm -g fprint_newline.asm -felf64 -o fprint_newline.o

fstring_length.o: fstring_length.asm
	nasm -g fstring_length.asm -felf64 -o fstring_length.o

fprint_string.o: fprint_string.asm
	nasm -g fprint_string.asm -felf64 -o fprint_string.o

main.o: main.asm
	nasm -g main.asm -felf64 -o main.o

main: fexit.o main.o fprint_char.o fprint_newline.o fstring_length.o fprint_string.o
	ld -o main main.o fexit.o fprint_char.o fprint_newline.o fstring_length.o fprint_string.o

