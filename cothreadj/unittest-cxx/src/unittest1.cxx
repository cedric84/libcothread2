/**
 * @brief		This file contains a unittest.
 * @file
 */

#include <unittest.hxx>

/**
 * @brief		The unittest entry point.
 * @ingroup		doxy_cothreadj_unittest_cxx
 */
extern COTHREAD_LINK_HIDDEN void COTHREAD_CALL
unittest1(void)
{
	struct _cothd0_t : public _cothdj_t
	{
		protected:
			virtual int
			main(int user_val) override
			{
				assert(100	== user_val);
				assert(101	== this->yield(200));
				return 201;
			}
	};

	struct _cothd1_t : public _cothdj_t
	{
		private:
			_cothd0_t	nested;

		protected:
			virtual int
			main(int user_val) override
			{
				return this->nested.yield(user_val);
			}
	};

	_cothd1_t*	cothd;
	assert(NULL	!= (cothd = new _cothd1_t{}));
	assert(200	== cothd->yield(100));
	assert(201	== cothd->yield(101));
	delete cothd;
}
