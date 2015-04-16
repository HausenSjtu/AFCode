#ifndef 	_FOCUS_IN_H_
#define	_FOCUS_IN_H_

//引用外部的头文件

//AVServer
#include <avserver.h>
#include <alg_aewb.h>
#include <video_2a.h>

//System 
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

//FrameWork Drv
#include <drv_sync.h>
#include <drv_lens.h>



#ifdef	FOCUS_IN_GLOBAL
	#define	FOCUS_IN_EXT		
#else
	#define	FOCUS_IN_EXT	extern
#endif

#if 0
#define	FOCUS_PARA_DBG
#define 	SEARCH_SCENE_ESTIMATE_DBG
#define 	COMMON_SCENE_DIR_VERIFY_DBG
#define 	MOTION_DETECT_DBG
#define 	STILL_DETECT_DBG
#define 	SEARCH_SWAY_DIR_DBG
#define 	SEARCH_SIMPLE_DBG
#define 	SEARCH_FAST_DBG
#define 	SEARCH_CLIMB_MOUNT_DBG
#define 	SEARCH_PEAK_DBG
#define 	SEARCH_NO_DETAIL_DBG
#define	SEARCH_CURVE_COORD_DBG
#endif

#ifdef SEARCH_SCENE_ESTIMATE_DBG
	#define	sse_prt(format,arg...) printf(format,##arg)
#else
	#define	sse_prt(format,arg...)
#endif


#ifdef COMMON_SCENE_DIR_VERIFY_DBG
	#define	csdv_prt(format,arg...) printf(format,##arg)
#else
	#define	csdv_prt(format,arg...)
#endif


#ifdef MOTION_DETECT_DBG
	#define	md_prt(format,arg...) printf(format,##arg)
#else
	#define	md_prt(format,arg...)
#endif


#ifdef STILL_DETECT_DBG
	#define	sd_prt(format,arg...) printf(format,##arg)
#else
	#define	sd_prt(format,arg...)
#endif


#ifdef SEARCH_SWAY_DIR_DBG
	#define	ssd_prt(format,arg...) printf(format,##arg)
#else
	#define	ssd_prt(format,arg...)
#endif


#ifdef SEARCH_SIMPLE_DBG
	#define	ss_prt(format,arg...) printf(format,##arg)
#else
	#define	ss_prt(format,arg...)
#endif


#ifdef SEARCH_FAST_DBG
	#define	sf_prt(format,arg...) printf(format,##arg)
#else
	#define	sf_prt(format,arg...)
#endif


#ifdef SEARCH_CLIMB_MOUNT_DBG
	#define	scm_prt(format,arg...) printf(format,##arg)
#else
	#define	scm_prt(format,arg...)
#endif


#ifdef SEARCH_PEAK_DBG
	#define	sp_prt(format,arg...) printf(format,##arg)
#else
	#define	sp_prt(format,arg...)
#endif


#ifdef SEARCH_NO_DETAIL_DBG
	#define	snd_prt(format,arg...) printf(format,##arg)
#else
	#define	snd_prt(format,arg...)
#endif


#ifdef SEARCH_CURVE_COORD_DBG
	#define	scc_prt(format,arg...) printf(format,##arg)
#else
	#define	scc_prt(format,arg...)
#endif

#if 0
#define	int32s		(signed int)			
#define	int32u		(unsigned int)

#define	int16s		(signed short)
#define	int16u		(unsigned short)	

#define	int8s		(signed char)
#define	int8u		(unsigned char)		
#endif


#ifndef		_INTEGER_TYPE_

#define		_INTEGER_TYPE_
typedef		signed int			int32s;	
typedef		unsigned int		int32u;

typedef		signed short		int16s;
typedef		unsigned short	int16u;

typedef		signed char		int8s;
typedef		unsigned char		int8u;

#endif

#define 		true     			1
#define 		false    			0

//保存聚焦统计数据数组的长度
#define	TRACE_LEN						10
#define	CURVE_DATA_INIT				99999999

#define 	FRAMES_DETECT_NUM			10
#define	FRAMES_DETECT_NUM_AE		5

