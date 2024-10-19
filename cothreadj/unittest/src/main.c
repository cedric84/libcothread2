/**
 * @brief		This file contains the application entry point.
 * @file
 */

#include <unittest.h>
#include <string.h>
#include <stdint.h>

/**
 * @brief		Checks the configuration.
 * @param		[in]	prefix	The prefix to check, may be NULL.
 * @ingroup		doxy_cothreadj_unittest
 */
static void COTHREAD_CALL
check_config(const char* prefix)
{
	//---Definitions---//
	static const char*	cc_name		= COTHREAD_CC_ID_GCC			== COTHREAD_CC_ID ? "gcc"
									: COTHREAD_CC_ID_CLANG			== COTHREAD_CC_ID ? "clang"
									: COTHREAD_CC_ID_MINGW			== COTHREAD_CC_ID ? "mingw"
									: COTHREAD_CC_ID_CL				== COTHREAD_CC_ID ? "cl"
									: "???";

	static const char*	arch_name	= COTHREAD_ARCH_ID_X86			== COTHREAD_ARCH_ID ? "x86"
									: COTHREAD_ARCH_ID_X86_64		== COTHREAD_ARCH_ID ? "x86_64"
									: "???";

	static const char*	os_name		= COTHREAD_OS_ID_GNU_LINUX		== COTHREAD_OS_ID ? "gnu_linux"
									: COTHREAD_OS_ID_FREEBSD		== COTHREAD_OS_ID ? "freebsd"
									: COTHREAD_OS_ID_MACOS			== COTHREAD_OS_ID ? "macos"
									: COTHREAD_OS_ID_WINDOWS		== COTHREAD_OS_ID ? "windows"
									: "???";

	//---Check the configuration---//
	assert('?'	!= cc_name[0]);
	assert('?'	!= arch_name[0]);
	assert('?'	!= os_name[0]);

	//---Check the prefix if any---//
	if (NULL == prefix) {
		// do nothing.
	} else if (0 == strcmp(prefix, "x86-gnu_linux")) {
		assert(COTHREAD_CC_ID_GCC		== COTHREAD_CC_ID);
		assert(COTHREAD_ARCH_ID_X86		== COTHREAD_ARCH_ID);
		assert(COTHREAD_OS_ID_GNU_LINUX	== COTHREAD_OS_ID);
	} else if (0 == strcmp(prefix, "x86_64-gnu_linux")) {
		assert(COTHREAD_CC_ID_GCC		== COTHREAD_CC_ID);
		assert(COTHREAD_ARCH_ID_X86_64	== COTHREAD_ARCH_ID);
		assert(COTHREAD_OS_ID_GNU_LINUX	== COTHREAD_OS_ID);
	} else if (0 == strcmp(prefix, "x86_64-freebsd")) {
		assert(COTHREAD_CC_ID_CLANG		== COTHREAD_CC_ID);
		assert(COTHREAD_ARCH_ID_X86_64	== COTHREAD_ARCH_ID);
		assert(COTHREAD_OS_ID_FREEBSD	== COTHREAD_OS_ID);
	} else if (0 == strcmp(prefix, "x86_64-macos")) {
		assert(COTHREAD_CC_ID_CLANG		== COTHREAD_CC_ID);
		assert(COTHREAD_ARCH_ID_X86_64	== COTHREAD_ARCH_ID);
		assert(COTHREAD_OS_ID_MACOS		== COTHREAD_OS_ID);
	} else if (0 == strcmp(prefix, "x86-mingw")) {
		assert(COTHREAD_CC_ID_MINGW		== COTHREAD_CC_ID);
		assert(COTHREAD_ARCH_ID_X86		== COTHREAD_ARCH_ID);
		assert(COTHREAD_OS_ID_WINDOWS	== COTHREAD_OS_ID);
	} else if (0 == strcmp(prefix, "x86_64-mingw")) {
		assert(COTHREAD_CC_ID_MINGW		== COTHREAD_CC_ID);
		assert(COTHREAD_ARCH_ID_X86_64	== COTHREAD_ARCH_ID);
		assert(COTHREAD_OS_ID_WINDOWS	== COTHREAD_OS_ID);
	} else if (0 == strcmp(prefix, "x86-windows")) {
		assert(COTHREAD_CC_ID_CL		== COTHREAD_CC_ID);
		assert(COTHREAD_ARCH_ID_X86		== COTHREAD_ARCH_ID);
		assert(COTHREAD_OS_ID_WINDOWS	== COTHREAD_OS_ID);
	} else if (0 == strcmp(prefix, "x86_64-windows")) {
		assert(COTHREAD_CC_ID_CL		== COTHREAD_CC_ID);
		assert(COTHREAD_ARCH_ID_X86_64	== COTHREAD_ARCH_ID);
		assert(COTHREAD_OS_ID_WINDOWS	== COTHREAD_OS_ID);
	} else {
		assert(0);
	}

	//---Log the configuration---//
	static const int	align	= 16;
	printf("%*s: %s\n", align, "compiler", cc_name);
	printf("%*s: %s\n", align, "architecture", arch_name);
	printf("%*s: %s\n", align, "operating system", os_name);
}

/**
 * @brief		Checks the macros.
 * @ingroup		doxy_cothreadj_unittest
 */
