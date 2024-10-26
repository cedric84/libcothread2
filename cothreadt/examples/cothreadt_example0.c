/**
 * @brief		This file contains an example of use.
 * @file
 */

#include <cothread/cothreadt.h>
#include <stdio.h>

static void COTHREAD_CALL
user_cb(cothreadt_t* cothread)
{
	//---Definitions---//
	int*	ctr	= (int*)cothreadt_get_user_data(cothread);

	//---Yield from callee to caller---//
	printf("callee updates user data to %d\n", ++(ctr[0]));
	cothreadt_yield(cothread);
	printf("callee updates user data to %d\n", ++(ctr[0]));
}

extern int
main(int argc, char* argv[])
{
	//---Log---//
	printf("%s started\n", __func__);

	//---Initialize the cothread---//
	cothreadt_attr_t	attr;
	cothreadt_t			cothread;
	cothreadt_attr_init(&attr, user_cb);
	if (COTHREAD_ERR_ISOK(cothreadt_init(&cothread, &attr))) {
		//---Store a user data in the cothread---//
		int	ctr	= 1234;
		cothreadt_set_user_data(&cothread, &ctr);

		//---Yield from caller to callee---//
		cothreadt_yield(&cothread);
		printf("caller updates user data to %d\n", ++ctr);
		cothreadt_yield(&cothread);
		printf("caller updates user data to %d\n", ++ctr);

		//---Uninitialize the cothread---//
		cothreadt_uninit(&cothread);
	}

	//---Return---//
	printf("%s terminated\n", __func__);
	return 0;
}
