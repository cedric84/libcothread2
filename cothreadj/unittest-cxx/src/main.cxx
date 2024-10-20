/**
 * @brief		This file contains the application entry point.
 * @file
 */

#include <unittest.hxx>

/**
 * @brief		The application entry point.
 * @param		[in]	argc		The number of arguments.
 * @param		[in]	argv		The arguments values.
 * @return		Returns zero on success.
 * @ingroup		doxy_cothreadj_unittest_cxx
 */
extern "C" int
main(int argc, char* argv[])
{
	printf("%s started\n", __func__);

	unittest0();
	unittest1();
	unittest2();

	printf("%s terminated\n", __func__);
	return 0;
}
