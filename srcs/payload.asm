section .text
global payload
global payload_size

payload_size: dq end - payload

payload:
	push rbx
	push rcx
	push rdx
	jmp load_string
string_loaded:
	mov rdx, 10
	pop rcx
	mov rbx, 1
	mov rax, 4
	int 0x80
	mov rax, 0x44444444
	pop rbx
	pop rcx
	pop rdx
	jmp rax
;msg: db "---GMP---", 10, 0
;msg_len: db $ - msg
load_string:
	call string_loaded
	db "---GMP---", 10, 0
;target_addr: dq 0x44444444

end:

