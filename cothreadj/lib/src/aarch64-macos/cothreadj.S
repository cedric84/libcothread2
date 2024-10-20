.equ	COTHREADJ_ATTR_STACK,		+0
.equ	COTHREADJ_ATTR_STACK_SZ,	+8

.text

// calling conventions:
// https://developer.apple.com/documentation/xcode/writing-arm64-code-for-apple-platforms
// https://github.com/ARM-software/abi-aa/releases (aapcs64.pdf, §6 "The Base Procedure Call Standard")
//
// r30			link register.
// r29			frame pointer.
// r19...r28	callee-saved registers.
// r18			platform register, reserved by operating system. Don’t use this register.
// r9...r15		temporary registers.
// r8			indirect result location register.
// r0...r7		parameter (arg0...arg7) registers.
// r0			result register.
// a subroutine invocation must preserve the contents of the registers r19-r29 and SP.
// the stack shall be 16-byte aligned immediately before the call instruction is executed.
//
// mov	dst, src
//
.global	_cothreadj_init
.p2align 4
_cothreadj_init:
	//---Save registers---//
	sub	sp, sp, #32		// allocate memory from the stack (16-byte aligned.)
	str	lr, [sp, #0]	// save link register.
	str x19, [sp, #8]	// save x19 register.
	str x20, [sp, #16]	// save x20 register.

	//---Save the caller stack---//
	mov	x19, fp
	mov	x20, sp

	//---Setup the callee stack---//
	// Compute the past-the-end stack address
	ldr	x2, [x1, #COTHREADJ_ATTR_STACK]		// store the lowest stack address in x2.
	ldr	x3, [x1, #COTHREADJ_ATTR_STACK_SZ]	// store the stack size in x3.
	add	x2, x2, x3							// x2 points the past-the-end stack address.
	// Setup the callee stack frame
	mov	fp, x2								// load the frame pointer with the past-the-end stack address.
	mov	sp, fp								// empty the stack.

	//---Initialize the cothread---//
//	mov	x1, x1				// x1 is used as cothreadj_core arg1 (already set.)
//	mov	x0, x0				// x0 is used as cothreadj_core arg0 (already set.)
	bl	_cothreadj_core		// call cothreadj_core.

	//---Restore the caller stack---//
	mov	sp, x20
	mov	fp, x19

	//---Restore registers---//
	ldr x20, [sp, #16]	// restore x20 register.
	ldr x19, [sp, #8]	// restore x19 register.
	ldr	lr, [sp, #0]	// restore link register.
	add	sp, sp, #32		// deallocate memory from the stack.

	//---Return---//
	ret
