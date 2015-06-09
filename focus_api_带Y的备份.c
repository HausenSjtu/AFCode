
#define	FOCUS_IN_GLOBAL
#include "focus_in.h"

struct 	filter_statistic_unit 		filter_statistic_q[4][TRACE_LEN];
struct 	filter_statistic_peak 		filter_statistic_p[4];
struct 	filter_statistic_valley 	filter_statistic_v[4];

int32s 	focus_win_detect_buffer[4][FRAMES_DETECT_NUM];
int32s 	focus_ae_detect_buffer[3][FRAMES_DETECT_NUM/2];

int32s 	focus_step_buffer[3];


void filter_statistic_store(void)
{
	int32s i;
	int32s j;

	//记录四个窗口的统计值
	for(j=0; j<4; j++)
	{
	
		for(i = 0; i <TRACE_LEN - 1; i++)
		{
			filter_statistic_q[j][i].fpos			=filter_statistic_q[j][i+1].fpos;
			filter_statistic_q[j][i].fstep			=filter_statistic_q[j][i+1].fstep;
			filter_statistic_q[j][i].lpf			=filter_statistic_q[j][i+1].lpf;
			filter_statistic_q[j][i].hpf			=filter_statistic_q[j][i+1].hpf;

		}
		
		filter_statistic_q[j][TRACE_LEN - 1].fpos = lens_get_focus_cur_pos();
		filter_statistic_q[j][TRACE_LEN - 1].fstep = lens_get_focus_step();
		filter_statistic_q[j][TRACE_LEN - 1].lpf 	= af_info.win_lpf[j];
		filter_statistic_q[j][TRACE_LEN - 1].hpf 	= af_info.win_hpf[j]; 

		//曝光值有效
		if(af_info.ae_shut > 0)
		{
			for(i = 0; i <TRACE_LEN - 1; i++)
			{
				filter_statistic_q[j][i].ae_shut		=filter_statistic_q[j][i+1].ae_shut;
				filter_statistic_q[j][i].ae_sgain		=filter_statistic_q[j][i+1].ae_sgain;
				filter_statistic_q[j][i].ae_igain		=filter_statistic_q[j][i+1].ae_igain;
			}
		
			filter_statistic_q[j][TRACE_LEN - 1].ae_shut	= af_info.ae_shut;
			filter_statistic_q[j][TRACE_LEN - 1].ae_sgain	= af_info.ae_sgain;
			filter_statistic_q[j][TRACE_LEN - 1].ae_igain	= af_info.ae_igain;
		}

		
		//filter_statistic_q[j][TRACE_LEN - 1].cnt 	= af_info.win_cnt; 


		//记录窗口的峰值位置
		if(af_info.win_lpf[j]>filter_statistic_p[j].lpf_peak)
		{
			filter_statistic_p[j].lpf_peak_pos	=filter_statistic_q[j][TRACE_LEN - 1].fpos;
			filter_statistic_p[j].lpf_peak		=af_info.win_lpf[j];

			//更新所有的步长
			for(i=0; i<TRACE_LEN -1; i++)
			{
				filter_statistic_p[j].lpf_peak_step_list[i]=filter_statistic_q[j][i].fstep;
			}

			//更新最新的step 步长
			filter_statistic_p[j].lpf_peak_step_list[TRACE_LEN -1]=filter_statistic_q[j][TRACE_LEN - 1].fstep;
		}	

		if(af_info.win_hpf[j]>filter_statistic_p[j].hpf_peak)
		{
			filter_statistic_p[j].hpf_peak_pos	=filter_statistic_q[j][TRACE_LEN - 1].fpos;
			filter_statistic_p[j].hpf_peak		=af_info.win_hpf[j];	

			//更新所有的步长
			for(i=0; i<TRACE_LEN -1; i++)
			{
				filter_statistic_p[j].hpf_peak_step_list[i]=filter_statistic_q[j][i].fstep;
			}

			//更新最新的step 步长
			filter_statistic_p[j].hpf_peak_step_list[TRACE_LEN -1]=filter_statistic_q[j][TRACE_LEN - 1].fstep;
		}
				
		//记录窗口的谷底位置
		if(af_info.win_lpf[j]<filter_statistic_v[j].lpf_valley)
		{
			filter_statistic_v[j].lpf_valley_pos	=filter_statistic_q[j][TRACE_LEN - 1].fpos;
			filter_statistic_v[j].lpf_valley		=af_info.win_lpf[j];
		}

		//记录窗口的谷底位置
		if(af_info.win_hpf[j]<filter_statistic_v[j].hpf_valley)
		{
			filter_statistic_v[j].hpf_valley_pos	=filter_statistic_q[j][TRACE_LEN - 1].fpos;
			filter_statistic_v[j].hpf_valley		=af_info.win_hpf[j];
		}
	}
	
}



