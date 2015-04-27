#include <avserver.h>
#include <aewb_xdm.h>
#include <alg_aewb.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <drv_lens.h>
#include <drv_sync.h>
#include <video_2a.h>


//设置曲线的坐标数
#define	ZOOM_COORD_NUM					161

#define	CURVE_NUM							3
#define 	CURVE_COORD_NUM					ZOOM_COORD_NUM



const int	zoom_coord_tbl[]=
{
0,32,64,96,128,160,192,224,256,
288,320,352,384,416,448,480,512,544,
576,608,640,672,704,736,768,800,832,
864,896,928,960,992,1024,1056,1088,1120,
1152,1184,1216,1248,1280,1312,1344,1376,1408,
1440,1472,1504,1536,1568,1600,1632,1664,1696,
1728,1760,1792,1824,1856,1888,1920,1952,1984,
2016,2048,2080,2112,2144,2176,2208,2240,2272,
2304,2336,2368,2400,2432,2464,2496,2528,2560,
2592,2624,2656,2688,2720,2752,2784,2816,2848,
2880,2912,2944,2976,3008,3040,3072,3104,3136,
3168,3200,3232,3264,3296,3328,3360,3392,3424,
3456,3488,3520,3552,3584,3616,3648,3680,3712,
3744,3776,3808,3840,3872,3904,3936,3968,4000,
4032,4064,4096,4128,4160,4192,4224,4256,4288,
4320,4352,4384,4416,4448,4480,4512,4544,4576,
4608,4640,4672,4704,4736,4768,4800,4832,4864,
4896,4928,4960,4992,5024,5056,5088,5120
};

struct focus_curve
{
	//聚焦曲线
	short			curve_3m[CURVE_COORD_NUM];

	short*			curve_array[1];

};


struct focus_curve focus_curve_info=
{
//聚焦曲线
{0,},

//聚焦曲线数组
{
focus_curve_info.curve_3m,
},


};




 	
short	curve_3m_table[]=
{
8656,8528,8468,8466,8336,8112,7920,7760,7568,
7376,7248,7088,7024,6832,6640,6480,6448,6224,
6096,6000,5872,5808,5552,5488,5296,5200,5104,
4944,4880,4816,4688,4656,4496,4336,4272,4144,
4080,3984,3856,3824,3792,3600,3632,3504,3376,
3312,3312,3248,3088,3024,2960,2960,2800,2800,
2672,2608,2544,2448,2448,2352,2352,2288,2224,
2128,2128,2032,2032,2032,1904,1904,1904,1744,
1712,1712,1712,1712,1584,1584,1520,1424,1424,
1360,1328,1328,1264,1264,1264,1264,1264,1136,
1104,1072,1072,1072,944,944,944,944,944,
944,816,816,816,816,816,816,816,816,
656,656,656,656,656,656,656,656,528,
496,496,496,496,496,496,496,496,496,
496,496,496,496,432,432,432,432,432,
432,432,432,400,400,400,400,400,400,
400,400,400,400,400,400,400,304,368,
368,368,368,368,368,368,368,368
};

/****************************************************************************************
*
*						        获取特定的Focus Curve
*
***************************************************************************************/

short* 	focus_curve_3m_get(void)
{
	return 	focus_curve_info.curve_3m;
}

int		focus_curve_coord_arrive(void)
{
	int	zoom_pos;
	int	focus_pos;
	int	i;
	int	idx;
	int	zoom_dir;

	zoom_pos	=lens_get_zoom_cur_pos();
	zoom_dir		=lens_get_zoom_dir();

	//fprintf(stderr, "Zoom Cur Pos=%d\r\n", zoom_pos);
	for(i=0; i<sizeof(zoom_coord_tbl)/sizeof(zoom_coord_tbl[0]); i++)
	{
		if(zoom_pos==zoom_coord_tbl[i])
		{
			idx=i;
			break;
		}
	}

	if(idx>=sizeof(zoom_coord_tbl)/sizeof(zoom_coord_tbl[0]))
	{
		idx=sizeof(zoom_coord_tbl)/sizeof(zoom_coord_tbl[0]) - 1;
	}

	focus_pos=curve_3m_table[idx];
	fprintf(stderr, "======zhs=====Focus Cur Pos=%d\r\n", focus_pos);

	#if 0
	if(zoom_dir==LENS_ZOOM_WIDE)
	{
		focus_pos+=40;
		//fprintf(stderr, "ZWide: Focus Arrive Pos=%d\r\n", focus_pos);
		
	}
	else
	{
		focus_pos-=40;
		//fprintf(stderr, "ZTele: Focus Arrive Pos=%d\r\n", focus_pos);
		
	}
	#endif
	
	lens_goto_motor_pos(FOCUS_MOTOR_SEL, focus_pos, 32);

	fprintf(stderr, "Focus Pos Arrived -----\r\n\r\n\r\n");
	
}


short  	get_focus_index_curve(int  zoom_pos,  short *fcurve)
{
	short 	i;
	short	focus_pos;
	int		focus_idx;

	//首先获取ZOOM的坐标等级
	//(0		-	2000) 		Lvl0
	//(2000	-	4360) 		Lvl1
	//(4360 	-	4500)		Lvl2
	//(4500 	-	4680)		Lvl3

	focus_idx=0;
	for(i=0; i<ZOOM_COORD_NUM; i++)
	{
		if(zoom_pos<=zoom_coord_tbl[i])
		{
			focus_idx=i;
			break;
		}
	}

	//fprintf(stderr,"zoom pos=%d, zoom rank idx=%d\r\n", zoom_pos, i);
	
	if(focus_idx>=ZOOM_COORD_NUM)
	{
		//ZOOM最大
		return  	focus_curve_info.curve_3m[CURVE_COORD_NUM-1];
	}
	else
	{
		return	focus_curve_info.curve_3m[focus_idx];
	}
	
}


void 	focus_curve_data_init(void)
{
	int 	i;


	for(i=0; i<CURVE_COORD_NUM; i++)
	{
		focus_curve_info.curve_3m[i]=curve_3m_table[i];
	}
	
}


void 	focus_para_init(void)
{
	focus_curve_data_init();
}