/****************************************************************************************
*
*								聚焦统计值窗口大小
*
****************************************************************************************/
//窗口A的水平坐标
#define 	WIN1_VSTART					0x080	
#define	WIN1_HSTART					0x0A0
//窗口A的垂直坐标
#define	WIN1_HEIGHT					0x360
#define	WIN1_WIDTH					0x200

//窗口B的水平坐标
#define	WIN2_VSTART					0x080
#define	WIN2_HSTART					0x2A0
//窗口B的垂直坐标
#define	WIN2_HEIGHT					0x360
#define	WIN2_WIDTH					0x200

//窗口C的水平坐标
#define	WIN3_VSTART					0x080
#define	WIN3_HSTART					0x4A0
//窗口C的垂直坐标
#define	WIN3_HEIGHT					0x360
#define	WIN3_WIDTH					0x200

//窗口D的水平坐标
#define	WIN4_VSTART					0x080
#define	WIN4_HSTART					0x0A0
//窗口D的垂直坐标
#define	WIN4_HEIGHT					0x360
#define	WIN4_WIDTH					0x600


//窗口A的水平坐标
#define 	WIN5_VSTART					0x100	
#define	WIN5_HSTART					0x180
//窗口A的垂直坐标
#define	WIN5_HEIGHT					0x260
#define	WIN5_WIDTH					0x450


/****************************************************************************************
*
*									聚焦参数
*
***************************************************************************************/
#define	PARA_VLINESEL_L				0x04
#define	PARA_HLCTH_L					0x03
#define	PARA_HBGATE_SW				0x01
#define	PARA_HBGTH_L					0x0f



/****************************************************************************************
*
*						跑电机还是电机停止时获取统计值
*
****************************************************************************************/
#define 	MOTOR_RUN						1
#define	MOTOR_STOP					0

/****************************************************************************************
*
*									Search Policy	
*
****************************************************************************************/
#define	SEARCH_CONTINUE				0xff
#define	SEARCH_RESTART				0xfe
#define	SEARCH_END					0xfd


#define	SEARCH_FAST					0xa0
#define	SEARCH_CLIMB_MOUNT			0xa1
#define	SEARCH_DOWN_MOUNT			0xa2
#define	SEARCH_PEAK					0xa3
#define	SEARCH_VALLEY					0xa4
#define	SEARCH_NO_DETAIL				0xa5
#define	SEARCH_NO_DETAIL_FAST		0xa6


/****************************************************************************************
*
*									Search Policy
*
****************************************************************************************/
#define	SEARCH_PEAK_SIMPLE			0
#define	SEARCH_CLIMB_SIMPLE			1
#define	SEARCH_PEAK_NEARBY			2
#define	SEARCH_TURNING_POINT			3
#define	SEARCH_FAR_AWAY				4
#define	SEARCH_LSUM_LOW				5
#define	SEARCH_LOW_LIGHT				6
#define	SEARCH_CLIMB_COMPLEX			7
#define	SEARCH_PEAK_PRECISE			8
#define	SEARCH_GLOBAL					9
#define	SEARCH_FAST_LOW_LUMA		10


/****************************************************************************************
*
*									Curve Limit
*
****************************************************************************************/
#define	FOCUS_CURVE_LOWER_LIMIT  	1
#define	FOCUS_CURVE_UPPER_LIMIT        	2
#define	FOCUS_CURVE_UNLIMIT	 		3


#define	FOCUS_LIMIT					1
#define	FOCUS_UNLIMIT					2

/****************************************************************************************
*
*									AGC	Level
*
****************************************************************************************/
#define	AGC_LEVEL0						0
#define	AGC_LEVEL1						1
#define	AGC_LEVEL2						2
#define	AGC_LEVEL3						3
#define	AGC_LEVEL4						4
#define	AGC_LEVEL5						5


/****************************************************************************************
*
*								Motion Dection Thresh
*
****************************************************************************************/
#if 0
#define	MOTION_TH_4						600					
#define	MOTION_TH_8						1000
#define	MOTION_TH_12					1500
#define	MOTION_TH_16					2000
#define	MOTION_TH_24					5000
#endif

#if 0
#define	MOTION_TH_4					500					
#define	MOTION_TH_8					800
#define	MOTION_TH_12					1000
#define	MOTION_TH_16					1200
#define	MOTION_TH_24					1800
#define	MOTION_TH_32					2600
#endif

