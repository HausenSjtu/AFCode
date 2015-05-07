#include "focus_in.h"

int32s auto_focus_proc(void)
{
	search_global();
	gthread_stop_req(AF_PRIO);
}