void filter_statistic_init(void)
{
	int32s 	i;
	int32s	j;
	int32s	focus_pos;

	focus_pos=lens_get_focus_cur_pos();
	
	for(j=0; j<4; j++)
	{
		filter_statistic_p[j].lpf_peak_pos		=focus_pos;
		filter_statistic_p[j].hpf_peak_pos		=focus_pos;
		
		

		filter_statistic_p[j].lpf_peak 			=STATISTICS_MIN;
		filter_statistic_p[j].hpf_peak 			=STATISTICS_MIN;

		
		filter_statistic_v[j].hpf_valley_pos		=focus_pos;
		filter_statistic_v[j].lpf_valley_pos		=focus_pos;
		
		filter_statistic_v[j].lpf_valley			=CURVE_DATA_INIT;
		filter_statistic_v[j].hpf_valley			=CURVE_DATA_INIT;
		
		for(i = 0; i<TRACE_LEN; i++)
		{
			filter_statistic_q[j][i].fpos 			= focus_pos;
			filter_statistic_q[j][i].lpf			= CURVE_DATA_INIT;
			filter_statistic_q[j][i].hpf			= CURVE_DATA_INIT;	
			//filter_statistic_q[j][i].cnt			= CURVE_DATA_INIT;
			
			filter_statistic_q[j][i].ae_shut		= CURVE_DATA_INIT;	
			filter_statistic_q[j][i].ae_sgain		= CURVE_DATA_INIT;	
			filter_statistic_q[j][i].ae_igain		= CURVE_DATA_INIT;	

			filter_statistic_q[j][i].fstep				=lens_get_focus_step();
			filter_statistic_p[j].lpf_peak_step_list[i]	=lens_get_focus_step();
		}
	}

}


