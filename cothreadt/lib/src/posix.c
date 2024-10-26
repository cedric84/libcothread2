/**
 * @brief		This file contains the definitions for the POSIX implementation.
 * @file
 */

#include <cothread/cothreadt.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/// @ingroup doxy_cothreadt
/// @{
#define COTHREADT_FLAG_ABORTABLE	(1 << 0)	///< @brief	Says whether the cothread may be aborted or not.
#define COTHREADT_FLAG_ABORTING		(1 << 1)	///< @brief	Says whether the cothread shall abort or not.
/// @}

/**
 * @brief		Unlocks the specified cothread.
 * @param		[in]	cothread	The cothread to unlock.
 * @relates		_cothreadt_t
 */
static inline void COTHREAD_CALL
cothreadt_unlock(cothreadt_t* cothread)
{
	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Unlock the mutex---//
	const int	pthread_ret	= pthread_mutex_unlock(&(cothread->mtx));
	if (0 != pthread_ret) {
		COTHREADT_LOGF_FATAL(cothread, "cannot unlock cothread (%d)", pthread_ret);
	}
}

/**
 * @brief		Locks the specified cothread.
 * @param		[in]	cothread	The cothread to lock.
 * @relates		_cothreadt_t
 */
static inline void COTHREAD_CALL
cothreadt_lock(cothreadt_t* cothread)
{
	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Lock the mutex---//
	const int	pthread_ret	= pthread_mutex_lock(&(cothread->mtx));
	if (0 != pthread_ret) {
		COTHREADT_LOGF_FATAL(cothread, "cannot lock cothread (%d)", pthread_ret);
	}
}

/**
 * @brief		Waits for the specified cothread to be signaled.
 * @param		[in]	cothread	The cothread to wait for.
 * @relates		_cothreadt_t
 */
static inline void COTHREAD_CALL
cothreadt_wait(cothreadt_t* cothread)
{
	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Wait---//
	const int	pthread_ret	= pthread_cond_wait(&(cothread->cond), &(cothread->mtx));
	if (0 != pthread_ret) {
		COTHREADT_LOGF_FATAL(cothread, "cannot wait for cothread (%d)", pthread_ret);
	}
}

/**
 * @brief		Signals the specified cothread.
 * @param		[in]	cothread	The cothread to signal.
 * @relates		_cothreadt_t
 */
static inline void COTHREAD_CALL
cothreadt_signal(cothreadt_t* cothread)
{
	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Signal---//
	const int	pthread_ret	= pthread_cond_signal(&(cothread->cond));
	if (0 != pthread_ret) {
		COTHREADT_LOGF_FATAL(cothread, "cannot signal cothread (%d)", pthread_ret);
	}
}

/**
 * @brief		The thread entry point.
 * @param		[in]	arg		The cothread.
 * @return		Returns NULL.
 * @relates		_cothreadt_t
 */
static void*
cothreadt_thd_cb(void* arg)
{
	//---Definitions---//
	cothreadt_t*	cothread	= (cothreadt_t*)arg;

	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Wait for the thread to be resumed---//
	cothreadt_lock(cothread);
	while (cothreadt_state_resumed != cothread->state) {
		cothreadt_wait(cothread);
	}
	cothreadt_unlock(cothread);

	//---Run the user callback if no abortion is pending---//
	if (0 == (COTHREADT_FLAG_ABORTING & cothread->flags)) {
		cothread->flags	&= ~COTHREADT_FLAG_ABORTABLE;
		cothread->user_cb(cothread);
	}

	//---Return to caller---//
	cothreadt_lock(cothread);
	assert(cothreadt_state_resumed	== cothread->state);
	cothread->state	= cothreadt_state_paused;
	cothreadt_signal(cothread);
	cothreadt_unlock(cothread);

	//---Return---//
	return NULL;
}

/**
 * @brief		Aborts the execution of the specified cothread.
 * @param		[in]	cothread	The cothread to abort the execution of.
 * @relates		_cothreadt_t
 */
static void COTHREAD_CALL
cothreadt_abort(cothreadt_t* cothread)
{
	//---Check arguments---//
	assert(NULL	!= cothread);
	assert(0	== pthread_equal(pthread_self(), cothread->thread));

	//---Abort---//
	assert(COTHREADT_FLAG_ABORTABLE	== (COTHREADT_FLAG_ABORTABLE & cothread->flags));
	assert(0						== (COTHREADT_FLAG_ABORTING & cothread->flags));
	cothread->flags	|= COTHREADT_FLAG_ABORTING;

	//---Wake the thread up---//
	cothreadt_lock(cothread);
	assert(cothreadt_state_paused	== cothread->state);
	cothread->state	= cothreadt_state_resumed;
	cothreadt_signal(cothread);
	cothreadt_unlock(cothread);
}

