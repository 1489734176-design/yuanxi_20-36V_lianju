#ifndef __MC_CORE_H
#define __MC_CORE_H

#include "stdint.h"
#include "mm32spin0230.h"
typedef struct
{
	__IO  uint8_t  	u8ADC_Timer_flag;
	
	__IO  uint16_t  u16BemfU;
	__IO  uint16_t  u16BemfV;
	__IO  uint16_t  u16BemfW;
	__IO  uint16_t  u16BemfZero;
//	__IO  uint16_t  u16BemfZeroHalf;
//	__IO  uint16_t  u16BemfZero_1_5;
//	__IO  uint16_t  u16BemfZeroRise_THD;
//	__IO  uint16_t  u16BemfZerofall_THD;

	__IO  uint16_t  u16I_Peak;	
	__IO  uint16_t  u16I_Peak_calculate;	
	__IO  uint16_t  u16I_SUM_Avg;		
	__IO  uint16_t  u16Vbus;
	__IO  uint16_t  u16Ref2V5;
	__IO  uint16_t  u16NTC_MOS;
	__IO  uint16_t  u16NTC_BAT;
	__IO  uint16_t  u16VR;
	



} ADC_Value_TypeDef;


 /*motor error */
typedef union
{
    uint16_t word;
    struct
    {
		uint16_t	gIPDOCP:1;
		uint16_t	gLackPhase:1;            //电机缺相,任意扇区IPD值与静态偏置差异过小

		uint16_t	gBlock:1;
		uint16_t	gLosStep:1;            //电机失步
		uint16_t	gOpenLoopErr:1;
    }bits;

}MC_Error_u;  
typedef struct 
{
	
	ADC_Value_TypeDef Board_ADC_DATA;
	MC_Error_u MC_error;
	uint8_t current_comp_out; 
	uint32_t lock_timer_time;
	uint32_t  time_count_60_degree;
	uint8_t bldc_step_current;
	uint8_t bldc_bemf_detected;
	uint32_t swith_timer_count;
//	uint8_t zero_cross_lock;
	uint32_t crossed_zero_timer_count;
	
	uint32_t pwm_actual_runing_data;
//	uint8_t last_step_index;
	uint8_t mc_dir;
	uint8_t mc_moving;	
	uint32_t  aglin_count;
//	uint16_t tim13_h_count;
	uint8_t ipd_star_flag;
	uint8_t  motor_en;
	uint32_t block_recover_count;
	uint32_t block_count;
	uint32_t block_count1;
	uint32_t block_count2;
	uint32_t block_count3;
	uint32_t block_count4;
  uint32_t block_count_runing;
	uint16_t CurrentOffset_ad_data;
	
	uint16_t drag_step_count;
  uint16_t drag_step_count_compare;
	
	uint8_t motor_bemf_closed_count_set;
	uint8_t motor_bemf_closed_count;
	
//	uint8_t force_shihf_phase_flag; 
	uint8_t motor_bemf_runing;
	uint32_t Run_SectorCnt; 
	
	uint8_t  motion_en;
	uint8_t motion_processed_result;
	
	
	
	
	
	
 
	
	
	
	
		
		
		
		uint16_t Ibus_Peak_Ocp_Once_AdcValue;
	
	
} Mc_core_list;
typedef struct
{
	uint16_t shift_closedloop_angle;
	uint16_t mask_closedloop_angle;
	uint16_t high_speed_shift_closedloop_angle;
	uint16_t high_speed_mask_closedloop_angle;
	uint16_t low_speed_shift_closedloop_angle;
	uint16_t low_speed_mask_closedloop_angle;
	uint32_t mask_openloop_timer_count;
	uint32_t shift_openloop_timer_count;
	uint32_t mask_min_timer_count;
	uint32_t mask_max_timer_count;
	uint32_t shift_min_timer_count;
	uint32_t shift_max_timer_count;
	uint16_t ipd_idle_time;
	uint16_t ipd_pulse_time;
	uint8_t  lose_step_protect_en;
	uint16_t lose_step_protect_count_set;
	uint8_t  force_shihf_phase_en;
	uint32_t force_shihf_phase_timer_count_set;
	uint16_t force_change_phase_protect_count_set;
	uint8_t  motor_rotate_error_detect_en;
	uint16_t ibus_ipd_ocp_adc_value;
	uint8_t  ibus_peak_ocp_once_en;
	uint16_t motion_check_time_count;
	uint16_t motion_pass_count_set;
	uint32_t max_sector_count_set;
	uint32_t max_sector_time_count_set;
	uint16_t lack_phase_delta;
	uint8_t  i_peak_ad_channel;
} McCore_Param_t;


 

extern Mc_core_list mc_core_list;
void step_control(unsigned char step,unsigned char dir);
void change_phase(void);
u32 read_timer_count(void);
void mc_init(void);
void motor_block_detect(void);
void motion_handle(void);
void ipd_detect(void);
void motor_to_bemf_runing_pre(void);

void mc_core_timer14_isr_handle(void);
void mc_core_adc_isr_handle(void);
void max_sector_handle(void);
void mc_core_comp_isr_handle(void);
void mc_lose_step_init(void);
void mc_core_list_init(const McCore_Param_t *param);
void mc_core_stop_reset(void);
#endif