sta_type focus_detect_buffer_update(int32u win0_value, int32u win1_value, int32u win2_value, int32u ae_shut, int32u ae_sgain, int32u ae_igain)
{
	int32u 		i;

	int32u 		sum_win[3];
	int32u		sum_ae[3];

	int32u 		min_win[3];
	int32u 		max_win[3];

	int32u		min_ae[3];
	int32u		max_ae[3];

	sta_type		sta_t;

	
	for(i = 0; i<FRAMES_DETECT_NUM-1; i++)
	{
		//更新3 个窗口的统计值
		focus_win_detect_buffer[0][i] 	= focus_win_detect_buffer[0][i+1];
		focus_win_detect_buffer[1][i] 	= focus_win_detect_buffer[1][i+1];
		focus_win_detect_buffer[2][i] 	= focus_win_detect_buffer[2][i+1];
	
	}
	focus_win_detect_buffer[0][FRAMES_DETECT_NUM-1] = win0_value;
	focus_win_detect_buffer[1][FRAMES_DETECT_NUM-1] = win1_value;
	focus_win_detect_buffer[2][FRAMES_DETECT_NUM-1] = win2_value;


	//SHUT 值大于0 代表有效曝光值
	//AE 3 场曝光一次
	if(ae_shut > 0)
	{
		
		for(i = 0; i<FRAMES_DETECT_NUM_AE-1; i++)
		{
			//更新亮度统计值
			focus_ae_detect_buffer[0][i]	= focus_ae_detect_buffer[0][i+1];
			focus_ae_detect_buffer[1][i]	= focus_ae_detect_buffer[1][i+1];
			focus_ae_detect_buffer[2][i]	= focus_ae_detect_buffer[2][i+1];
		}
		
		focus_ae_detect_buffer[0][FRAMES_DETECT_NUM_AE-1]	= ae_shut;
		focus_ae_detect_buffer[1][FRAMES_DETECT_NUM_AE-1]	= ae_sgain;
		focus_ae_detect_buffer[2][FRAMES_DETECT_NUM_AE-1]	= ae_igain;
	}

	
	
	sum_win[0] 	= 0;
	sum_win[1]	= 0;
	sum_win[2]	= 0;
	
	sum_ae[0]	= 0;
	sum_ae[1]	= 0;
	sum_ae[2]	= 0;
	
	for(i = 0; i<FRAMES_DETECT_NUM; i++)
	{
		sum_win[0] 	+= focus_win_detect_buffer[0][i] ;
		sum_win[1] 	+= focus_win_detect_buffer[1][i] ;
		sum_win[2] 	+= focus_win_detect_buffer[2][i] ;
	}

	for(i=0; i<FRAMES_DETECT_NUM_AE; i++)
	{
		sum_ae[0]	+= focus_ae_detect_buffer[0][i];
		sum_ae[1]	+= focus_ae_detect_buffer[1][i];
		sum_ae[2]	+= focus_ae_detect_buffer[2][i];
	}
	
	for(i=0; i<3; i++)
	{
		min_win[i] 	= 4294967290u;
		max_win[i] 	= 0;

		min_ae[i]		= 4294967290u;
		max_ae[i]	=0;
	}
	
	
	for(i = 0; i<FRAMES_DETECT_NUM; i++)
	{
		//AF 极限值搜索
		min_win[0] 	=focus_win_detect_buffer[0][i]<min_win[0]?  	focus_win_detect_buffer[0][i] : min_win[0];
		max_win[0] 	=focus_win_detect_buffer[0][i]>max_win[0]? 	focus_win_detect_buffer[0][i] : max_win[0];

		min_win[1] 	=focus_win_detect_buffer[1][i]<min_win[1]?  	focus_win_detect_buffer[1][i] : min_win[1];
		max_win[1] 	=focus_win_detect_buffer[1][i]>max_win[1]? 	focus_win_detect_buffer[1][i] : max_win[1];

		min_win[2] 	=focus_win_detect_buffer[2][i]<min_win[2]?  	focus_win_detect_buffer[2][i] : min_win[2];
		max_win[2] 	=focus_win_detect_buffer[2][i]>max_win[2]? 	focus_win_detect_buffer[2][i] : max_win[2];
		
	}
	
	for(i = 0; i<FRAMES_DETECT_NUM_AE; i++)
	{
	
		//AE 极限值搜索
		min_ae[0] 	=focus_ae_detect_buffer[0][i]<min_ae[0]?  	focus_ae_detect_buffer[0][i] : min_ae[0];
		max_ae[0] 	=focus_ae_detect_buffer[0][i]>max_ae[0]? 	focus_ae_detect_buffer[0][i] : max_ae[0];

		min_ae[1] 	=focus_ae_detect_buffer[1][i]<min_ae[1]?  	focus_ae_detect_buffer[1][i] : min_ae[1];
		max_ae[1] 	=focus_ae_detect_buffer[1][i]>max_ae[1]? 	focus_ae_detect_buffer[1][i] : max_ae[1];

		min_ae[2] 	=focus_ae_detect_buffer[2][i]<min_ae[2]?  	focus_ae_detect_buffer[2][i] : min_ae[2];
		max_ae[2] 	=focus_ae_detect_buffer[2][i]>max_ae[2]? 	focus_ae_detect_buffer[2][i] : max_ae[2];
		
	}

	for(i=0; i<3; i++)
	{
		sta_t.win_value[i]	=(sum_win[i] - min_win[i] - max_win[i])/(FRAMES_DETECT_NUM-2);
	}


	sta_t.ae_shut 	=(sum_ae[0] - min_ae[0] - max_ae[0])/(FRAMES_DETECT_NUM_AE -2);
	sta_t.ae_sgain	=(sum_ae[1] - min_ae[1] - max_ae[1])/(FRAMES_DETECT_NUM_AE -2);
	sta_t.ae_igain	=(sum_ae[2] - min_ae[2] - max_ae[2])/(FRAMES_DETECT_NUM_AE -2);
	
	
	return sta_t;
	
}

