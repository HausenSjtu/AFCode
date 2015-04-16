/*
 * drv_lens.h
 *
 *  Created on: Dec 21, 2011
 *      Author: leopard
 */

#ifndef DRV_LENS_H_
#define DRV_LENS_H_
typedef enum
{	MoveAbsolutely = 0,
	MoveRelatively = 1,
} LensMoveMethod;


typedef enum{
	LENS_STATUS_PTING,
	LENS_STATUS_PTED,
	LENS_STATUS_ZOOMING,
	LENS_STAUTS_ZOOMED,
	LENS_STATUS_FOCUSING,
	LENS_STAUTS_FOCUSED,
	LENS_STATUS_IDLE,
}LENS_WORKING_STATUS;

typedef enum{
	LENS_OP_RESULT_ERROR_ZOOMIN= -4,
	LENS_OP_RESULT_ERROR_ZOOMOUT= -3,
	LENS_OP_RESULT_ERROR_FAR= -2,
	LENS_OP_RESULT_ERROR_NEAR= -1,
	LENS_OP_RESULT_OK = 0,
	LENS_OP_RESULT_PI,

}LENS_OP_RESULT;

typedef enum
{
	LENS_FA_OK = 0,
	LENS_FA_ERR_ENDPOS = -1,
	LENS_FA_PI=2,
} LENS_FA_RESULT;


typedef enum
{
	LES_ZOOM_IN = 1, 	//tele
	LES_ZOOM_OUT = -1,	//wide
} ZOOM_DIR_DIR;

typedef enum
{
	LES_FA_DIR_FAR = 1, LES_FA_DIR_NEAR = -1,
} LES_FA_DIR;

// LensMoveMethod: Move Absolutely or Relatively



// LensFocusPara : consist of focusLength, focusStroke, focusStep.......
typedef struct
{
//	float focusLength; // the focusLength of Lens
//	int isZoomLens; // the zoom Lens or fixed focusLength
//	double focusStepLength; // focus step length of Lens 	(unit: mm/step)
//	long nearFocusStroke; // focus stroke @ Near Scope  (Near Scope: from ResetPosition to ForwardEnd(Near Direction) ) 		(unit:steps)
//	long farFocusStroke; // focus stroke @ Far Scope (Far Scope: from ResetPosition to BackwardEnd(Far Direction) )  	 (unit:steps)
	long focusTotalSteps; // the total focus stroke = (nearFocusStroke+ farFocusStroke)      (unit:steps)
//	double focusStroke; // focus stroke (the lens motion scope)		(unit: mm)
//	long maxFocusSpeed; // max Move Speed of Lens when Focusing		 (unit: pps(periods per second or pulses per second))
//	long runningFocusSpeed; // running Speed of Lens when Focusing  (unit: pps(periods per second))
	long focusResetPosition; // focus Reset Position   ( n steps from INF end��BackwardEnd��)    unit: steps)
//	double timeToMoveFocusStroke; // time cost to Move from BackwardEnd to  ForwardEnd(FocusStroke)  ( unit: second )
//	long farAllowance; // Mechanical space left for safety : beyond Far End  	(unit: steps)
//	long nearAllowance; // Mechanical space left for safety : beyond Near End 	(unit: steps)
//	double zoomStepLength; // zoom step of Lens 	(unit: mm/step)
//	long zoomWideStroke; // Wide area stroke of Zoom Lens	 (unit:steps)
//	long zoomTeleStroke; // Tele area stroke of Zoom Lens  	 (unit:steps)
	long zoomTotalSteps; // the total Zoom stroke = (zoomWideStroke+zoomTeleStroke)	 (unit:steps)
	long zoomResetPosition; // zoom Reset Position   ( n steps from INF end��BackwardEnd��)    unit: steps)
	long zoomRatio;
//	long WideAllowance; // Mechanical space left for safety : beyond wide End  	(unit: steps)
//	long TeleAllowance; // Mechanical space left for safety : beyond tele End  	(unit: steps)

} LensFocusPara;
/*
#ifndef  ABS
#define  ABS(x)                (((x) < 0) ? -(x) : (x))
#endif
*/

//#define RateOfChange(x,y)    ( ( (y)- (x)) / (x) )
//#define ROUND(a)         ((int)  ((a) + 0.5) )


#define		LENS_ZOOM_INIT				0x01
#define		LENS_FOCUS_INIT				0x02
#define		LENS_ZOOM_DRV					0x03
#define		LENS_FOCUS_DRV				0x04
#define		LENS_ZOOM_FOCUS_DRV			0x05
#define		LENS_MOTOR_RUNNING			0x06
#define		LENS_MOTOR_STOP				0x07
#define		LENS_MOTOR_STOPPED			0x08
#define		LENS_INIT						0x09

#define		LENS_FOCUS_FAR				0x10
#define		LENS_FOCUS_NEAR				0x11

#define		LENS_ZOOM_TELE				0x20
#define		LENS_ZOOM_WIDE				0x21	

#define		ZOOM_MOTOR_SEL				0x01
#define		FOCUS_MOTOR_SEL				0x00

#define		MOTOR_RUN						0x01
#define		MOTOR_STOP					0x00


/*****************************************************

				LENS Coordination Para

*****************************************************/
#define		ZOOM_COORD_MAX				4980
#define		ZOOM_COORD_MIN				0

#define		FOCUS_COORD_MAX				1600
#define		FOCUS_COORD_MIN				-1000

#define		ZOOM_PI_COORD					3740
#define		FOCUS_PI_COORD				(480+200)

#define		ZOOM_STEP_MAX					52
#define		ZOOM_STEP_MIN					0

#define		FOCUS_STEP_MAX				104
#define		FOCUS_STEP_MIN				0


void 	OSTimeDly(int msecs);

int 		lens_dev_open(void);
int 		lens_dev_close(void);

void 	lens_init_zoom_motor(void);
void 	lens_init_focus_motor(void);
void 	lens_drive_zoom_motor(void);
void 	lens_drive_focus_motor(void);
void 	lens_set_focus_dir(int focus_dir);
int 		lens_get_focus_dir(void);
void 	lens_reverse_focus_dir(void);
void 	lens_set_zoom_dir(int zoom_dir);
int 		lens_get_zoom_dir(void);
void 	lens_reverse_zoom_dir(void);

void 	lens_set_focus_step(int focus_step);
void 	lens_set_zoom_step(int zoom_step);

void 	lens_set_zoom_cur_pos(int zoom_pos);
int 		lens_get_zoom_cur_pos(void);
int 		lens_get_zoom_coord_max(void);
int 		lens_get_zoom_coord_min(void);

void 	lens_set_focus_cur_pos(int focus_pos);
int 		lens_get_focus_cur_pos(void);
int 		lens_get_focus_coord_max(void);
int 		lens_get_focus_coord_min(void);

int 		lens_get_zoom_sensor_stat(void);
int 		lens_get_focus_sensor_stat(void);
void 	lens_drive_motor(void);

void 	lens_motor_pi_switch(int pi_switch);
void 	lens_motor_lock_init(void);
void 	lens_motor_lock(void);
void 	lens_motor_unlock(void);
void 	lens_wait_motor_stop(void);

int		af_key_io_init(void);
int		af_key_io_stat_rd(void);

#endif /* DRV_LENS_H_ */
