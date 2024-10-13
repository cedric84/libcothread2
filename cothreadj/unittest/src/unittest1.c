/**
 * @brief		This file contains a unittest.
 * @file
 */

#include <unittest.h>

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
	assert(NULL	!= cothread);

	assert(100	== user_val);
	assert(100	== cothreadj_yield(cothread, 200));

	return 201;
}

/// @cond
int COTHREAD_CALL
fct0(cothreadj_t* cothread, size_t depth, int* initd)
{
	//---Definitions---//
	int	rc;

	//---Check arguments---//
	assert(NULL	!= cothread);
	assert(NULL	!= initd);

	//---Do we need to go deeper ?---//
	if (0 != depth) {
		//---Go deeper---//
		rc	= fct0(cothread, depth - 1, initd);
	} else {
		//---Is the cothread not initialized yet ?---//
		if (0 == initd[0]) {
			//---Initialize the cothread---//
			static cothreadj_stack_t	stack[sizeof(void*) * 1024 * 1024 / sizeof(cothreadj_stack_t)];
			cothreadj_attr_t			attr;
			cothreadj_attr_init(&attr, stack, sizeof(stack), user_cb);
			cothreadj_init(cothread, &attr);
			initd[0]	= !0;
		}

		//---Yield---//
		rc	= cothreadj_yield(cothread, 100);
	}

	//---Return---//
	return rc;
}
/// @endcond

/**
 * @brief		The unittest entry point.
 * @ingroup		doxy_cothreadj_unittest
 */
extern COTHREAD_LINK_HIDDEN void COTHREAD_CALL
unittest1(void)
{
	cothreadj_t	cothread;
	int			initd	= 0;
	assert(200	== fct0(&cothread, 2, &initd));
	assert(201	== fct0(&cothread, 1, &initd));
}