sta_type 	focus_detect_buffer_init(int32u win0_value, int32u win1_value, int32u win2_value,  int32u ae_shut, int32u ae_sgain, int32u ae_igain)
{
	int32u 	i;
	sta_type	sta_t;

	for(i = 0; i<FRAMES_DETECT_NUM; i++)
	{
		focus_win_detect_buffer[0][i] 	= 	win0_value;
		focus_win_detect_buffer[1][i] 	= 	win1_value;
		focus_win_detect_buffer[2][i] 	= 	win2_value;
		
	}

	for(i = 0; i<FRAMES_DETECT_NUM_AE; i++)
	{
		focus_ae_detect_buffer[0][i] 	= 	ae_shut;
		focus_ae_detect_buffer[1][i] 	= 	ae_sgain;
		focus_ae_detect_buffer[2][i] 	= 	ae_igain;
		
	}
	
	sta_t.win_value[0]	=win0_value;
	sta_t.win_value[1]	=win1_value;
	sta_t.win_value[2]	=win2_value;
	
	sta_t.ae_shut		=ae_shut;
	sta_t.ae_sgain		=ae_sgain;
	sta_t.ae_igain		=ae_igain;

	return sta_t;
	
}

void	focus_step_buffer_init(int32s focus_step)
{
	int32s 	tbl_len=sizeof(focus_step_buffer)/sizeof(focus_step_buffer[0]);
	int32s 	i;

	for(i=0; i<tbl_len; i++)
	{
		focus_step_buffer[i]=focus_step;
	}
	
}

int32s focus_step_buffer_update(int32s focus_step)
{
	int32s 	tbl_len=sizeof(focus_step_buffer)/sizeof(focus_step_buffer[0]);
	int32s 	i;

	for(i=0; i<tbl_len-1; i++)
	{
		focus_step_buffer[i]=focus_step_buffer[i+1];
	}

	focus_step_buffer[tbl_len-1]=focus_step;

	return	focus_step_buffer[0];
}


#if 1
int32s focus_pos_arrive(int32s focus_step, int32s focus_tpos)
 {

	int8u	err;
	
 	int32s 	i;
	int32s 	focus_cpos;
	int32s 	step_cnt;
	int32s 	step_remain;

	int32s 	bak_dir;
	int32s 	bak_step;
	int32s 	dist;
	int32s 	accelerate_step;
	int32s	motion_th;

	//motion_th=get_motion_detect_thresh(focus_step);
	motion_th=0;

	bak_dir 	=lens_get_focus_dir();
	bak_step =lens_get_focus_step();

	focus_step= focus_step> 36? 36 : focus_step;
	focus_step= focus_step < 0? 0 : focus_step;

	
	focus_cpos= lens_get_focus_cur_pos();
	
	if(focus_cpos>focus_tpos)
	{
		//lens_set_focus_dir(LENS_FOCUS_FAR);
		lens_focus_compen(LENS_FOCUS_FAR);
	}
	else if(focus_cpos<focus_tpos)
	{	    
		//lens_set_focus_dir(LENS_FOCUS_NEAR);
		lens_focus_compen(LENS_FOCUS_NEAR);
	}
	else
	{
		return	SEARCH_CONTINUE;
	}
	
	//printf("focus_cur_pos=%d, focus_tar_pos=%d\r\n", focus_cpos, focus_tpos);
	
	//accelerate_step = 4;
	dist=abs(focus_cpos-focus_tpos);

	#if 0
	if(dist>accelerate_step && focus_step>accelerate_step)
	{
		lens_set_focus_step(accelerate_step);

		if(drive_filter_update(MOTOR_RUN)==SEARCH_RESTART)
		{
			return	SEARCH_RESTART;
		}

		//移动侦测
		if(motion_detect_all(motion_th, 300)==MOTION_DETECT)
		{
			return 	SEARCH_RESTART;
		}
		
		
		dist=dist-accelerate_step;
	}
	#endif
	
	step_cnt=dist/focus_step;
	step_remain=dist%focus_step;
	
	lens_set_focus_step(focus_step);

	//printf("step_cnt=%d, step_remain=%d, focus_step=%d\r\n", step_cnt, step_remain, focus_step);
	
	for(i = 0; i<step_cnt; i++)
	{
		if(drive_filter_update(MOTOR_RUN)==SEARCH_RESTART)
		{
			return	SEARCH_RESTART;
		}
	
		if(motion_detect_all(motion_th, 300)==MOTION_DETECT)
		{
			return 	SEARCH_RESTART;
		}
		
	}
	
	if(step_remain!= 0)
	{
	
		lens_set_focus_step(step_remain);
		
		if(drive_filter_update(MOTOR_RUN)==SEARCH_RESTART)
		{
			return	SEARCH_RESTART;
		}
		
		if(motion_detect_all(motion_th, 300)==MOTION_DETECT)
		{
			return 	SEARCH_RESTART;
		}
	
	}

	lens_set_focus_dir(bak_dir);
	lens_set_focus_step(bak_step);
	
	if(motion_detect_all_cnt(2, motion_th, 300)==MOTION_DETECT)
	{
		return SEARCH_RESTART;
	}
	

	
	return SEARCH_CONTINUE;
 }
