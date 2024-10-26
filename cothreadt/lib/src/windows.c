/**
 * @brief		This file contains the definitions for the WINDOWS implementation.
 * @file
 */

#include <cothread/cothreadt.h>
#include <assert.h>
#include <stdio.h>

/**
 * @brief		Waits for the specified event to be signaled.
 * @param		[in]	cothread	The cothread.
 * @param		[in]	event		The event to wait for.
 * @relates		_cothreadt_t
 */
static void COTHREAD_CALL
cothreadt_wait(cothreadt_t* cothread, HANDLE event)
{
	//---Check arguments---//
	assert(NULL	!= cothread);
	assert(NULL	!= event);

	//---Wait---//
	const DWORD	wait_ret	= WaitForSingleObject(event, INFINITE);
	if (WAIT_OBJECT_0 != wait_ret) {
		COTHREADT_LOGF_FATAL(cothread, "cannot wait for event (%lu, %lu)", wait_ret, GetLastError());
	}
}

/**
 * @brief		Signals the specified event.
 * @param		[in]	cothread	The cothread.
 * @param		[in]	event		The event to signal.
 * @relates		_cothreadt_t
 */
static void COTHREAD_CALL
cothreadt_signal(cothreadt_t* cothread, HANDLE event)
{
	//---Check arguments---//
	assert(NULL	!= cothread);
	assert(NULL	!= event);

	//---Signal---//
	if (FALSE == SetEvent(event)) {
		COTHREADT_LOGF_FATAL(cothread, "cannot signal event (%lu)", GetLastError());
	}
}

/**
 * @brief		The thread entry point.
 * @param		[in]	lpParameter		The cothread.
 * @return		Returns zero.
 * @relates		_cothreadt_t
 */
static DWORD WINAPI
cothreadt_thd_cb(LPVOID lpParameter)
{
	//---Definitions---//
	cothreadt_t*	cothread	= (cothreadt_t*)lpParameter;

	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Wait for the thread to be resumed---//
	cothreadt_wait(cothread, cothread->callee);

	//---Run the user callback---//
	cothread->user_cb(cothread);

	//---Return to caller---//
	cothreadt_signal(cothread, cothread->caller);

	//---Return---//
	return 0;
}

extern COTHREAD_LINK void COTHREAD_CALL
cothreadt_uninit(cothreadt_t* cothread)
{
	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Wait for the thread to complete---//
	const DWORD	wait_ret	= WaitForSingleObject(cothread->thread_hdl, INFINITE);
	if (WAIT_OBJECT_0 != wait_ret) {
		COTHREADT_LOGF_FATAL(cothread, "cannot wait for thread (%lu, %lu)", wait_ret, GetLastError());
	}

	//---Release the thread---//
	if (FALSE == CloseHandle(cothread->thread_hdl)) {
		COTHREADT_LOGF_FATAL(cothread, "cannot release thread (%lu)", GetLastError());
	}

	//---Release the callee event---//
	if (FALSE == CloseHandle(cothread->callee)) {
		COTHREADT_LOGF_FATAL(cothread, "cannot release event (%lu)", GetLastError());
	}

	//---Release the caller event---//
	if (FALSE == CloseHandle(cothread->caller)) {
		COTHREADT_LOGF_FATAL(cothread, "cannot release event (%lu)", GetLastError());
	}
}

extern COTHREAD_LINK cothread_err_t COTHREAD_CALL
cothreadt_init(cothreadt_t* cothread, const cothreadt_attr_t* attr)
{
	//---Definitions---//
	cothread_err_t	err	= cothread_err_notsup;

	//---Check arguments---//
	assert(NULL	!= cothread);
	assert(NULL	!= attr);

	//---Zero---//
	cothread->user_cb	= attr->user_cb;

	//---Create the caller event---//
	if (NULL == (cothread->caller = CreateEvent(NULL, FALSE, FALSE, NULL))) {
		COTHREADT_LOGF(cothread, "cannot create event (%lu)", GetLastError());
	} else {
		//---Create the callee event---//
		if (NULL == (cothread->callee = CreateEvent(NULL, FALSE, FALSE, NULL))) {
			COTHREADT_LOGF(cothread, "cannot create event (%lu)", GetLastError());
		} else {
			//---Create the thread---//
			if (NULL == (cothread->thread_hdl = CreateThread(NULL, 0, cothreadt_thd_cb, cothread, 0, &(cothread->thread_id)))) {
				COTHREADT_LOGF(cothread, "cannot create thread (%lu)", GetLastError());
			} else {
				//---Update the error code---//
				err	= cothread_err_ok;
			}

			//---Error Management---//
			if (COTHREAD_ERR_ISNOK(err)) {
				if (FALSE == CloseHandle(cothread->callee)) {
					COTHREADT_LOGF_FATAL(cothread, "cannot release event (%lu)", GetLastError());
				}
			}
		}

		//---Error Management---//
		if (COTHREAD_ERR_ISNOK(err)) {
			if (FALSE == CloseHandle(cothread->caller)) {
				COTHREADT_LOGF_FATAL(cothread, "cannot release event (%lu)", GetLastError());
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

	//---Compute the events to signal & to wait for---//
	HANDLE	wait_event;
	HANDLE	signal_event;
	if (GetCurrentThreadId() == cothread->thread_id) {
		wait_event		= cothread->callee;
		signal_event	= cothread->caller;
	} else {
		wait_event		= cothread->caller;
		signal_event	= cothread->callee;
	}

	//---Signal & wait---//
	cothreadt_signal(cothread, signal_event);
	cothreadt_wait(cothread, wait_event);
}
