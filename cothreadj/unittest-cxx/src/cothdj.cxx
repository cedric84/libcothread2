/**
 * @brief		This file contains the cothreadj class definitions.
 * @file
 */

#include <unittest.hxx>

/**
 * @brief		The destructor.
 */
_cothdj_t::~_cothdj_t(void)
{
}

/**
 * @brief		The main constructor.
 */
_cothdj_t::_cothdj_t(void)
{
	//---Initialize the cothread---//
	cothreadj_attr_t	attr;
	cothreadj_attr_init(&attr, this->stack, sizeof(this->stack), &::_cothdj_t::user_cb);
	cothreadj_init(&(this->cothreadj), &attr);
	cothreadj_set_user_data(&(this->cothreadj), this);
}


/**
 * @brief		The callee entry point.
 * @param		[in]	cothread	The cothread.
 * @param		[in]	user_val	Any user value (except zero) received from the other endpoint.
 * @return		Returns any user value (except zero) to send to the other endpoint.
 */
int COTHREAD_CALL
_cothdj_t::user_cb(cothreadj_t* cothread, int user_val)
{
	//---Check arguments---//
	assert(NULL	!= cothread);

	//---Enter the entry point---//
	_cothdj_t*	thiz	= (_cothdj_t*)cothreadj_get_user_data(cothread);
	return thiz->main(user_val);
}

/**
 * @brief		Switches from the current endpoint to the other one.
 * @param		[in]	user_val	Any user value (except zero) to send to the other endpoint.
 * @return		Returns the @e user_val received from the other endpoint.
 */
int
_cothdj_t::yield(int user_val)
{
	return cothreadj_yield(&(this->cothreadj), user_val);
}
