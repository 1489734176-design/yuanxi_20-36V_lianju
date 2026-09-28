#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H


#include "stdint.h"
#include "mm32spin0230.h"



typedef enum 
{		
    MOTOR_STOP = 0,           //停止状态
    MOTOR_MOTION = 1,		      // 查询状态
    MOTOR_POSITION = 2,	      // 测电感
    MOTOR_RUN = 3, 		       	// 闭环
    MOTOR_SBRAKE = 4,         //软刹车
    MOTOR_HBRAKE = 5,         //硬刹车
    MOTOR_BRAKE2STOP = 6,     //刹车到停止过度
    MotorStart_Drag_OPENLOOP = 7,    
		Motor_Align=8,
    MOTOR_READY = 9,    
    MOTOR_STOPWAIT = 10,    
    MOTOR_MOTION_FAIL = 11,		      // 查询状态    
		MOTOR_BRAKE_WAIT = 12,		      // 查询状态   
}MC_StateMachine_e;
typedef struct PidPI
{
		int32_t 	kp2;
    int32_t 	kp; 
    int32_t 	ki; 
    int32_t 	error; 
    int32_t 	error1; 
    int32_t 	error2;
    int32_t 	refValue; 
    int32_t 	output; 
    int32_t 	proportion; 
    int64_t 	errorIntegral;   
    int32_t 	lowerLimitOutput;   //Lower Limit for Output limitation
    int32_t 	upperLimitOutput;   //Lower Limit for Output limitation
    int32_t 	maxIntegralRatio;   
    int32_t 	lowerLimitIntegral; //Lower Limit for Integral term limitation
    int32_t 	upperLimitIntegral; //Lower Limit for Integral term limitation


    int32_t 	pTemp;
    int32_t 	iTemp;

    
    int32_t 	deltaUk; //deltaUk = Uk - Uk_1;  Uk为位置式PI输出

    int32_t 	deltaOut;
    int32_t 	rpmSetPwmDuty;

    int32_t 	KpDuty;
    int32_t 	KiDuty;
    int32_t 	OutDuty;
		int32_t  Limit_max_data;
		int32_t  Limit_min_data;
} PidPI_t;
typedef struct 
{

	MC_StateMachine_e bldc_state;



	uint8_t u8Calibration_OK_flag;
	uint8_t u8CurrentOffset_error_flag;
	uint16_t CurrentOffset_ad_data;
	uint16_t pwm_duty_aim;
	uint16_t pwm_duty_aim2;
	uint16_t pwm_duty_aim_per;//PWM目标预备值
	uint16_t pwm_duty_virtual;
	uint8_t  hbrake_en;
	uint8_t  sbrake_en;
	uint16_t sbreak_pwm_duty;

	uint8_t motor_run_last_dir;
	uint32_t motor_run_count;
	uint32_t motor_speed;
	uint32_t	time_count_60_degree_filter;

}Motor_Control_list;
extern  Motor_Control_list motor_control_list;
#if LIMIT_SPEED_EN
extern PidPI_t Speed_Pid;
#endif
void MC_Machine_State(void);
void CurrentOffsetCalibration(void);
void mose_check(void);


void start_mc_init(void);
void align_control(void);
u8 drag_control(void);
void drag_control_init(void);

void  Speed_PI_init(void);
int32_t MC_PI_Handler( PidPI_t *pi,int32_t errorBack);
void get_speed(void);
void  Speed_PI_reset(void);
#endif
