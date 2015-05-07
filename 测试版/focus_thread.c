#include "focus_in.h"

int zhs;
zhs =0;

int32s auto_focus_proc(void)
{
	
	int i ;
	if (zhs ==1){
		for (i=0;i<157;i++){
			lens_drive_zoom_motor();
			search_global();
			gthread_stop_req(AF_PRIO);
		}
	}
	else{
		zhs =1;
		search_global();
		gthread_stop_req(AF_PRIO);
	}
	
}

