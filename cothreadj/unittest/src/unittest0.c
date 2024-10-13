/**
 * @brief		This file contains a unittest.
 * @file
 */

#include <unittest.h>
#include <string.h>

/// @cond
static cothreadj_t	cothread_g;
/// @endcond

/**
 * @brief		The callee entry point.
 * @param		[in]	cothread	The cothread.
 * @param		[in]	user_val	Any user value (except zero) received from the other endpoint.
 * @return		Returns any user value (except zero) to send to the other endpoint.
 * @ingroup		doxy_cothreadj_unittest
 */
static int COTHREAD_CALL
user_cb(cothreadj_t* cothread, int user_val)
{
	//---Check arguments---//
	assert(NULL			!= cothread);
	assert(&cothread_g	== cothread);

	//---Check the counter---//
	size_t*	ctr	= (size_t*)cothreadj_get_user_data(cothread);
	assert(1234	== ctr[0]++);

	//---Switch from callee to caller---//
	assert(&(cothread->callee)	== cothread->current);
	assert(100					== user_val);
	assert(101					== cothreadj_yield(cothread, 200));
	assert(&(cothread->callee)	== cothread->current);

	//---Check the counter---//
	assert(1236	== ctr[0]++);

	//---Return to caller---//
	return 201;
}

/**
 * @brief		The unittest entry point.
 * @ingroup		doxy_cothreadj_unittest
 */
extern COTHREAD_LINK_HIDDEN void COTHREAD_CALL
unittest0(void)
{
	//---Initialize the attributes---//
	static cothreadj_stack_t	stack[sizeof(void*) * 1024 * 1024 / sizeof(cothreadj_stack_t)];
	cothreadj_attr_t			attr;
	cothreadj_attr_init(&attr, stack, sizeof(stack), user_cb);

	//---Initialize the cothread---//
	size_t	ctr	= 1234;
	cothreadj_init(&cothread_g, &attr);
	cothreadj_set_user_data(&cothread_g, &ctr);

	//---Clear the attributes which are now useless---//
	memset(&attr, 0, sizeof(attr));

	//---Switch from caller to callee---//
	assert(&(cothread_g.caller)	== cothread_g.current);
	assert(200					== cothreadj_yield(&cothread_g, 100));
	assert(&(cothread_g.caller)	== cothread_g.current);

	//---Check the counter---//
	assert(1235	== ctr++);

	//---Switch from caller to callee---//
	assert(&(cothread_g.caller)	== cothread_g.current);
	assert(201					== cothreadj_yield(&cothread_g, 101));
	assert(&(cothread_g.caller)	== cothread_g.current);

	//---Check the counter---//
	assert(1237	== ctr);
}