extern COTHREAD_LINK void COTHREAD_CALL
cothreadt_uninit(cothreadt_t* cothread)
{
	//---Definitions---//
	int	pthread_ret;

	//---Check arguments---//
	assert(NULL	!= cothread);

	//---May the cothread be aborted ?---//
	if (COTHREADT_FLAG_ABORTABLE == (COTHREADT_FLAG_ABORTABLE & cothread->flags)) {
		cothreadt_abort(cothread);
	}

	//---Join the thread---//
	if (0 != (pthread_ret = pthread_join(cothread->thread, NULL))) {
		COTHREADT_LOGF_FATAL(cothread, "cannot join thread (%d)", pthread_ret);
	}

	//---Destroy the condition variable---//
	if (0 != (pthread_ret = pthread_cond_destroy(&(cothread->cond)))) {
		COTHREADT_LOGF_FATAL(cothread, "cannot destroy condition variable (%d)", pthread_ret);
	}

	//---Destroy the mutex---//
	if (0 != (pthread_ret = pthread_mutex_destroy(&(cothread->mtx)))) {
		COTHREADT_LOGF_FATAL(cothread, "cannot destroy mutex (%d)", pthread_ret);
	}
}

extern COTHREAD_LINK cothread_err_t COTHREAD_CALL
cothreadt_init(cothreadt_t* cothread, const cothreadt_attr_t* attr)
{
	//---Definitions---//
	cothread_err_t	err	= cothread_err_notsup;
	int				pthread_ret;

	//---Check arguments---//
	assert(NULL	!= cothread);
	assert(NULL	!= attr);

	//---Zero---//
	cothread->state		= cothreadt_state_paused;
	cothread->flags		= COTHREADT_FLAG_ABORTABLE;
	cothread->user_cb	= attr->user_cb;

	//---Initialize the mutex---//
	if (0 != (pthread_ret = pthread_mutex_init(&(cothread->mtx), NULL))) {
		COTHREADT_LOGF(cothread, "cannot initialize mutex (%d)", pthread_ret);
	} else {
		//---Initialize the condition variable---//
		if (0 != (pthread_ret = pthread_cond_init(&(cothread->cond), NULL))) {
			COTHREADT_LOGF(cothread, "cannot initialize condition variable (%d)", pthread_ret);
		} else {
			//---Create the thread---//
			if (0 != (pthread_ret = pthread_create(&(cothread->thread), NULL, cothreadt_thd_cb, cothread))) {
				COTHREADT_LOGF(cothread, "cannot create thread (%d)", pthread_ret);
			} else {
				//---Update the error code---//
				err	= cothread_err_ok;
			}

			//---Error Management---//
			if (COTHREAD_ERR_ISNOK(err)) {
				if (0 != (pthread_ret = pthread_cond_destroy(&(cothread->cond)))) {
					COTHREADT_LOGF_FATAL(cothread, "cannot destroy condition variable (%d)", pthread_ret);
				}
			}
		}

		//---Error Management---//
		if (COTHREAD_ERR_ISNOK(err)) {
			if (0 != (pthread_ret = pthread_mutex_destroy(&(cothread->mtx)))) {
				COTHREADT_LOGF_FATAL(cothread, "cannot destroy mutex (%d)", pthread_ret);
			}
		}
	}

	//---Return---//
	return err;
}

extern COTHREAD_LINK void COTHREAD_CALL
cothreadt_set_user_data(cothreadt_t* cothread, void* user_data)
{
	assert(NULL	!= cothread);
	cothread->user_data	= user_data;
}

extern COTHREAD_LINK void* COTHREAD_CALL
cothreadt_get_user_data(const cothreadt_t* cothread)
{
	assert(NULL	!= cothread);
	return cothread->user_data;
}

extern COTHREAD_LINK void COTHREAD_CALL
cothreadt_yield(cothreadt_t* cothread)
{
	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Lock---//
	cothreadt_lock(cothread);

	//---Get the current state---//
	const cothreadt_state_t running_state	= cothread->state;
	const int				is_callee		= (0 != pthread_equal(pthread_self(), cothread->thread));
	assert((cothreadt_state_resumed	== cothread->state) || (0 == is_callee));
	assert((cothreadt_state_paused	== cothread->state) || (0 != is_callee));

	//---Switch the current state---//
	cothread->state	= cothreadt_state_resumed == cothread->state ? cothreadt_state_paused : cothreadt_state_resumed;
	cothreadt_signal(cothread);

	//---Wait for the running state---//
	while (running_state != cothread->state) {
		cothreadt_wait(cothread);
	}

	//---Unlock---//
	cothreadt_unlock(cothread);
}
