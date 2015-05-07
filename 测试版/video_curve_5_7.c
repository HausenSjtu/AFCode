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
0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512,544,576,608,640,672,704,736,768,800,832,864,896,928,960,992,1024,1056,1088,1120,1152,1184,1216,1248,1280,1312,1344,1376,1408,1440,1472,1504,1536,1568,1600,1632,1664,1696,1728,1760,1792,1824,1856,1888,1920,1952,1984,2016,2048,2080,2112,2144,2176,2208,2240,2272,2304,2336,2368,2400,2432,2464,2496,2528,2560,2592,2624,2656,2688,2720,2752,2784,2816,2848,2880,2912,2944,2976,3008,3040,3072,3104,3136,3168,3200,3232,3264,3296,3328,3360,3392,3424,3456,3488,3520,3552,3584,3616,3648,3680,3712,3744,3776,3808,3840,3872,3904,3936,3968,4000,4032,4064,4096,4128,4160,4192,4224,4256,4288,4320,4352,4384,4416,4448,4480,4512,4544,4576,4608,4640,4672,4704,4736,4768,4800,4832,4864,4896,4928,4960,4992
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
4501,4789,5052,5290,5506,5701,5878,6037,6181,6311,6428,6534,6630,6717,6795,6868,6934,6995,7052,7106,7157,7207,7255,7302,7348,7395,7442,7490,7540,7590,7643,7697,7754,7813,7874,7937,8004,8072,8143,8217,8294,8372,8453,8537,8622,8710,8799,8891,8983,9078,9173,9270,9368,9466,9565,9664,9764,9863,9963,10061,10159,10257,10354,10449,10543,10635,10727,10816,10903,10988,11072,11152,11231,11306,11379,11450,11518,11583,11645,11704,11761,11813,11864,11911,11956,11997,12036,12072,12105,12135,12163,12189,12211,12232,12251,12267,12282,12294,12305,12315,12323,12331,12337,12342,12347,12351,12356,12360,12364,12368,12373,12378,12385,12392,12400,12410,12421,12434,12448,12464,12482,12501,12523,12547,12573,12601,12630,12663,12696,12732,12769,12807,12848,12889,12930,12973,13015,13058,13099,13140,13179,13216,13250,13280,13306,13328,13343,13351,13352,13344,13325,13296,13254,13198,13127,13040,12934
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