static void COTHREAD_CALL
check_macros(void)
{
	//---Check stack size rounding macro---//
	assert((0 * COTHREADJ_STACK_ALIGN)	== COTHREADJ_ROUND_STACK_SZ(0 * COTHREADJ_STACK_ALIGN + 0));
	assert((1 * COTHREADJ_STACK_ALIGN)	== COTHREADJ_ROUND_STACK_SZ(0 * COTHREADJ_STACK_ALIGN + 1));
	assert((1 * COTHREADJ_STACK_ALIGN)	== COTHREADJ_ROUND_STACK_SZ(1 * COTHREADJ_STACK_ALIGN - 1));
	assert((1 * COTHREADJ_STACK_ALIGN)	== COTHREADJ_ROUND_STACK_SZ(1 * COTHREADJ_STACK_ALIGN + 0));
	assert((2 * COTHREADJ_STACK_ALIGN)	== COTHREADJ_ROUND_STACK_SZ(1 * COTHREADJ_STACK_ALIGN + 1));
	assert((2 * COTHREADJ_STACK_ALIGN)	== COTHREADJ_ROUND_STACK_SZ(2 * COTHREADJ_STACK_ALIGN - 1));
	assert((2 * COTHREADJ_STACK_ALIGN)	== COTHREADJ_ROUND_STACK_SZ(2 * COTHREADJ_STACK_ALIGN + 0));
	assert((3 * COTHREADJ_STACK_ALIGN)	== COTHREADJ_ROUND_STACK_SZ(2 * COTHREADJ_STACK_ALIGN + 1));
}

/**
 * @brief		Checks the @ref _cothreadj_attr_t initialization functions.
 * @ingroup		doxy_cothreadj_unittest
 */
static void COTHREAD_CALL
check_attr_init(void)
{
	//---Check structure member offsets used in assembly code---//
	if (COTHREAD_ARCH_ID_X86 == COTHREAD_ARCH_ID) {
		assert(0	== (uintptr_t)&(((cothreadj_attr_t*)0)->stack));
		assert(4	== (uintptr_t)&(((cothreadj_attr_t*)0)->stack_sz));
	} else if (COTHREAD_ARCH_ID_X86_64 == COTHREAD_ARCH_ID) {
		assert(0	== (uintptr_t)&(((cothreadj_attr_t*)0)->stack));
		assert(8	== (uintptr_t)&(((cothreadj_attr_t*)0)->stack_sz));
	} else {
		assert(0);
	}

	//---Define a stack---//
	cothreadj_stack_t	stack[COTHREADJ_ROUND_STACK_SZ(1) / sizeof(cothreadj_stack_t)];
	assert(0	== ((COTHREADJ_STACK_ALIGN - 1) & (uintptr_t)&stack));

	//---Check the initialization function---//
	cothreadj_attr_t	attr;
	memset(&attr, 0xFFU, sizeof(attr));
	cothreadj_attr_init(&attr, stack, sizeof(stack), (cothreadj_cb_t)0x1234);
	assert(stack					== attr.stack);
	assert(COTHREADJ_STACK_ALIGN	== attr.stack_sz);
	assert((cothreadj_cb_t)0x1234	== attr.user_cb);
	//
	assert(NULL						== attr.dbg_caller_name);
	assert(NULL						== attr.dbg_callee_name);
	assert(NULL						== attr.dbg_strm);

	//---Check debug functions---//
	assert(NULL						== attr.dbg_caller_name);
	cothreadj_attr_set_dbg_caller_name(&attr, (const char*)0x1122);
	assert((const char*)0x1122		== attr.dbg_caller_name);
	//
	assert(NULL						== attr.dbg_callee_name);
	cothreadj_attr_set_dbg_callee_name(&attr, (const char*)0x3344);
	assert((const char*)0x3344		== attr.dbg_callee_name);
	//
	assert(NULL						== attr.dbg_strm);
	cothreadj_attr_set_dbg_strm(&attr, (FILE*)0x5566);
	assert((FILE*)0x5566			== attr.dbg_strm);
}

/**
 * @brief		Checks the @ref _cothreadj_t initialization functions.
 * @ingroup		doxy_cothreadj_unittest
 */
static void COTHREAD_CALL
check_cothread_init(void)
{
	//---Initialize the attributes---//
	cothreadj_stack_t	stack[(2 * 1024) / sizeof(cothreadj_stack_t)];
	cothreadj_attr_t	attr;
	cothreadj_attr_init(&attr, stack, sizeof(stack), (cothreadj_cb_t)0x1234);

	//---Initialize the cothread---//
	cothreadj_t	cothread;
	memset(&cothread, 0xFFU, sizeof(cothread));
	cothread.user_data	= (void*)0x1234;
	cothreadj_init(&cothread, &attr);
	assert(&(cothread.caller)	== cothread.current);
	assert(NULL					!= cothread.caller.dbg_name);
	assert(0					== strcmp("caller", cothread.caller.dbg_name));
	assert(NULL					!= cothread.callee.dbg_name);
	assert(0					== strcmp("callee", cothread.callee.dbg_name));
	assert((void*)0x1234		== cothread.user_data);
	assert(NULL					== cothread.dbg_strm);

	//---Check user data functions---//
	assert((void*)0x1234		== cothread.user_data);
	assert((void*)0x1234		== cothreadj_get_user_data(&cothread));
	cothreadj_set_user_data(&cothread, (void*)0x5678);
	assert((void*)0x5678		== cothread.user_data);
	assert((void*)0x5678		== cothreadj_get_user_data(&cothread));
}

/**
 * @brief		The application entry point.
 * @param		[in]	argc		The number of arguments.
 * @param		[in]	argv		The arguments values.
 * @return		Returns zero on success.
 * @ingroup		doxy_cothreadj_unittest
 */
extern int
main(int argc, char* argv[])
{
	printf("%s started\n", __func__);

	assert((1	<= argc) && (argc <= 2));
	const char*	prefix	= (2 <= argc) ? argv[1] : NULL;

	check_config(prefix);
	check_macros();
	check_attr_init();
	check_cothread_init();
	unittest0();
	unittest1();
	printf("%s terminated\n", __func__);
	return 0;
}