#if 0
#define	MOTION_TH_4					300					
#define	MOTION_TH_8					500
#define	MOTION_TH_12					800
#define	MOTION_TH_16					1000
#define	MOTION_TH_24					1200
#define	MOTION_TH_32					1500
#endif

#define	MOTION_TH_4					260					
#define	MOTION_TH_8					320
#define	MOTION_TH_12					500
#define	MOTION_TH_16					600
#define	MOTION_TH_24					800
#define	MOTION_TH_32					1000

/****************************************************************************************
*
*								Motion Dection Result
*
****************************************************************************************/
#define  	STILL_DETECT					0xe0
#define	MOTION_DETECT					0xe1


/****************************************************************************************
*
*						判断方向时,用来保存统计值的临时缓冲区长度
*						以及不同曲线统计值在缓冲区中的位置索引
*
****************************************************************************************/
#define	STATISTIC_BUF_LENGTH			4

#define	LSUM_IDX						0
#define 	HSUM_IDX						STATISTIC_BUF_LENGTH
#define	HPF_IDX						(2*STATISTIC_BUF_LENGTH)
#define	LPF_IDX							(3*STATISTIC_BUF_LENGTH)
#define 	CNT_IDX						(4*STATISTIC_BUF_LENGTH)
#define	BUF_SIZE						(5*STATISTIC_BUF_LENGTH)

/****************************************************************************************
*
*						保存统计值的临时缓冲区
*
****************************************************************************************/
FOCUS_IN_EXT	int32s 	statistic_buf[BUF_SIZE];


#define	TELE_DIRECTION					1
#define	WIDE_DIRECTION				0

#define	AF_LIMIT						FOCUS_LIMIT


/****************************************************************************************
*
*								Scene definition
*
****************************************************************************************/
#define	FAR_AWAY							0
#define	SPOT_LIGHT							1
#define	NO_DETAIL							2
#define	LSUM_LOW							3
#define	COMMON_SCENE						4
#define	SIMPLE_SCENE						5
#define	RICH_SCENE							6


/****************************************************************************************
*
*						曲线上升还是下降趋势
*
****************************************************************************************/
#define	CURVE_DOWN_TREND 				1
#define	CURVE_UP_TREND					0


/****************************************************************************************
*
*						重新聚焦时,有可能已经位于清晰点					
*						判断是曲线尖峰的HSUM阈值
*						位于尖峰时直接调用Search Peak函数
*
****************************************************************************************/
#define 	PEAK_HSUM_THRESH					10000


/****************************************************************************************
*
*								Search Fast Para
*
****************************************************************************************/
#define	HSUM_RATE_DAY_TH					1.3
#define	HSUM_RATE_NIGHT_TH				1.2


/***************************************************************************************
*
*						     			夜晚场景的增益阈值
*
***************************************************************************************/
#define	NIGHT_GAIN_THRESH				(0x10)


/***************************************************************************************
*
*						     			场景变化(重新聚焦)的阈值
*
***************************************************************************************/
#define	SCENE_CHANGE_THH					135

/***************************************************************************************
*
*						     			移动监测阈值
*
***************************************************************************************/
#define	SCENE_MOTION_MAIN_THH			50
#define	SCENE_MOTION_SUB_THH				100

#define	SCENE_CNT_THH						200

#define	STATISTICS_MIN						200

/***************************************************************************************
*
*						     			低照度阈值
*
***************************************************************************************/
#define	LOW_LUMA_SGAIN_THH				80000	

#define	LOW_LUMA_SGAIN_THH1				16000
#define	LOW_LUMA_SGAIN_THH2				30000	
#define	LOW_LUMA_SGAIN_THH3				60000


/****************************************************************************************
*
*							滤波器统计值记录单元
*
****************************************************************************************/
struct filter_statistic_unit
{
	int32u 	fpos;
	int32u	fstep;
	int32u 	lpf;
	int32u 	hpf;
	int32u	ae_shut;
	int32u	ae_sgain;
	int32u	ae_igain;
};

