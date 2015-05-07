#include <avserver.h>
#include <aewb_xdm.h>
#include <alg_aewb.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <drv_sync.h>
#include <drv_lens.h>
#include <video_2a.h>
#include "sys_log.h"

#include <stdlib.h>

static int ptz_msg_id=0;

int VIDEO_cmdTskCreate()
{

	ptz_msg_id=ptz_msg_init();
	fprintf(stderr, "$$$$$$$$$$$$$ptz msg id=%d$$$$$$$$$$$$$$\n", ptz_msg_id);


	
	fprintf(stderr, "*************************************************\n");
	fprintf(stderr, "**************Cmd Tsk Create**********************\n");
	fprintf(stderr, "*************************************************\n");
	fprintf(stderr, "*************************************************\n");

	return OSA_SOK;
}


int VIDEO_cmdTskDelete()
{
  	int status;

	fprintf(stderr, "************************************************\n");
	fprintf(stderr, "**************Cmd Tsk Delete*********************\n");
	fprintf(stderr, "************************************************\n");
	fprintf(stderr, "************************************************\n");
	
 	status=OSA_SOK;
	
  	return status;
}



int VIDEO_cmdTskRun()
{
	int 	msg_type_src=2;
	af_info_type 	af_info;
	int 	i;
	int 	msg_cmd;

	int	para1;
	int	para2;

	struct timeval tv1, tv2; 
	int 	m_secs;
	char    buf[20]=
	{	
		0x80, 0x01, 0x04, 0x07, 0x27, 0xff, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};	

	
	//fprintf(stderr, "************************************************\n");
	//fprintf(stderr, "**************Cmd Tsk Run***********************\n");
	//fprintf(stderr, "************************************************\n");

	
	if((msg_cmd=ptz_msg_rcv_s_nowait(ptz_msg_id, &para1, &para2))>0)
	{
		FILE *fp;

		LOG_TRACE("msg cmd=%d\n", msg_cmd);
		LOG_TRACE("======zhsTest===zoom_pos=%d=foucs_pos=%d-----\r\n",lens_get_zoom_cur_pos(), lens_get_focus_cur_pos());
		fp = fopen("/mnt/ramdisk/test.txt","a+");
		if (fp==NULL)
			printf("file open failed~");
		else{
			fprintf(fp,"zoom_pos=%d\tfocus_pos=%d\n",lens_get_zoom_cur_pos(),lens_get_focus_cur_pos());
			fclose(fp);
		}
		
		switch(msg_cmd)
		{

			case 	ZOOM_WIDE_DRV		:
				
				buf[3]=0x07;
				buf[4]=0x37;
				cam_zoom_cmdproc(buf, 0);
				
				break;
				
			case	ZOOM_TELE_DRV		:
				
				buf[3]=0x07;
				buf[4]=0x27;
				cam_zoom_cmdproc(buf, 0);
			
				break;
				
			case	ZOOM_MOTOR_STOP	:
				
				buf[3]=0x07;
				buf[4]=0x00;
				cam_zoom_cmdproc(buf, 0);
				break;

				
			case 	FOCUS_NEAR_DRV		:

				buf[3]=0x08;
				buf[4]=0x03;
				cam_focus_cmdproc(buf, 0);
				
				break;
				
			case 	FOCUS_FAR_DRV		:

				buf[3]=0x08;
				buf[4]=0x02;
				cam_focus_cmdproc(buf, 0);

				break;
				
			case	FOCUS_MOTOR_STOP	:

				buf[3]=0x08;
				buf[4]=0x00;
				cam_focus_cmdproc(buf, 0);
				break;

	
			case	MOTOR_INIT:
				cam_motor_init_cmdproc();
				//fprintf(stderr, "MOTOR INIT END\n");
				break;

			default:

				break;


		}
		
		//fprintf(stderr, "-----Snd Back-----\n");
		ptz_msg_snd_s_nowait(ptz_msg_id, para1, para2);
	}
	
	OSTimeDly(40);
	
	return OSA_SOK;
}



int VIDEO_cmdTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);
  int DN_status = -1;
  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Create...\n");
  #endif

  status = VIDEO_cmdTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_aewbTskCreate()\n");
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Create...DONE\n");
  #endif

  status = OSA_tskWaitMsg(pTsk, &pMsg);
  if(status!=OSA_SOK)
    return OSA_SOK;

  cmd = OSA_msgGetCmd(pMsg);

  if(cmd==AVSERVER_CMD_DELETE) {

    done = TRUE;
    ackMsg = TRUE;

  } else {

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
    OSA_printf(" Cmd: Start...\n");
    #endif

    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
  }

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" Cmd: Start...DONE\n");
  #endif


  int check_time=0;

  while(!done) {

    status = VIDEO_cmdTskRun();

    if(status!=OSA_SOK) {
      OSA_ERROR("VIDEO_aewbTskRun()\n");
      break;
    }


    status = OSA_tskCheckMsg(pTsk, &pMsg);

    if(status!=OSA_SOK)
      continue;

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
    OSA_printf(" 2A: Received CMD = 0x%04x\n", cmd);
    #endif

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AVSERVER_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      default:

        #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
        OSA_printf(" 2A: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Delete...\n");
  #endif

  int it=1;
  VIDEO_cmdTskDelete();
  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Delete...DONE\n");
  #endif


  return OSA_SOK;
}

int VIDEO_cmdCreate()
{
	int status;

	status = OSA_tskCreate( &gVIDEO_ctrl.cmdTsk, VIDEO_cmdTskMain, VIDEO_CMD_THR_PRI, VIDEO_CMD_STACK_SIZE, 0);
	if(status!=OSA_SOK) 
	{
		OSA_ERROR("OSA_tskCreate()\n");
		return status;
	}

	return status;
}

int VIDEO_cmdDelete()
{
	int status;

	status = OSA_tskDelete( &gVIDEO_ctrl.cmdTsk );

	return status;
}

