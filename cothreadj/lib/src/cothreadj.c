/**
 * @brief		This file contains the C functions definitions.
 * @file
 */

/**
 * @page		doxy_p_cothreadj		cothread - setjmp / longjmp
 * @tableofcontents
 *
 * @section		doxy_p_cothreadj_def	Definitions
 *				The [cothread](@ref _cothreadj_t) is an object that contains two execution contexts:
 *				the @e caller and the @e callee.
 *				Switching (or @e yielding) from one execution to another pauses the first one and resumes the later one.
 *
 * @section		doxy_p_cothreadj_use	Usage
 *				-# First of all, a @e stack should be allocated.
 *				This stack has to be aligned on a @ref COTHREADJ_STACK_ALIGN -byte boundary and its size has to
 *				be a multiple of it ;
 *				-# Once the stack is allocated, some [attributes](@ref _cothreadj_attr_t) have to be initialized
 *				with the @ref cothreadj_attr_init function ;
 *				-# For debugging purposes, the library may be more verbose by using
 *				the @ref cothreadj_attr_set_dbg_caller_name, the @ref cothreadj_attr_set_dbg_callee_name
 *				and the @ref cothreadj_attr_set_dbg_strm functions ;
 *				-# Once the attributes are initialized, the @ref cothreadj_init function should be called
 *				to initialize the [cothread](@ref _cothreadj_t) itself ;
 *				-# From this point, modifying the attributes has no effect on the cothread, and the
 *				@ref cothreadj_set_user_data and the @ref cothreadj_get_user_data functions may be called
 *				to store and retrieve any user data in the initialized cothread (the library never modifies it) ;
 *				-# Finally, calling the @ref cothreadj_yield function pauses the current execution context
 *				and resumes the other one.
 *				.
 *
 * @section		doxy_p_cothreadj_ex		Example
 * @include		cothreadj_example0.c
 */

#include <cothread/cothreadj.h>
#include <assert.h>
#include <stdint.h>

/**
 * @brief		Logs the specified message.
 * @param		[in]	_cothread	The cothread to log the message with.
 * @param		[in]	_fmt		The format of the message to log.
 * @param		[in]	...			The values to fill the format with.
 * @ingroup		doxy_cothreadj
 */
#define COTHREADJ_LOGF(_cothread, _fmt, ...)	{												\
	const cothreadj_t*	_cothd	= (_cothread);													\
	if (NULL != _cothd->dbg_strm) {																\
		fprintf(_cothd->dbg_strm, "%s: " _fmt "\n", _cothd->current->dbg_name, __VA_ARGS__);	\
	}																							\
}

/**
 * @brief		Calls the setjmp function.
 * @param		[in]	_buf	The buffer to store the execution context in.
 * @return		Returns zero on first return, any user value during the second one.
 * @ingroup		doxy_cothreadj
 */
#define COTHREADJ_SETJMP(_buf)	setjmp((_buf))

/**
 * @brief		Calls the longjmp function.
 * @param		[in]	_buf		The execution context to restore.
 * @param		[in]	_user_val	The user value to return.
 * @ingroup		doxy_cothreadj
 */
#define COTHREADJ_LONGJMP(_buf, _user_val)	longjmp((_buf), (_user_val))

extern COTHREAD_LINK void COTHREAD_CALL
cothreadj_attr_init(cothreadj_attr_t* attr, cothreadj_stack_t* stack, size_t stack_sz, cothreadj_cb_t user_cb)
{
	//---Check arguments---//
	assert(NULL	!= attr);
	assert(NULL	!= stack);
	assert(0	!= stack_sz);
	assert(NULL	!= user_cb);

	//---Check the stack alignment---//
	assert(0	== ((COTHREADJ_STACK_ALIGN - 1) & (uintptr_t)stack));
	assert(0	== ((COTHREADJ_STACK_ALIGN - 1) & stack_sz));

	//---Initialize---//
	attr->stack				= stack;
	attr->stack_sz			= stack_sz;
	attr->user_cb			= user_cb;
	//
	attr->dbg_caller_name	= NULL;
	attr->dbg_callee_name	= NULL;
	attr->dbg_strm			= NULL;
}