//用来保存滤波器统计值的"尖峰" 值
struct filter_statistic_peak
{
	int32s lpf_peak_pos;
	int32u lpf_peak;

	int32s hpf_peak_pos;
	int32u hpf_peak;

	int32s lpf_peak_step_list[TRACE_LEN];
	int32s hpf_peak_step_list[TRACE_LEN];	
};

//用来保存滤波器统计值的"低谷" 值
struct filter_statistic_valley
{

	int32s lpf_valley_pos;
	int32u lpf_valley;

	int32s hpf_valley_pos;
	int32u hpf_valley;

};

FOCUS_IN_EXT	struct filter_statistic_unit 	filter_statistic_q[4][TRACE_LEN];
FOCUS_IN_EXT	struct filter_statistic_peak 	filter_statistic_p[4];
FOCUS_IN_EXT	struct filter_statistic_valley 	filter_statistic_v[4];


/****************************************************************************************
*
*							滤波器统计值API
*
****************************************************************************************/

#define 	get_curr_lpf(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-1].lpf)
#define	get_curr_hpf(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-1].hpf)

#define 	get_prev_lpf(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-2].lpf)
#define	get_prev_hpf(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-2].hpf)

#define 	get_lpf_peak(win_idx)			(filter_statistic_p[win_idx].lpf_peak)
#define 	get_lpf_peak_pos(win_idx)		(filter_statistic_p[win_idx].lpf_peak_pos)

#define 	get_hpf_peak(win_idx)			(filter_statistic_p[win_idx].hpf_peak)
#define 	get_hpf_peak_pos(win_idx)		(filter_statistic_p[win_idx].hpf_peak_pos)

#define 	get_lpf_valley(win_idx)			(filter_statistic_v[win_idx].lpf_valley)
#define 	get_lpf_valley_pos(win_idx)		(filter_statistic_v[win_idx].lpf_valley_pos)

#define 	get_hpf_valley(win_idx)			(filter_statistic_v[win_idx].hpf_valley)
#define 	get_hpf_valley_pos(win_idx)		(filter_statistic_v[win_idx].hpf_valley_pos)


#define 	get_curr_shut(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-1].ae_shut)
#define	get_curr_sgain(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-1].ae_sgain)
#define	get_curr_igain(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-1].ae_igain)

#define 	get_prev_shut(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-2].ae_shut)
#define	get_prev_sgain(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-2].ae_sgain)
#define	get_prev_igain(win_idx)			(filter_statistic_q[win_idx][TRACE_LEN-2].ae_igain)


#define 	MIN(x, y) 				( ((x) <= (y)) ? (x) : (y) )
#define	MAX(x, y)				( ((x) >= (y)) ? (x) : (y) )


/****************************************************************************************
*
*							聚焦窗口的选择句柄
*
****************************************************************************************/
FOCUS_IN_EXT	int32s	win_sel;

FOCUS_IN_EXT	int32s	limit_flag;

//曲线统计信息
struct autofocus_info
{
	int32u	win_lpf[4];
	int32u	win_hpf[4];
	
	//int32u	win_cnt;
	int32u	ae_shut;
	int32u	ae_sgain;
	int32u	ae_igain;
};

typedef	struct
{
	int32u	win_value[3];
	//int32u	cnt_value;
	int32u	ae_shut;
	int32u	ae_sgain;
	int32u	ae_igain;
}sta_type;

FOCUS_IN_EXT	struct autofocus_info	af_info;
FOCUS_IN_EXT	volatile int8u  af_info_rdstatus;

/****************************************************************************************
*
*							获取窗口统计值API
*
****************************************************************************************/
#define	get_win_lpf(win_idx)			(af_info.win_lpf[win_idx])
#define	get_win_hpf(win_idx)		(af_info.win_hpf[win_idx])
#define	get_win_shut()				(af_info.ae_shut)
#define	get_win_sgain()				(af_info.ae_sgain)
#define	get_win_igain()				(af_info.ae_igain)


/****************************************************************************************
*
*							zhs test how can it store the data
*
****************************************************************************************/
int32u zhs_pre_focus_pos = 0 ;
int32s zhs_pre_lpf = 0;
Bool zhs_mark_have_done = FALSE;


#endif	/*_FOCUS_IN_H_*/
