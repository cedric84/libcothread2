/**
 * @brief		This file contains the public declarations.
 * @file
 *
 * @defgroup	doxy_cothreadj		cothread - setjmp / longjmp
 * @{
 *		@defgroup	doxy_cothreadj_unittest		unittest
 * @}
 */

#ifndef __COTHREAD_COTHREADJ_H__
#define __COTHREAD_COTHREADJ_H__

#include <cothread/config.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

//---Forward declarations---//
/// @ingroup doxy_cothreadj
/// @{
typedef	struct _cothreadj_stack_t	cothreadj_stack_t;	///< @brief	The stack type.
typedef struct _cothreadj_attr_t	cothreadj_attr_t;	///< @brief	The cothread attribute type.
typedef struct _cothreadj_ep_t		cothreadj_ep_t;		///< @brief	The cothread endpoint type.
typedef struct _cothreadj_t			cothreadj_t;		///< @brief	The cothread type.
/// @}

//---Stack type detection---//
/// @ingroup doxy_cothreadj
/// @{
#if		(!0	\
		&& (COTHREAD_CC_ID_GCC			== COTHREAD_CC_ID)		\
		&& (COTHREAD_ARCH_ID_X86		== COTHREAD_ARCH_ID)	\
		&& (COTHREAD_OS_ID_GNU_LINUX	== COTHREAD_OS_ID)		\
		)
	#define COTHREADJ_STACK_ALIGN	16	///< @brief	The stack alignment.
	/** @cond */ struct _cothreadj_stack_t { char buf; } __attribute__ ((aligned (COTHREADJ_STACK_ALIGN)));	/** @endcond */
#elif	(!0	\
		&& (COTHREAD_CC_ID_GCC			== COTHREAD_CC_ID)		\
		&& (COTHREAD_ARCH_ID_X86_64		== COTHREAD_ARCH_ID)	\
		&& (COTHREAD_OS_ID_GNU_LINUX	== COTHREAD_OS_ID)		\
		)
	#define COTHREADJ_STACK_ALIGN	16	///< @brief	The stack alignment.
	/** @cond */ struct _cothreadj_stack_t { char buf; } __attribute__ ((aligned (COTHREADJ_STACK_ALIGN)));	/** @endcond */
#elif	(!0	\
		&& (COTHREAD_CC_ID_CLANG		== COTHREAD_CC_ID)		\
		&& (COTHREAD_ARCH_ID_X86_64		== COTHREAD_ARCH_ID)	\
		&& (COTHREAD_OS_ID_FREEBSD		== COTHREAD_OS_ID)		\
		)
	#define COTHREADJ_STACK_ALIGN	16	///< @brief	The stack alignment.
	/** @cond */ struct _cothreadj_stack_t { char buf; } __attribute__ ((aligned (COTHREADJ_STACK_ALIGN)));	/** @endcond */
#else
	#error	"stack type is not detected."
#endif
/// @}

/**
 * @brief		Rounds the specified stack size upward to make it a multiple of @ref COTHREADJ_STACK_ALIGN.
 * @param		[in]	_sz		The stack size to round.
 * @return		Returns the rounded value.
 * @ingroup		doxy_cothreadj
 */
#define COTHREADJ_ROUND_STACK_SZ(_sz)	((((_sz) + (COTHREADJ_STACK_ALIGN - 1)) / COTHREADJ_STACK_ALIGN) * COTHREADJ_STACK_ALIGN)

/**
 * @brief		The callee entry point.
 * @param		[in]	cothread	The cothread.
 * @param		[in]	user_val	Any user value (except zero) received from the other endpoint.
 * @return		Returns any user value (except zero) to send to the other endpoint.
 * @ingroup		doxy_cothreadj
 */
typedef int (COTHREAD_CALL * cothreadj_cb_t) (cothreadj_t* cothread, int user_val);

/**
 * @brief		The cothread attribute type.
 * @ingroup		doxy_cothreadj
 */
struct _cothreadj_attr_t
{
	cothreadj_stack_t*	stack;				///< @brief	The lowest address of the callee stack.
	size_t				stack_sz;			///< @brief	The size of the callee stack, in bytes.
	cothreadj_cb_t		user_cb;			///< @brief	The callee entry point.
	//
	const char*			dbg_caller_name;	///< @brief	The caller debug name, may be NULL, no internal copy is done.
	const char*			dbg_callee_name;	///< @brief	The callee debug name, may be NULL, no internal copy is done.
	FILE*				dbg_strm;			///< @brief	The stream to log debug informations to, may be NULL.
};