extern COTHREAD_LINK void COTHREAD_CALL
cothreadj_attr_set_dbg_caller_name(cothreadj_attr_t* attr, const char* name)
{
	assert(NULL	!= attr);
	attr->dbg_caller_name	= name;
}

extern COTHREAD_LINK void COTHREAD_CALL
cothreadj_attr_set_dbg_callee_name(cothreadj_attr_t* attr, const char* name)
{
	assert(NULL	!= attr);
	attr->dbg_callee_name	= name;
}

extern COTHREAD_LINK void COTHREAD_CALL
cothreadj_attr_set_dbg_strm(cothreadj_attr_t* attr, FILE* strm)
{
	assert(NULL	!= attr);
	attr->dbg_strm	= strm;
}

/**
 * @brief		Initializes and runs the specified cothread.
 * @param		[in]	cothread	The cothread to initialize.
 * @param		[in]	attr		The attributes to initialize the cothread with.
 * @relates		_cothreadj_t
 */
extern COTHREAD_LINK_HIDDEN void COTHREAD_CALL
cothreadj_core(cothreadj_t* cothread, const cothreadj_attr_t* attr)
{
	//---Definitions---//
	static const char	dbg_caller_name_default[]	= "caller";
	static const char	dbg_callee_name_default[]	= "callee";

	//---Check arguments---//
	assert(NULL	!= cothread);
	assert(NULL	!= attr);

	//---Init---//
	cothread->current			= &(cothread->callee);
	cothread->caller.dbg_name	= (NULL != attr->dbg_caller_name) ? attr->dbg_caller_name : dbg_caller_name_default;
	cothread->callee.dbg_name	= (NULL != attr->dbg_callee_name) ? attr->dbg_callee_name : dbg_callee_name_default;
	cothread->dbg_strm			= attr->dbg_strm;

	//---Initialize the callee endpoint---//
	cothreadj_cb_t	user_cb	= attr->user_cb;
	COTHREADJ_LOGF(cothread, "%s", "initializing endpoint");
	int	user_val	= COTHREADJ_SETJMP(cothread->current->buf);
	if (0 != user_val) {
		//---Forget the attributes which are not valid during the 2nd return---//
		attr	= NULL;

		//---Run the user callback---//
		COTHREADJ_LOGF(cothread, "%s", "starting user callback");
		user_val	= user_cb(cothread, user_val);
		COTHREADJ_LOGF(cothread, "%s", "user callback returned");

		//---Jump to the caller---//
		COTHREADJ_LOGF(cothread, "complete, returning to %s", cothread->caller.dbg_name);
		cothread->current	= &(cothread->caller);
		COTHREADJ_LONGJMP(cothread->current->buf, user_val);
	}

	//---Return to caller---//
	COTHREADJ_LOGF(cothread, "%s", "initialized");
	cothread->current	= &(cothread->caller);
	COTHREADJ_LOGF(cothread, "%s", "initialized");
}

extern COTHREAD_LINK void COTHREAD_CALL
cothreadj_set_user_data(cothreadj_t* cothread, void* user_data)
{
	assert(NULL	!= cothread);
	cothread->user_data	= user_data;
}

extern COTHREAD_LINK void* COTHREAD_CALL
cothreadj_get_user_data(const cothreadj_t* cothread)
{
	assert(NULL	!= cothread);
	return cothread->user_data;
}

extern COTHREAD_LINK int COTHREAD_CALL
cothreadj_yield(cothreadj_t* cothread, int user_val)
{
	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Save the current endpoint---//
	COTHREADJ_LOGF(cothread, "%s", "saving endpoint");
	const int	ret	= COTHREADJ_SETJMP(cothread->current->buf);

	//---Is it the first return from setjmp ?---//
	if (0 == ret) {
		//---Switch the endpoints---//
		COTHREADJ_LOGF(cothread, "%s", "yielding");
		cothread->current	= (&(cothread->caller) == cothread->current) ? &(cothread->callee) : &(cothread->caller);
		COTHREADJ_LOGF(cothread, "%s", "resuming");
		COTHREADJ_LONGJMP(cothread->current->buf, user_val);
	}

	//---Return---//
	COTHREADJ_LOGF(cothread, "%s", "endpoint restored");
	return ret;
}
