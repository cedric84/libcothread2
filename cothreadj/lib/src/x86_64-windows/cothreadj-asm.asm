PUBLIC cothreadj_init

EXTERN cothreadj_core : proto

COTHREADJ_ATTR_STACK		EQU		+0
COTHREADJ_ATTR_STACK_SZ		EQU		+8

; https://stackoverflow.com/questions/33751509/external-assembly-file-in-visual-studio
; https://developer.ibm.com/articles/l-gas-nasm/
;
; calling conventions:
; https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170
;
; rcx is used as arg0.
; rdx is used as arg1.
;
; r12-r15, rdi, rsi, rbx, rbp, rsp belong to the calling function and shall be preserved by the called one.
; the stack shall be 16-byte aligned immediately before the call instruction is executed.
; r9 home, r8 home, rdx home & rcx home are always pushed (in this order) before the call instruction is executed.
;
; mov	dst, src
;
.code
cothreadj_init:
	;---Save registers---
	push	r12
	push	r13

	;---Save the caller stack---
	mov		r12, rbp
	mov		r13, rsp

	;---Setup the callee stack---
	; Compute the past-the-end stack address
	mov		rax, COTHREADJ_ATTR_STACK[rdx]		; store the lowest stack address in rax.
	add		rax, COTHREADJ_ATTR_STACK_SZ[rdx]	; rax points the past-the-end stack address.
	; Setup the callee stack frame
	mov		rbp, rax							; load the stack base pointer with the past-the-end stack address.
	mov		rsp, rbp							; empty the stack.
	; from this point, stack is aligned on a 16-byte boundary (if the provided stack is well defined.)

	;---Initialize the cothread---
;	mov		rdx, rdx		; rdx is used as cothreadj_core arg1 (already set.)
;	mov		rcx, rcx		; rcx is used as cothreadj_core arg0 (already set.)
	push	r9				; push r9 home.
	push	r8				; push r8 home.
	push	rdx				; push rdx home.
	push	rcx				; push rcx home.
	call	cothreadj_core	; call cothreadj_core.
	add		rsp, (4*8)		; remove home registers from the stack.

	;---Restore the caller stack---
	mov		rsp, r13
	mov		rbp, r12

	;---Return---
	pop		r13
	pop		r12
	ret
END
