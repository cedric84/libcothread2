/**
 * @brief		This file contains a unittest.
 * @file
 */

#include <unittest.h>
#include <string.h>

/// @cond
static cothreadt_t	cothread_g;
/// @endcond

/**
 * @brief		The cothread entry point.
 * @param		[in]	cothread	The cothread.
 * @ingroup		doxy_cothreadt_unittest
 */
static void COTHREAD_CALL
user_cb(cothreadt_t* cothread)
{
	//---Check arguments---//
	assert(NULL			!= cothread);
	assert(&cothread_g	== cothread);

	//---Switch from callee to caller---//
	size_t*	ctr	= (size_t*)cothreadt_get_user_data(cothread);
	assert(101	== ctr[0]++);
	cothreadt_yield(cothread);

	//---Return to caller---//
	assert(103	== ctr[0]++);
}

/**
 * @brief		The unittest entry point.
 * @ingroup		doxy_cothreadt_unittest
 */
extern COTHREAD_LINK_HIDDEN void COTHREAD_CALL
unittest0(void)
{
	//---Initialize the attributes---//
	cothreadt_attr_t	attr;
	cothreadt_attr_init(&attr, user_cb);

	//---Initialize the cothread---//
	size_t	ctr	= 100;
	assert(cothread_err_ok	== cothreadt_init(&cothread_g, &attr));
	cothreadt_set_user_data(&cothread_g, &ctr);

	//---Clear the attributes which are now useless---//
	memset(&attr, 0, sizeof(attr));

	//---Switch from caller to callee---//
	assert(100	== ctr++);
	cothreadt_yield(&cothread_g);
	assert(102	== ctr++);

	//---Switch from caller to callee---//
	cothreadt_yield(&cothread_g);
	assert(104	== ctr++);

	//---Uninitialize the cothread---//
	cothreadt_uninit(&cothread_g);
}
