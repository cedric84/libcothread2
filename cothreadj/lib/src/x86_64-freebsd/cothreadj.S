.equ	COTHREADJ_ATTR_STACK,		+0
.equ	COTHREADJ_ATTR_STACK_SZ,	+8

.text

// calling conventions:
// https://gitlab.com/x86-psABIs/x86-64-ABI
//
// %rdi is used as arg0.
// %rsi is used as arg1.
//
// %rbp, %rbx, %r12-%r15 belong to the calling function and shall be preserved by the called one.
// the stack shall be 16-byte aligned immediately before the call instruction is executed.
//
// mov	src, dst
//
.global	cothreadj_init
.type	cothreadj_init, @function
cothreadj_init:
	//---Save registers---//
	push	%r12
	push	%r13

	//---Save the caller stack---//
	mov		%rbp, %r12
	mov		%rsp, %r13

	//---Setup the callee stack---//
	// Compute the past-the-end stack address
	mov		COTHREADJ_ATTR_STACK(%rsi), %rax		# store the lowest stack address in %rax.
	add		COTHREADJ_ATTR_STACK_SZ(%rsi), %rax		# %rax points the past-the-end stack address.
	// Setup the callee stack frame
	mov		%rax, %rbp								# load the stack base pointer with the past-the-end stack address.
	mov		%rbp, %rsp								# empty the stack.
	// from this point, stack is aligned on a 16-byte boundary (if the provided stack is well defined.)

	//---Initialize the cothread---//
//	mov		%rsi, %rsi		# %rsi is used as cothreadj_core arg1 (already set.)
//	mov		%rdi, %rdi		# %rdi is used as cothreadj_core arg0 (already set.)
	call	cothreadj_core	# call cothreadj_core.

	//---Restore the caller stack---//
	mov		%r13, %rsp
	mov		%r12, %rbp

	//---Return---//
	pop		%r13
	pop		%r12
	ret
