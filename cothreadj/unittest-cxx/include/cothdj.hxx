/**
 * @brief		This file contains the cothreadj class declarations.
 * @file
 */

#ifndef __UNITTEST_COTHDJ_HXX__
#define __UNITTEST_COTHDJ_HXX__

/**
 * @brief		The cothreadj class.
 * @ingroup		doxy_cothreadj_unittest_cxx
 */
struct _cothdj_t
{
	protected:
		virtual	~_cothdj_t	(void);
	protected:
				 _cothdj_t	(void);

	private:
		cothreadj_stack_t	stack[sizeof(void*) * 1024 * 1024 / sizeof(cothreadj_stack_t)];	///< @brief	The stack.
		cothreadj_t			cothreadj;	///< @brief	The cothread.
	private:
		static int COTHREAD_CALL user_cb	(cothreadj_t* cothread, int user_val);
	protected:
		/**
		 * @brief		The entry point.
		 * @param		[in]	user_val	Any user value (except zero) to send to the other endpoint.
		 * @return		Returns the @e user_val received from the other endpoint.
		 */
		virtual int		main	(int user_val) = 0;
	public:
		int				yield	(int user_val);
};

#endif /* __UNITTEST_COTHDJ_HXX__ */
