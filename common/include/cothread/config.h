/**
 * @brief		This file contains the detected configuration.
 * @file
 */

/**
 * @defgroup	doxy_cothread_config	configuration
 */

#ifndef __COTHREAD_CONFIG_H__
#define __COTHREAD_CONFIG_H__

/// @ingroup doxy_cothread_config
/// @{
#define COTHREAD_CC_ID_GCC			1	///< @brief	The GNU C compiler identifier.
#define COTHREAD_CC_ID_CLANG		2	///< @brief	The LLVM Clang compiler identifier.

#define COTHREAD_ARCH_ID_X86		1	///< @brief	The x86 architecture identifier.
#define COTHREAD_ARCH_ID_X86_64		2	///< @brief	The x86_64 architecture identifier.

#define COTHREAD_OS_ID_GNU_LINUX	1	///< @brief	The GNU/Linux operating system identifier.
#define COTHREAD_OS_ID_FREEBSD		2	///< @brief	The FreeBSD operating system identifier.
#define COTHREAD_OS_ID_MACOS		3	///< @brief	The macOS operating system identifier.
/// @}

#if		(defined(__GNUC__) && !defined(__clang__) && !defined(__MINGW32__))
	#define	COTHREAD_CC_ID	COTHREAD_CC_ID_GCC
#elif	(defined(__clang__))
	#define	COTHREAD_CC_ID	COTHREAD_CC_ID_CLANG
#endif

#if		((COTHREAD_CC_ID_GCC == COTHREAD_CC_ID) && defined(__i386__))
	#define	COTHREAD_ARCH_ID	COTHREAD_ARCH_ID_X86
#elif	((COTHREAD_CC_ID_GCC == COTHREAD_CC_ID) && defined(__x86_64__))
	#define	COTHREAD_ARCH_ID	COTHREAD_ARCH_ID_X86_64
#elif	((COTHREAD_CC_ID_CLANG == COTHREAD_CC_ID) && defined(__x86_64__))
	#define	COTHREAD_ARCH_ID	COTHREAD_ARCH_ID_X86_64
#endif

#if		((COTHREAD_CC_ID_GCC == COTHREAD_CC_ID) && defined(__gnu_linux__))
	#define	COTHREAD_OS_ID		COTHREAD_OS_ID_GNU_LINUX
#elif	((COTHREAD_CC_ID_CLANG == COTHREAD_CC_ID) && defined(__FreeBSD__))
	#define	COTHREAD_OS_ID		COTHREAD_OS_ID_FREEBSD
#elif	((COTHREAD_CC_ID_CLANG == COTHREAD_CC_ID) && defined(__APPLE__) && defined(__MACH__) && defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__))
	#define	COTHREAD_OS_ID		COTHREAD_OS_ID_MACOS
#endif

#if		(!defined(COTHREAD_CC_ID)	|| (0 == COTHREAD_CC_ID))
	#error	"compiler is not detected."
#elif	(!defined(COTHREAD_ARCH_ID)	|| (0 == COTHREAD_ARCH_ID))
	#error	"architecture is not detected."
#elif	(!defined(COTHREAD_OS_ID)	|| (0 == COTHREAD_OS_ID))
	#error	"operating system is not detected."
#endif










#if		(!0	\
		&& (COTHREAD_CC_ID_GCC			== COTHREAD_CC_ID)		\
		&& (COTHREAD_ARCH_ID_X86		== COTHREAD_ARCH_ID)	\
		&& (COTHREAD_OS_ID_GNU_LINUX	== COTHREAD_OS_ID)		\
		)
	#define COTHREAD_LINK_EXPORT
	#define	COTHREAD_LINK_HIDDEN	__attribute__ ((visibility ("hidden")))
	#define COTHREAD_CALL			__attribute__ ((cdecl))
#elif	(!0	\
		&& (COTHREAD_CC_ID_GCC			== COTHREAD_CC_ID)		\
		&& (COTHREAD_ARCH_ID_X86_64		== COTHREAD_ARCH_ID)	\
		&& (COTHREAD_OS_ID_GNU_LINUX	== COTHREAD_OS_ID)		\
		)
	#define COTHREAD_LINK_EXPORT
	#define	COTHREAD_LINK_HIDDEN	__attribute__ ((visibility ("hidden")))
	#define COTHREAD_CALL			__attribute__ ((sysv_abi))
#elif	(!0	\
		&& (COTHREAD_CC_ID_CLANG		== COTHREAD_CC_ID)		\
		&& (COTHREAD_ARCH_ID_X86_64		== COTHREAD_ARCH_ID)	\
		&& (COTHREAD_OS_ID_FREEBSD		== COTHREAD_OS_ID)		\
		)
	#define COTHREAD_LINK_EXPORT
	#define	COTHREAD_LINK_HIDDEN	__attribute__ ((visibility ("hidden")))
	#define COTHREAD_CALL			__attribute__ ((sysv_abi))
#elif	(!0	\
		&& (COTHREAD_CC_ID_CLANG		== COTHREAD_CC_ID)		\
		&& (COTHREAD_ARCH_ID_X86_64		== COTHREAD_ARCH_ID)	\
		&& (COTHREAD_OS_ID_MACOS		== COTHREAD_OS_ID)		\
		)
	#define COTHREAD_LINK_EXPORT
	#define	COTHREAD_LINK_HIDDEN	__attribute__ ((visibility ("hidden")))
	#define COTHREAD_CALL			__attribute__ ((sysv_abi))
#else
	#error	"configuration is not supported."
#endif

#ifndef COTHREAD_LINK
	#define	COTHREAD_LINK
#endif

#endif /* __COTHREAD_CONFIG_H__ */
