/**
 * @brief		This file contains the public declarations.
 * @file
 *
 * @defgroup	doxy_cothreadt		cothread - thread
 * @{
 *		@defgroup	doxy_cothreadt_unittest			C unittest
 * @}
 */

#ifndef __COTHREAD_COTHREADT_H__
#define __COTHREAD_COTHREADT_H__

#include <cothread/config.h>
#include <cothread/types.h>

//---Forward declarations---//
/// @ingroup doxy_cothreadt
/// @{
typedef struct _cothreadt_attr_t	cothreadt_attr_t;	///< @brief	The cothread attribute type.
typedef struct _cothreadt_t			cothreadt_t;		///< @brief	The cothread type.
/// @}

/**
 * @brief		Logs the specified message.
 * @param		[in]	_cothread	The cothread to log the message with.
 * @param		[in]	_fmt		The format of the message to log.
 * @param		[in]	...			The values to fill the format with.
 * @ingroup		doxy_cothreadt
 */
#define COTHREADT_LOGF(_cothread, _fmt, ...)	{	\
	fprintf(stderr, _fmt "\n", __VA_ARGS__);		\
}

/**
 * @brief		Logs the specified message and exits the process.
 * @param		[in]	_cothread	The cothread to log the message with.
 * @param		[in]	_fmt		The format of the message to log.
 * @param		[in]	...			The values to fill the format with.
 * @ingroup		doxy_cothreadt
 */
#define COTHREADT_LOGF_FATAL(_cothread, _fmt, ...)	{	\
	COTHREADT_LOGF(_cothread, _fmt, __VA_ARGS__);		\
	exit(EXIT_FAILURE);									\
}

/**
 * @brief		The cothread entry point.
 * @param		[in]	cothread	The cothread.
 * @ingroup		doxy_cothreadt
 */
typedef void (COTHREAD_CALL * cothreadt_cb_t) (cothreadt_t* cothread);

/**
 * @brief		The cothread attribute type.
 * @ingroup		doxy_cothreadt
 */
struct _cothreadt_attr_t
{
	cothreadt_cb_t	user_cb;	///< @brief	The cothread entry point.
};

#if		(0	\
		|| (COTHREAD_OS_ID_GNU_LINUX	== COTHREAD_OS_ID)	\
		|| (COTHREAD_OS_ID_FREEBSD		== COTHREAD_OS_ID)	\
		|| (COTHREAD_OS_ID_MACOS		== COTHREAD_OS_ID)	\
		)
	#include <cothread/cothreadt_posix.h>
#elif	(COTHREAD_OS_ID_WINDOWS	== COTHREAD_OS_ID)
	#include <cothread/cothreadt_windows.h>
#else
	#error "operating system is not supported."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief		Initializes the specified attributes.
 * @param		[in]	attr		The attributes to initialize.
 * @param		[in]	user_cb		The cothread entry point.
 * @relates		_cothreadt_attr_t
 */
extern COTHREAD_LINK void	COTHREAD_CALL cothreadt_attr_init	(cothreadt_attr_t* attr, cothreadt_cb_t user_cb);

/**
 * @brief		Uninitializes the specified cothread.
 * @param		[in]	cothread	The cothread to uninitialize.
 * @relates		_cothreadt_t
 */
extern COTHREAD_LINK void	COTHREAD_CALL cothreadt_uninit	(cothreadt_t* cothread);

/**
 * @brief		Initializes the specified cothread.
 * @param		[in]	cothread	The cothread to initialize.
 * @param		[in]	attr		The attributes to initialize the cothread with.
 * @note		Modifying @e attr after calling this function has no effect on the initialized @e cothread.
 * @return		Returns
 *				- @ref cothread_err_ok in case of success ;
 *				- @ref cothread_err_notsup otherwise.
 *				.
 * @relates		_cothreadt_t
 */
extern COTHREAD_LINK cothread_err_t		COTHREAD_CALL cothreadt_init	(cothreadt_t* cothread, const cothreadt_attr_t* attr);

/**
 * @brief		Stores the specified user data in the specified cothread.
 * @param		[in]	cothread	The cothread to store the user data in.
 * @param		[in]	user_data	Any user data to store in the cothread.
 * @relates		_cothreadt_t
 */
extern COTHREAD_LINK void	COTHREAD_CALL cothreadt_set_user_data	(cothreadt_t* cothread, void* user_data);

/**
 * @brief		Returns the user data stored in the specified cothread.
 * @param		[in]	cothread	The cothread to return the user data stored in.
 * @return		Returns the user data.
 * @relates		_cothreadt_t
 */
extern COTHREAD_LINK void*	COTHREAD_CALL cothreadt_get_user_data	(const cothreadt_t* cothread);

/**
 * @brief		Switches from the current context to the other one.
 * @param		[in]	cothread	The cothread to switch.
 * @relates		_cothreadt_t
 */
extern COTHREAD_LINK void				COTHREAD_CALL cothreadt_yield	(cothreadt_t* cothread);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __COTHREAD_COTHREADT_H__ */
