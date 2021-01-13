global _strings_equal
global _add_strings
global error
global printInt
global printString
global readInt
global readString

extern printf
extern scanf
extern puts
extern strcmp
extern strlen
extern malloc
extern memcpy
extern exit

	section .data

empty_str db '', 0
runtime_error db 'runtime error', 0
print_int_fmt db `%d\n`, 0
print_string_fmt db `%s\n`, 0
read_int_fmt db `%d`, 0
read_newlines_fmt db `%*[\n]`, 0
read_line_fmt db `%m[^\n]`, 0

	section .text

_strings_equal:
	push rbp
	mov rbp, rsp
	and rsp, 0xfffffffffffffff0
	push rsi
	push rdi
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
	mov rsi, [rbp + 24]
	mov rdi, [rbp + 16]
	call strcmp
	test eax, eax
	sete al
	movzx eax, al
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rdi
	pop rsi
	leave
	ret

_add_strings:
	push rbp
	mov rbp, rsp
	sub rsp, 48
	and rsp, 0xfffffffffffffff0
	push rsi
	push rdi
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
	mov rdi, [rbp + 16]
	mov rsi, [rbp + 24]
	mov [rbp - 40], rdi
	mov [rbp - 48], rsi
	mov rax, [rbp - 40]
	mov rdi, rax
	call strlen
	mov [rbp - 8], rax
	mov rax, [rbp - 48]
	mov rdi, rax
	call strlen
	mov [rbp - 16], rax
	mov rdx, [rbp - 8]
	mov rax, [rbp - 16]
	add rax, rdx
	add rax, 1
	mov rdi, rax
	call malloc
	mov [rbp - 24], rax
	cmp QWORD [rbp - 24], 0
	jne .L12
	mov eax, empty_str
	jmp .L13
.L12:
	mov rdx, [rbp - 8]
	mov rcx, [rbp - 40]
	mov rax, [rbp - 24]
	mov rsi, rcx
	mov rdi, rax
	call memcpy
	mov rax, [rbp - 16]
	lea rdx, [rax+1]
	mov rcx, [rbp - 24]
	mov rax, [rbp - 8]
	add rcx, rax
	mov rax, [rbp - 48]
	mov rsi, rax
	mov rdi, rcx
	call memcpy
	mov rax, [rbp - 24]
.L13:
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rdi
	pop rsi
	leave
	ret

error:
	push rbp
	mov rbp, rsp
	mov rdi, runtime_error
	call puts
	mov rdi, 1
	call exit

printInt:
	push rbp
	mov rbp, rsp
	and rsp, 0xfffffffffffffff0
	push rsi
	push rdi
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
	mov rsi, [rbp + 16]
	mov rdi, print_int_fmt
	mov rax, 0
	call printf
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rdi
	pop rsi
	leave
	ret

printString:
	push rbp
	mov rbp, rsp
	and rsp, 0xfffffffffffffff0
	push rsi
	push rdi
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
	mov rsi, [rbp + 16]
	mov rdi, print_string_fmt
	mov rax, 0
	call printf
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rdi
	pop rsi
	leave
	ret

readInt:
	push rbp
	mov rbp, rsp
	sub rsp, 4
	and rsp, 0xfffffffffffffff0
	push rsi
	push rdi
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
	lea rsi, [rbp - 4]
	mov rdi, read_int_fmt
	mov rax, 0
	call scanf
	cmp rax, 1
	je .L1
	mov rax, 0
	jmp .L2
.L1:
	mov eax, [rbp - 4]
.L2:
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rdi
	pop rsi
	leave
	ret

readString:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	and rsp, 0xfffffffffffffff0
	push rsi
	push rdi
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
	mov rdi, read_newlines_fmt
	mov rax, 0
	call scanf
	lea rsi, [rbp - 8]
	mov rdi, read_line_fmt
	mov rax, 0
	call scanf
	cmp rax, 1
	je .L1
	mov rax, empty_str
	jmp .L2
.L1:
	mov rax, [rbp - 8]
.L2:
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rdi
	pop rsi
	leave
	ret