#endif

int32s	get_lpf_peak_pos_prior_step(int32s win_idx)
{
	return	filter_statistic_p[win_idx].lpf_peak_step_list[TRACE_LEN - 3];
}

int32s	get_lpf_peak_pos_next_step(int32s win_idx)
{

	return	filter_statistic_p[win_idx].lpf_peak_step_list[TRACE_LEN - 2];
}

int32s	get_lpf_peak_pos_real(int32s focus_dir, int32s win_idx)
{
	int32s 	step1;
	int32s	step2;
	int32s	peak_pos;
	int32s	peak_pos_real;
	
	step1=filter_statistic_p[win_idx].lpf_peak_step_list[TRACE_LEN - 1];
	step2=filter_statistic_p[win_idx].lpf_peak_step_list[TRACE_LEN - 2];
	peak_pos=get_lpf_peak_pos(win_idx);
		
	if(focus_dir==LENS_FOCUS_FAR)
	{
		peak_pos_real=peak_pos -step1-step2; 
	}
	else
	{
		peak_pos_real=peak_pos +step1+step2;
	}

	return	peak_pos_real;
}

int32s	get_hpf_peak_pos_prior_step(int32s win_idx)
{
	return	filter_statistic_p[win_idx].hpf_peak_step_list[TRACE_LEN - 3];
}

int32s	get_hpf_peak_pos_next_step(int32s win_idx)
{

	return	filter_statistic_p[win_idx].hpf_peak_step_list[TRACE_LEN - 2];
}

int32s	get_hpf_peak_pos_real(int32s focus_dir, int32s win_idx)
{
	int32s 	step1;
	int32s	step2;
	int32s	peak_pos;
	int32s	peak_pos_real;
	
	step1=filter_statistic_p[win_idx].hpf_peak_step_list[TRACE_LEN - 1];
	step2=filter_statistic_p[win_idx].hpf_peak_step_list[TRACE_LEN - 2];
	peak_pos=get_hpf_peak_pos(win_idx);
		
	if(focus_dir==LENS_FOCUS_FAR)
	{
		peak_pos_real=peak_pos -step1-step2; 
	}
	else
	{
		peak_pos_real=peak_pos +step1+step2;
	}

	return	peak_pos_real;
}


int32s	compute_krate_s(int32u curr_value, int32u prev_value)
{
	int32u 	value_diff;
	int32s	k_rate;
	
	value_diff=abs(curr_value - prev_value);

	if(value_diff > 4294960)
	{
		k_rate=value_diff/(prev_value/1000 == 0 ? 1 : prev_value/1000);
	}
	else
	{
		k_rate=value_diff*1000/(prev_value==0 ? 1 : prev_value);
	}

	if(k_rate>1000000)
		k_rate=1000000;

	if(curr_value<prev_value)
	{
		k_rate=-k_rate;
	}

	
	return	k_rate;
}

