#include "focus_in.h"
#include "sys_log.h"

#define SEARCH_RANGE 3432

int32s	search_global(void)
{
	int32s	i;

	//记录极限值
	int32s	hpf_peak;
	int32s	hpf_valley;

	int32s	lpf_peak;
	int32s	lpf_valley;
	
	int32s	lpf_peak_pos;
	int32s	hpf_peak_pos;
	int32s	focus_pos;
	int32s	focus_peak_pos;
	
	int32s 	zoom_pos;
	int32s	focus_lpos;
	int32s	focus_hpos;
	int32s	check_ret;
	int32s	focus_step;
	int32s	step_cnt;
	
	int32u 	prev_lpf[4];
	int32u	prev_hpf[4];
	
	int32u 	curr_lpf[4];
	int32u	curr_hpf[4];

	int32s	lpf_ratio;
	int32s	hpf_ratio;
	
	int32s	motion_th;
	int32s 	down_cnt_search;
	int32s 	down_cnt_exit;
	int16s*	curve_lower_limit;
	int16s*	curve_upper_limit;

	int32s	win_idx;

	int32s	zoom_dir;

	int32s	search_cnt;
	int32s	down_cnt;

	OSTimeDly(100);
	
	LOG_DBG("***********************START SEARCH_CLIMB_SIMPLE*************************************\r\n");
	
	printf("-------SEARCH_RANGE------- %d---------\r\n",SEARCH_RANGE);
	printf("-------lens_get_focus_coord_max------- %d---------\r\n",lens_get_focus_coord_max());
	printf("-------lens_get_focus_coord_min------- %d---------\r\n",lens_get_focus_coord_min());
			
		
	zoom_pos=lens_get_zoom_cur_pos();
	focus_pos=lens_get_focus_cur_pos();
	
	focus_hpos	=focus_pos+SEARCH_RANGE;
	focus_lpos	=focus_pos-SEARCH_RANGE;

	if(focus_hpos>lens_get_focus_coord_max())
	{
		focus_hpos=lens_get_focus_coord_max();
	}

	if(focus_lpos<lens_get_focus_coord_min())
	{
		focus_lpos=lens_get_focus_coord_min();
	}
	LOG_DBG("SP---zoom_pos=%d, Cur_Focus[%d] Focus Range[%d    %d]\r\n", zoom_pos,focus_pos, focus_lpos, focus_hpos);
	
	//最小步长驱动focus
	down_cnt_search=0;
	down_cnt_exit=0;
	step_cnt=0;

	search_cnt=2;

	//小倍率选择最大窗口聚焦
	win_idx = 3;

	//移动focus到上限
	focus_pos_arrive(52, focus_hpos);
	LOG_DBG("focus_pos_arrive to high pos:%d\n",focus_hpos);
	
//	OSTimeDly(200);
	
	LOG_DBG("focus_pos_arrive cur pos:%d\n",lens_get_focus_cur_pos());
	
	//设置马达方向
	lens_set_focus_dir(LENS_FOCUS_FAR);
	LOG_DBG("lens_set_focus_dir(LENS_FOCUS_FAR)\n");

	//设置马达步长
	focus_step = 52;
	
	lens_set_focus_step(focus_step);
	LOG_DBG("lens_set_focus_step(focus_step:%d)\n",focus_step);
	
	motion_th=1000;

	//跟踪矩阵初始化
	filter_statistic_init();
	LOG_DBG("filter_statistic_init\n");
	
	if(drive_filter_update(MOTOR_STOP)==SEARCH_RESTART)
	{
	
	}

	for(i=focus_hpos; i>=focus_lpos; i-=focus_step)
	{
		//驱动马达一次
		if(drive_filter_update(MOTOR_RUN)==SEARCH_RESTART)
		{
			
		}
		//检测画面变化
		if(motion_detect_all(motion_th, 500)==MOTION_DETECT)
		{
			
		}
		prev_lpf[0]=get_prev_lpf(win_idx);
		prev_hpf[0]=get_prev_hpf(win_idx);

		
		curr_lpf[0]=get_win_lpf(win_idx);
		curr_hpf[0]=get_win_hpf(win_idx);	

		//计算增长率
		lpf_ratio = compute_krate_s(curr_lpf[0], prev_lpf[0]);
		hpf_ratio = compute_krate_s(curr_hpf[0], prev_hpf[0]);

		LOG_DBG("Fpos=%d, cur_lpf=%d, prev_lpf=%d, cur_hpf=%d, prev_hpf=%d, h_ratio=%d, l_ratio=%d\n", \
			lens_get_focus_cur_pos(), curr_lpf[0], prev_lpf[0], curr_hpf[0], prev_hpf[0],hpf_ratio, lpf_ratio);
		
	}
	
	//win_idx=3;
	lpf_peak_pos=get_lpf_peak_pos(win_idx);
	hpf_peak_pos=get_hpf_peak_pos(win_idx);

	hpf_peak=get_hpf_peak(win_idx);
	hpf_valley=get_hpf_valley(win_idx);

	lpf_peak=get_lpf_peak(win_idx);
	lpf_valley=get_lpf_valley(win_idx);


	LOG_DBG( "-----lpf_peak=%d, lpf_valley=%d\r\n", lpf_peak, lpf_valley);
	LOG_DBG( "-----hpf_peak=%d, hpf_valley=%d\r\n", hpf_peak, hpf_valley);


	focus_peak_pos=lpf_peak_pos;

	LOG_DBG("-----Lpf PeakPos=%d, Hpf PeakPos=%d\n", lpf_peak_pos, hpf_peak_pos);


	focus_pos=focus_peak_pos + 3*focus_step;
	

	if(focus_pos < focus_lpos)
	{
		focus_pos=focus_lpos;
	}
	else if(focus_pos > focus_hpos)
	{
		focus_pos=focus_hpos;
	}
	LOG_DBG("back to peak pos=%d , search peak!!!!!!!!!!!!\r\n", focus_pos);
	
	if(focus_pos_arrive(32,  focus_pos)==SEARCH_RESTART)
	{

	}

		//设置搜索步长
	focus_step=4;
	lens_set_focus_step(focus_step);
	drive_filter_update(MOTOR_STOP);

	prev_lpf[0]=get_win_lpf(win_idx);
	down_cnt=0;
	
	printf("------------start accurate search ---------\r\n");
	
	for(i=0;i<60;i++)
	{
	
		//驱动马达后的值不稳定
		//但要进行移动侦测
		drive_filter_update(MOTOR_RUN);
		drive_filter_update(MOTOR_STOP);
		drive_filter_update(MOTOR_STOP);
		//drive_filter_update(MOTOR_STOP);
		//drive_filter_update(MOTOR_STOP);
		//取马达停止后的值
		curr_lpf[0]=get_win_lpf(win_idx);

		LOG_DBG("SP---focus pos=%d, cur_lpf=%d pre_lpf=%d\r\n",lens_get_focus_cur_pos(),curr_lpf[0],prev_lpf[0]);


		//统计值下降
		if(curr_lpf[0]<prev_lpf[0])
		{
			down_cnt++;
			//反转马达
			lens_reverse_focus_dir();
			printf("------------reverse motor---------\r\n");
			ptz_init_focus();
			ptz_init_focus();
			LOG_DBG("-------focus pos=%d, cur_lpf=%d pre_lpf=%d\r\n",lens_get_focus_cur_pos(),curr_lpf[0],prev_lpf[0]);

		}

		if(down_cnt>=3)
		{
			//返回清晰点
			drive_filter_update(MOTOR_RUN);
			drive_filter_update(MOTOR_STOP);
			drive_filter_update(MOTOR_STOP);
			//drive_filter_update(MOTOR_STOP);
			//drive_filter_update(MOTOR_STOP);
			LOG_DBG("------------down_cnt>=3----------\r\n");
			LOG_DBG("-------peak=%d-------\r\n", lens_get_focus_cur_pos());
			LOG_DBG("------zhs---focus pos=%d, cur_lpf=%d pre_lpf=%d\r\n",lens_get_focus_cur_pos(),curr_lpf[0],prev_lpf[0]);
			LOG_DBG("----------------------\r\n");
			break;
		}

		prev_lpf[0]=curr_lpf[0];
	}
	
	
	
	LOG_DBG("***********************END ZHS_SEARCH_GLOBAL*************************************\r\n");

}