/**
 * @brief		The cothread endpoint type.
 * @ingroup		doxy_cothreadj
 */
struct _cothreadj_ep_t
{
	jmp_buf		buf;		///< @brief	The execution context.
	const char*	dbg_name;	///< @brief	The debug name, never NULL.
};

/**
 * @brief		The cothread type.
 * @ingroup		doxy_cothreadj
 */
struct _cothreadj_t
{
	cothreadj_ep_t*		current;	///< @brief	Points the current endpoint.
	cothreadj_ep_t		caller;		///< @brief	The caller endpoint.
	cothreadj_ep_t		callee;		///< @brief	The callee endpoint.
	void*				user_data;	///< @brief	Any user data.
	FILE*				dbg_strm;	///< @brief	The stream to log debug informations to, may be NULL.
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief		Initializes the specified attributes.
 * @param		[in]	attr		The attributes to initialize.
 * @param		[in]	stack		The lowest address of the callee stack (must be @ref COTHREADJ_STACK_ALIGN aligned.)
 * @param		[in]	stack_sz	The size of the callee stack, in bytes
 *									(must be a multiple of @ref COTHREADJ_STACK_ALIGN, see @ref COTHREADJ_ROUND_STACK_SZ.)
 * @param		[in]	user_cb		The callee entry point.
 * @relates		_cothreadj_attr_t
 */
extern COTHREAD_LINK void		COTHREAD_CALL cothreadj_attr_init	(cothreadj_attr_t* attr, cothreadj_stack_t* stack, size_t stack_sz, cothreadj_cb_t user_cb);

/**
 * @brief		Sets a debug name for the caller.
 * @param		[in]	attr	The attributes to store the debug name in.
 * @param		[in]	name	The caller debug name, may be NULL, no internal copy is done.
 * @relates		_cothreadj_attr_t
 */
extern COTHREAD_LINK void		COTHREAD_CALL cothreadj_attr_set_dbg_caller_name	(cothreadj_attr_t* attr, const char* name);

/**
 * @brief		Sets a debug name for the callee.
 * @param		[in]	attr	The attributes to store the debug name in.
 * @param		[in]	name	The callee debug name, may be NULL, no internal copy is done.
 * @relates		_cothreadj_attr_t
 */
extern COTHREAD_LINK void		COTHREAD_CALL cothreadj_attr_set_dbg_callee_name	(cothreadj_attr_t* attr, const char* name);

/**
 * @brief		Sets the stream to log debug informations to.
 * @param		[in]	attr	The attributes to store the stream in.
 * @param		[in]	strm	The stream to log the debug informations to, may be NULL.
 * @relates		_cothreadj_attr_t
 */
extern COTHREAD_LINK void		COTHREAD_CALL cothreadj_attr_set_dbg_strm	(cothreadj_attr_t* attr, FILE* strm);

/**
 * @brief		Initializes the specified cothread.
 * @param		[in]	cothread	The cothread to initialize.
 * @param		[in]	attr		The attributes to initialize the cothread with.
 * @note		Modifying @e attr after calling this function has no effect on the initialized @e cothread.
 * @relates		_cothreadj_t
 */
extern COTHREAD_LINK void		COTHREAD_CALL cothreadj_init	(cothreadj_t* cothread, const cothreadj_attr_t* attr);

/**
 * @brief		Stores the specified user data in the specified cothread.
 * @param		[in]	cothread	The cothread to store the user data in.
 * @param		[in]	user_data	Any user data to store in the cothread.
 * @relates		_cothreadj_t
 */
extern COTHREAD_LINK void		COTHREAD_CALL cothreadj_set_user_data	(cothreadj_t* cothread, void* user_data);

/**
 * @brief		Returns the user data stored in the specified cothread.
 * @param		[in]	cothread	The cothread to return the user data stored in.
 * @return		Returns the user data.
 * @relates		_cothreadj_t
 */
extern COTHREAD_LINK void*		COTHREAD_CALL cothreadj_get_user_data	(const cothreadj_t* cothread);

/**
 * @brief		Switches from the current endpoint to the other one.
 * @param		[in]	cothread	The cothread to switch.
 * @param		[in]	user_val	Any user value (except zero) to send to the other endpoint.
 * @return		Returns the @e user_val received from the other endpoint.
 * @relates		_cothreadj_t
 */
extern COTHREAD_LINK int		COTHREAD_CALL cothreadj_yield	(cothreadj_t* cothread, int user_val);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __COTHREAD_COTHREADJ_H__ */
