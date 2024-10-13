/**
 * @brief		This file contains an example of use.
 * @file
 */

#include <cothread/cothreadj.h>

static int COTHREAD_CALL
user_cb(cothreadj_t* cothread, int user_val)
{
	//---Log---//
	printf("callee received %d from caller\n", user_val);

	//---Yield from callee to caller---//
	printf("callee received %d from caller\n", cothreadj_yield(cothread, 200));

	//---Return---//
	return 201;
}

extern int
main(int argc, char* argv[])
{
	//---Log---//
	printf("%s started\n", __func__);

	//---Allocate the stack---//
	static cothreadj_stack_t	stack[sizeof(void*) * 1024 * 1024 / sizeof(cothreadj_stack_t)];

	//---Allocate the cothread---//
	cothreadj_attr_t	attr;
	cothreadj_t			cothread;
	cothreadj_attr_init(&attr, stack, sizeof(stack), user_cb);
	cothreadj_init(&cothread, &attr);

	//---Yield from caller to callee---//
	printf("caller received %d from callee\n", cothreadj_yield(&cothread, 100));
	printf("caller received %d from callee\n", cothreadj_yield(&cothread, 101));

	//---Return---//
	printf("%s terminated\n", __func__);
	return 0;
}
