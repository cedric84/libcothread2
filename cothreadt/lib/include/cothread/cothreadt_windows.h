/**
 * @brief		This file contains the public declarations for the WINDOWS implementation.
 * @file
 */

#ifndef __COTHREAD_COTHREADT_WINDOWS_H__
#define __COTHREAD_COTHREADT_WINDOWS_H__

#include <windows.h>

/**
 * @brief		The cothread type.
 * @ingroup		doxy_cothreadt
 */
struct _cothreadt_t
{
	HANDLE				caller;		///< @brief	The event the caller is waiting for.
	HANDLE				callee;		///< @brief	The event the callee is waiting for.
	HANDLE				thread_hdl;	///< @brief	The thread handle.
	DWORD				thread_id;	///< @brief	The thread identifier.
	unsigned int		flags;		///< @brief	Several flags.

	cothreadt_cb_t		user_cb;	///< @brief	The cothread entry point.
	void*				user_data;	///< @brief	Any user data.
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __COTHREAD_COTHREADT_WINDOWS_H__ */
