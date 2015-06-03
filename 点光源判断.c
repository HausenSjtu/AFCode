
#define SEARCH_RANGE 3432
#define SEARCH_RANGE_SMALL 52


int GetBigYNum(int win_idx)
{

}


bool JudgeLight()
{
	int win_idx;

	win_idx = 3;

	if GetBigYNum(win_idx)>5000
		return true;
	else
		return false;

}

void NewFocus()
{
	if JudgeLight()
		auto_focus_proc()
	else
		search_Y()
}

void search_Y()
{

	int minYNum;
	int minYPos;

	int yNum;

	int win_idx;

	int yNumPrev;
	int down_cnt;


	win_idx = 3;



	printf("*********************start search_Y*********************\n");

	zoom_pos=lens_get_zoom_cur_pos();
	focus_pos=lens_get_focus_cur_pos();
	
	focus_hpos	=focus_pos+SEARCH_RANGE;
	focus_lpos	=focus_pos-SEARCH_RANGE;

	printf("-------zoom_pos=%d, Cur_Focus[%d] Focus Range[%d    %d]\r\n", zoom_pos,focus_pos, focus_lpos, focus_hpos);

	focus_pos_arrive(52, focus_hpos);
	printf("focus_pos_arrive to high pos:%d\n",focus_hpos);
	printf("focus_pos_arrive cur pos:%d\n",lens_get_focus_cur_pos());
	lens_set_focus_dir(LENS_FOCUS_FAR);
	printf("lens_set_focus_dir(LENS_FOCUS_FAR)\n");
	focus_step = 52;
	lens_set_focus_step(focus_step);
	printf("lens_set_focus_step(focus_step:%d)\n",focus_step);

	for(i=focus_hpos; i>=focus_lpos; i-=focus_step)
	{
		if(drive_filter_update(MOTOR_RUN)==SEARCH_RESTART)
		{
			
		}

		yNum = GetBigYNum(win_idx);
		if (yNum< minYNum)
		{
			minYNum = yNum;
			minYPos = lens_get_focus_cur_pos();
		}
	}

	focus_pos = minYPos + 0.5 * focus_step;

	printf("back to peak pos=%d , search peak!!!!!!!!!!!!\r\n", focus_pos);
	if(focus_pos_arrive(32,  focus_pos)==SEARCH_RESTART)
	{

	}

	yNumPrev = GetBigYNum(win_idx);
	down_cnt = 0;	

	for (int i = 0; i < 60; ++i)
	{
		drive_filter_update(MOTOR_RUN);
		drive_filter_update(MOTOR_STOP);
		drive_filter_update(MOTOR_STOP);

		yNum = GetBigYNum(win_idx);
		printf("focus_pos=%d, yNum=%d \r\n",lens_get_focus_cur_pos(), yNum);

		if (yNum > yNumPrev)
		{
			down_cnt++;
			lens_reverse_focus_dir();
			focus_step= focus_step / 2;
			lens_set_focus_step(focus_step);
			printf("------------reverse motor---------\r\n");
			printf("focus_pos = %d, yNumPrev = %d, yNum = %d\r\n", lens_get_focus_cur_pos(), yNumPrev, yNum);

		}

		if (down_cnt>=4)
		{
			drive_filter_update(MOTOR_RUN);
			drive_filter_update(MOTOR_STOP);
			drive_filter_update(MOTOR_STOP);

			printf("------------down_cnt>=4----------\r\n");
			printf("---------focus pos=%d, yNumPrev=%d yNum=%d\r\n",lens_get_focus_cur_pos(),yNumPrev,yNum);
			printf("----------------------\r\n");

			
			break;
		}

		yNumPrev = yNum;
	}

	printf("========================end search_Y========================================\r\n");

}