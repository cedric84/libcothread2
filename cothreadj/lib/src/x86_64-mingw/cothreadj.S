.equ	COTHREADJ_ATTR_STACK,		+0
.equ	COTHREADJ_ATTR_STACK_SZ,	+8

.text

// calling conventions:
// https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170
//
// %rcx is used as arg0.
// %rdx is used as arg1.
//
// %r12-%r15, %rdi, %rsi, %rbx, %rbp, %rsp belong to the calling function and shall be preserved by the called one.
// the stack shall be 16-byte aligned immediately before the call instruction is executed.
// %r9 home, %r8 home, %rdx home & %rcx home are always pushed (in this order) before the call instruction is executed.
//
// mov	src, dst
//
.global	cothreadj_init
cothreadj_init:
	//---Save registers---//
	push	%r12
	push	%r13

	//---Save the caller stack---//
	mov		%rbp, %r12
	mov		%rsp, %r13

	//---Setup the callee stack---//
	// Compute the past-the-end stack address
	mov		COTHREADJ_ATTR_STACK(%rdx), %rax		# store the lowest stack address in %rax.
	add		COTHREADJ_ATTR_STACK_SZ(%rdx), %rax		# %rax points the past-the-end stack address.
	// Setup the callee stack frame
	mov		%rax, %rbp								# load the stack base pointer with the past-the-end stack address.
	mov		%rbp, %rsp								# empty the stack.
	// from this point, stack is aligned on a 16-byte boundary (if the provided stack is well defined.)

	//---Initialize the cothread---//
//	mov		%rdx, %rdx		# %rdx is used as cothreadj_core arg1 (already set.)
//	mov		%rcx, %rcx		# %rcx is used as cothreadj_core arg0 (already set.)
	push	%r9				# push %r9 home.
	push	%r8				# push %r8 home.
	push	%rdx			# push %rdx home.
	push	%rcx			# push %rcx home.
	call	cothreadj_core	# call cothreadj_core.
	add		$(4*8), %rsp	# remove home registers from the stack.

	//---Restore the caller stack---//
	mov		%r13, %rsp
	mov		%r12, %rbp

	//---Return---//
	pop		%r13
	pop		%r12
	ret
