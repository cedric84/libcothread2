/**
 * @brief		This file contains the public declarations for the POSIX implementation.
 * @file
 */

#ifndef __COTHREAD_COTHREADT_POSIX_H__
#define __COTHREAD_COTHREADT_POSIX_H__

#include <pthread.h>

/**
 * @brief		The cothread state type.
 * @ingroup		doxy_cothreadt
 */
enum _cothreadt_state_t
{
	cothreadt_state_resumed,	///< @brief	The cothread is running.
	cothreadt_state_paused,		///< @brief	The cothread is paused.
};

/**
 * @brief		The cothread state type.
 * @ingroup		doxy_cothreadt
 */
typedef enum _cothreadt_state_t		cothreadt_state_t;

/**
 * @brief		The cothread type.
 * @ingroup		doxy_cothreadt
 */
struct _cothreadt_t
{
	pthread_t			thread;		///< @brief	The thread.
	pthread_mutex_t		mtx;		///< @brief	The mutex.
	pthread_cond_t		cond;		///< @brief	The condition variable.
	cothreadt_state_t	state;		///< @brief	The current state.

	cothreadt_cb_t		user_cb;	///< @brief	The cothread entry point.
	void*				user_data;	///< @brief	Any user data.
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __COTHREAD_COTHREADT_POSIX_H__ */