int32s 	search_limit(int32s focus_lpos, int32s focus_hpos, int32s motor_run)
{
	int32s 	focus_coord;
	int32s 	focus_step;
	int32s 	focus_dir;
	
	focus_coord	=lens_get_focus_cur_pos();
	focus_step	=lens_get_focus_step();
	focus_dir		=lens_get_focus_dir();
	
	if(motor_run)
	{
		if(focus_dir==LENS_FOCUS_FAR && (focus_coord-focus_step<=focus_lpos))
		{
			return FOCUS_CURVE_LOWER_LIMIT;
		}
		else if(focus_dir==LENS_FOCUS_NEAR && (focus_coord+focus_step>=focus_hpos))
		{
			return FOCUS_CURVE_UPPER_LIMIT;
		}
		else
		{
			return FOCUS_CURVE_UNLIMIT;
		}
	}
	else
	{
		if(focus_coord<focus_lpos)
			return FOCUS_CURVE_LOWER_LIMIT;
		else if(focus_coord>focus_hpos)
			return FOCUS_CURVE_UPPER_LIMIT;
		else
			return FOCUS_CURVE_UNLIMIT;
	}

}


int32s search_limit_check(int32s focus_lpos, int32s focus_hpos, int32s focus_step, int32s motor_run)
{
	int32s 	ret=FOCUS_UNLIMIT;
	
	if(FOCUS_CURVE_LOWER_LIMIT==search_limit(focus_lpos, focus_hpos, true))
	{
		if(focus_pos_arrive(focus_step, focus_lpos)==SEARCH_RESTART)
		{
			return SEARCH_RESTART;
		}

		limit_flag=1;
		//printf("L---curve bound--reverse motor dir, fpos=%d\r\n", lens_get_focus_cur_pos());
		lens_reverse_focus_dir();


		ret=FOCUS_LIMIT;
	}
	else if(FOCUS_CURVE_UPPER_LIMIT==search_limit(focus_lpos, focus_hpos, true))
	{
		if(focus_pos_arrive(focus_step, focus_hpos)==SEARCH_RESTART)
		{
			return SEARCH_RESTART;
		}

		limit_flag=2;
		//printf("U---curve bound--reverse motor dir, fpos=%d\r\n", lens_get_focus_cur_pos());
		lens_reverse_focus_dir();

		ret=FOCUS_LIMIT;
	}

	return ret;
}

int32s 	motion_detect_all(int32s th, int32s cnt_th)
{
	return STILL_DETECT;
}

int32s	motion_detect_all_cnt(int32s cnt, int32s th, int32s cnt_th)
{

	int32s		i;

	for(i=0; i<cnt; i++)
	{
		if(drive_filter_update(MOTOR_STOP)==SEARCH_RESTART)
		{
			return	MOTION_DETECT;
		}
		
		if(motion_detect_all(th, cnt_th)==MOTION_DETECT)
		{
			return 	MOTION_DETECT;
		}
	}

	return STILL_DETECT;	
}

int32s  drive_filter_update(motor_run)							
{																
	af_info_type  af_info_pk;
	int	i;

	
	if(motor_run)												
		lens_drive_focus_motor();	


	focus_msg_snd_c(af_focus_id, 3);
	
 	motor_msg_snd_c(af_motor_id, 3);							
	motor_msg_rcv_c(af_motor_id, 3);							
																
	//motor_msg_snd_c(af_focus_id, 3);								
	focus_msg_rcv_c(af_focus_id, 3);			

	
	focus_share_mem_read(af_focus_mem_id, (void*)(&af_info_pk), sizeof(af_info_type));


	//记录主窗口统计值	
	af_info.win_hpf[3]		=af_info_pk.main_data_fv1;
	af_info.win_lpf[3]		=af_info_pk.main_data_fv2;

	//记录中心窗口统计值
	af_info.win_hpf[0]		=af_info_pk.win_data_fv1[3];
	af_info.win_lpf[0]		=af_info_pk.win_data_fv2[3];
	
	af_info.win_hpf[1]		=af_info_pk.win_data_fv1[4];
	af_info.win_lpf[1]		=af_info_pk.win_data_fv2[4];

	af_info.win_hpf[2]		=af_info_pk.win_data_fv1[5];
	af_info.win_lpf[2]		=af_info_pk.win_data_fv2[5];

	//获取AE 的统计值
	af_info.ae_igain		=af_info_pk.ae_igain;
	af_info.ae_sgain		=af_info_pk.ae_sgain;
	af_info.ae_shut			=af_info_pk.ae_shut;

	af_info.nlumpNum		= af_info_pk.nlumpNum;

	// printf("AF: nlumpNum = %d\n",af_info_pk.nlumpNum;);
	filter_statistic_store();

	return	SEARCH_CONTINUE;
	
}

