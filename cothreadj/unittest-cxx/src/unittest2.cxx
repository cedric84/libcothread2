/**
 * @brief		This file contains a unittest.
 * @file
 */

#include <unittest.hxx>
#include <exception>

/**
 * @brief		The unittest entry point.
 * @ingroup		doxy_cothreadj_unittest_cxx
 */
extern COTHREAD_LINK_HIDDEN void COTHREAD_CALL
unittest2(void)
{
	struct _cothd_t : public _cothdj_t
	{
		public:
			size_t	ctr;
		public:
			_cothd_t(void)
			{ this->ctr	= 0; }

		private:
			void
			fct0(void)
			{
				//---Catch an exception---//
				try { throw ::std::exception{}; }
				catch (::std::exception&) { assert(5	== this->ctr++); }
			}

		private:
			void
			fct1(void)
			{
				//---Define an object to check the stack unwinding---//
				struct _dummy_t {
					private:	size_t*	ctr;
					public:		virtual ~_dummy_t	(void)			{ assert(6	== this->ctr[0]++); }
					public:				 _dummy_t	(size_t* ctr)	{ this->ctr	= ctr; }
				} dummy{&(this->ctr)};

				//---Throw an exception---//
				throw ::std::exception{};
			}

		protected:
			virtual int
			main(int user_val) override
			{
				//---Check arguments---//
				assert(100	== user_val);

				//---Catch an exception---//
				try { throw ::std::exception{}; }
				catch (::std::exception&) { assert(2	== this->ctr++); }

				//---Yield to caller---//
				assert(101	== this->yield(200));

				//---Catch an exception---//
				try { throw ::std::exception{}; }
				catch (::std::exception&) { assert(4	== this->ctr++); }

				//---Call a method that catches an exception---//
				this->fct0();

				//---Catch an exception thrown by a method---//
				try { this->fct1(); }
				catch (::std::exception&) { assert(7	== this->ctr++); }

				//---Return---//
				return 201;
			}
	};

	//---Create a cothread---//
	_cothd_t*	cothd;
	assert(NULL	!= (cothd = new _cothd_t{}));

	//---Catch an exception---//
	try { throw ::std::exception{}; }
	catch (::std::exception&) { assert(0	== cothd->ctr++); }

	//---Catch another exception---//
	try { throw ::std::exception{}; }
	catch (...) { assert(1	== cothd->ctr++); }

	//---Yield to callee---//
	assert(200	== cothd->yield(100));

	//---Catch an exception---//
	try { throw ::std::exception{}; }
	catch (::std::exception&) { assert(3	== cothd->ctr++); }

	//---Yield to callee---//
	assert(201	== cothd->yield(101));

	//---Release memory---//
	assert(8	== cothd->ctr);
	delete cothd;
}
