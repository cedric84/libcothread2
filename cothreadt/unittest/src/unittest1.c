/**
 * @brief		This file contains a unittest.
 * @file
 */

#include <unittest.h>

/**
 * @brief		The cothread entry point.
 * @param		[in]	cothread	The cothread.
 * @ingroup		doxy_cothreadt_unittest
 */
static void COTHREAD_CALL
user_cb(cothreadt_t* cothread)
{
	assert(0);	// never called.
}

/**
 * @brief		The unittest entry point.
 * @ingroup		doxy_cothreadt_unittest
 */
extern COTHREAD_LINK_HIDDEN void COTHREAD_CALL
unittest1(void)
{
	//---Initialize the attributes---//
	cothreadt_attr_t	attr;
	cothreadt_attr_init(&attr, user_cb);

	//---Initialize the cothread---//
	cothreadt_t	cothread;
	assert(cothread_err_ok	== cothreadt_init(&cothread, &attr));

	// never yielded.

	//---Uninitialize the cothread---//
	cothreadt_uninit(&cothread);
}
