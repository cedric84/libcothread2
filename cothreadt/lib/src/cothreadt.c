/**
 * @brief		This file contains the definitions for any implementation.
 * @file
 */

/**
 * @page		doxy_p_cothreadt		cothread - thread
 * @tableofcontents
 *
 * @section		doxy_p_cothreadt_def	Definitions
 *				The [cothread](@ref _cothreadt_t) is an object that contains two execution contexts:
 *				the @e caller and the @e callee.
 *				Switching (or @e yielding) from one execution to another pauses the first one and resumes the later one.
 *
 * @section		doxy_p_cothreadt_use	Usage
 *				-# First of all, some [attributes](@ref _cothreadt_attr_t) have to be initialized using the
 *				@ref cothreadt_attr_init function ;
 *				-# Once the attributes are initialized, the @ref cothreadt_init function should be called
 *				to initialize the [cothread](@ref _cothreadt_t) itself (note that this function may fail
 *				so its return value @b MUST be checked using the @ref COTHREAD_ERR_ISOK macro for example) ;
 *				-# If the initialization succeeded, a user data may be stored in the cothread using the
 *				@ref cothreadt_set_user_data function, and retrieved using the @ref cothreadt_get_user_data one
 *				(the library never modifies it.) Note also that the attributes may be discarded since the cothread
 *				never accesses them again ;
 *				-# From this point, the user is free to call the @ref cothreadt_yield function to switch from
 *				an execution context to the other one ;
 *				-# Finally, once the @e callee completes, the user has to call the @ref cothreadt_uninit function
 *				to release resources. Note that this function <b>MUST NOT</b> be called if the @ref cothreadt_init
 *				one has failed earlier.
 *				.
 *
 * @section		doxy_p_cothreadt_ex		Example
 * @include		cothreadt_example0.c
 */

#include <cothread/cothreadt.h>
#include <assert.h>

extern COTHREAD_LINK void COTHREAD_CALL
cothreadt_attr_init(cothreadt_attr_t* attr, cothreadt_cb_t user_cb)
{
	//---Check arguments---//
	assert(NULL	!= attr);
	assert(NULL	!= user_cb);

	//---Init---//
	attr->user_cb	= user_cb;
}
