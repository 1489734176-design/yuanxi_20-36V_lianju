#include "motor_control.h"
#include "mc_core.h"
#include "drv_pwm.h"
#include "parameter.h"
#include "board.h"
#include "drv_adc.h"
#include "user_control.h"
#include "systick.h"
#include "motor_config.h"
Motor_Control_list motor_control_list;
#if LIMIT_SPEED_EN
PidPI_t Speed_Pid;
#endif
extern u32 PI_sum_i;
extern u32 PI_p;
extern u32 pwm_duty_aim_PI_OUT;
void mc_init(void)
{
	McCore_Param_t p;
	p.shift_closedloop_angle = ShiftTime_ClosedLoop_Angle;
	p.mask_closedloop_angle = MaskTime_ClosedLoop_Angle;
	p.high_speed_shift_closedloop_angle = High_Speed_ShiftTime_ClosedLoop_Angle;
	p.high_speed_mask_closedloop_angle = High_Speed_MaskTime_ClosedLoop_Angle;
	p.low_speed_shift_closedloop_angle = LOW_Speed_ShiftTime_ClosedLoop_Angle;
	p.low_speed_mask_closedloop_angle = LOW_Speed_MaskTime_ClosedLoop_Angle;
	p.mask_openloop_timer_count = MaskTime_OpenLoop_TIMER_COUNT;
	p.shift_openloop_timer_count = ShiftTime_OpenLoop_TIMER_COUNT;
	p.mask_min_timer_count = MaskTime_Min_TIMER_COUNT;
	p.mask_max_timer_count = MaskTime_Max_TIMER_COUNT;
	p.shift_min_timer_count = ShiftTime_Min_TIMER_COUNT;
	p.shift_max_timer_count = ShiftTime_Max_TIMER_COUNT;
	p.ipd_idle_time = MotorStart_IPD_IdleTime;
	p.ipd_pulse_time = MotorStart_IPD_PulseTime;
	p.lose_step_protect_en = LosStep_Protect_En;
	p.lose_step_protect_count_set = LosStep_Protect_CntSet;
	p.force_shihf_phase_en = ForceChangePhase_En;
	p.force_shihf_phase_timer_count_set = ForceChangePhase_CntSet;
	p.force_change_phase_protect_count_set = ForceChangePhaseProtect_TimesSet;
	p.motor_rotate_error_detect_en = MOTOR_ROTATE_ERROR_DETECT_En;
	p.ibus_ipd_ocp_adc_value = IBus_IPD_OCP_AdcValue;
	p.ibus_peak_ocp_once_en = IBus_Peak_OCP_ONCE_EN;
	p.motion_check_time_count = Motion_CheckTime_Count;
	p.motion_pass_count_set = Motion_Pass_CntSet;
	p.max_sector_count_set = SectorTIme_Max_COUNT;
	p.max_sector_time_count_set = SectorTIme_Max_TIMER_COUNT;
	p.lack_phase_delta = (u16)(CURRENT_ADC_PER_A);
	p.i_peak_ad_channel = OP_O_CHANNEL;

	motor_control_list.bldc_state = MOTOR_STOP;
	motor_control_list.hbrake_en = HBrake_En;
	motor_control_list.sbrake_en = SBrake_En;
	motor_control_list.motor_run_count = 0;

	mc_core_list.MC_error.word = 0;
	mc_core_list.lock_timer_time = MaskTime_OpenLoop_TIMER_COUNT;
	mc_core_list.time_count_60_degree = 0;
	mc_core_list.bldc_step_current = 0;
	mc_core_list.bldc_bemf_detected = 0;
	mc_core_list.crossed_zero_timer_count = 0;
	mc_core_list.pwm_actual_runing_data = 0;
	mc_core_list.mc_moving = 0;
	mc_core_list.aglin_count = 0;
	mc_core_list.mc_dir = Tool_Dir_Init;
	mc_core_list.ipd_star_flag = 0;
	mc_core_list.motor_en = 0;
	mc_core_list.block_count = 0;
	mc_core_list.block_count1 = 0;
	mc_core_list.block_count2 = 0;
	mc_core_list.block_count3 = 0;
	mc_core_list.block_count4 = 0;
	mc_core_list.block_count_runing = 0;
	mc_core_list.drag_step_count = 0;
	mc_core_list.drag_step_count_compare = 0;
	mc_core_list.motor_bemf_runing = 0;
	mc_core_list.Run_SectorCnt = 0;
	mc_core_list.motion_en = Motion_En;
	mc_core_list.motion_processed_result = 0;
	mc_core_list.Ibus_Peak_Ocp_Once_AdcValue = IBus_Peak_OCP_ONCE_AdcValue;

	mc_core_list_init(&p);
}


void motor_block_detect(void)
{
	u32 block_ipeak_data=0;
	//u32 block_ipeak_data=mc_core_list.Board_ADC_DATA.u16I_Peak;
	
			if(mc_core_list.Board_ADC_DATA.u16I_Peak>mc_core_list.CurrentOffset_ad_data)
		{
			block_ipeak_data=mc_core_list.Board_ADC_DATA.u16I_Peak-mc_core_list.CurrentOffset_ad_data;
			
		}
		else 
		{
			block_ipeak_data=0;
		}
	#if Block_Protect_Current_En
	
		if(motor_control_list.bldc_state==MOTOR_RUN||motor_control_list.bldc_state==MotorStart_Drag_OPENLOOP)
		{
			
					mc_core_list.block_count++;
				  if(mc_core_list.block_count>Block_Protect_MAX_TIME)
					{
							mc_core_list.MC_error.bits.gBlock=1;
							mc_core_list.block_recover_count=0;
					
					}
					#if 1
					if(motor_control_list.motor_run_count>(250*SLOWLOOP_1ms_CNT_LOAD))
					{
						mc_core_list.block_count_runing++;
						if(mc_core_list.block_count_runing>Block_Protect_RUNING_TIME)
						{
								mc_core_list.MC_error.bits.gBlock=1;
								mc_core_list.block_recover_count=0;
						
						}
					
					}
					else 
					{
						mc_core_list.block_count_runing=0;
					}
					#endif
						if(block_ipeak_data>Block_Protect_Current1_AdcValue)
					{
						mc_core_list.block_count1++;
						if(mc_core_list.block_count1>Block_Protect_Current1_TIME)
						{
						
							mc_core_list.MC_error.bits.gBlock=1;
							mc_core_list.block_recover_count=0;
						}
					
					}		


					 if(block_ipeak_data>Block_Protect_Current2_AdcValue)
					{
						mc_core_list.block_count2++;
						if(mc_core_list.block_count2>Block_Protect_Current2_TIME)
						{
						
							mc_core_list.MC_error.bits.gBlock=1;
							mc_core_list.block_recover_count=0;
						}
					
					}

					 if(block_ipeak_data>Block_Protect_Current3_AdcValue)
					{
						mc_core_list.block_count3++;
						if(mc_core_list.block_count3>Block_Protect_Current3_TIME)
						{
						
							mc_core_list.MC_error.bits.gBlock=1;
							mc_core_list.block_recover_count=0;
						}
					
					}	

					 if(block_ipeak_data>Block_Protect_Current4_AdcValue)
					{
						mc_core_list.block_count4++;
						if(mc_core_list.block_count4>Block_Protect_Current4_TIME)
						{
						
							mc_core_list.MC_error.bits.gBlock=1;
							mc_core_list.block_recover_count=0;
						}
					
					}						
			
		}
		else
		{
//				if(mc_core_list.block_count)
//				{
//					mc_core_list.block_count--;
//				}
//				if(mc_core_list.block_count1)
//				{
//					mc_core_list.block_count1--;
//				}
//				if(mc_core_list.block_count2)
//				{
//					mc_core_list.block_count2--;
//				}
//				if(mc_core_list.block_count3)
//				{
//					mc_core_list.block_count3--;
//				}
//				if(mc_core_list.block_count4)
//				{
//					mc_core_list.block_count4--;
//				}
	
//				if((mc_core_list.block_count==0)&&(mc_core_list.block_count1==0)&&(mc_core_list.block_count2)==0 \
//					&&(mc_core_list.block_count3==0)&&(mc_core_list.block_count4==0))
//				{
//					mc_core_list.MC_error.bits.gBlock=0;
//				
//				}	
					mc_core_list.block_count=0;
					mc_core_list.block_count1=0;
					mc_core_list.block_count2=0;
					mc_core_list.block_count3=0;
					mc_core_list.block_count4=0;
					mc_core_list.block_count_runing=0;
					mc_core_list.block_recover_count++;
					if(mc_core_list.block_recover_count>Block_Protect_Recover_TIME)
					{
						mc_core_list.MC_error.bits.gBlock=0;
						mc_core_list.block_recover_count=0;
					}


		}
	
	
	#else
	if(motor_control_list.bldc_state==MOTOR_RUN||motor_control_list.bldc_state==MotorStart_Drag_OPENLOOP)
	{
		mc_core_list.block_count++;
		
		if(mc_core_list.pwm_actual_runing_data<Block_Protect_LowDuty_DATA)
		{
			if(mc_core_list.block_count>Block_Protect_LowDuty_COUNT)
			{
				
					//mos过载处理
//				if(mc_core_list.MC_error.bits.gBlock==0)
//				{
//					user_list.Mos_overload_count++;
//				
//				}
				mc_core_list.block_recover_count=0;
				mc_core_list.MC_error.bits.gBlock=1;
			}
		}
		else
		{			
			if(mc_core_list.block_count>Block_Protect_HighDuty_COUNT)
			{
					//mos过载处理
//				if(mc_core_list.MC_error.bits.gBlock==0)
//				{
//					user_list.Mos_overload_count++;
//				
//				}
				mc_core_list.MC_error.bits.gBlock=1;
			}
		
		}
	}
	else 
	{
		mc_core_list.block_count=0;
				mc_core_list.block_recover_count++;
			if(mc_core_list.block_recover_count>Block_Protect_Recover_TIME)
			{
				mc_core_list.MC_error.bits.gBlock=0;
				mc_core_list.block_recover_count=0;
			}

	
	}
	#endif
}
//void motor_block_detect(void)
//{
//	if(motor_control_list.bldc_state==MOTOR_RUN||motor_control_list.bldc_state==MotorStart_Drag_OPENLOOP)
//	{
//		mc_core_list.block_count++;
//		
//		if(mc_core_list.pwm_actual_runing_data<Block_Protect_LowDuty_DATA)
//		{
//			if(mc_core_list.block_count>Block_Protect_LowDuty_COUNT)
//			{
//				mc_core_list.MC_error.bits.gBlock=1;
//			}
//		}
//		else
//		{			
//			if(mc_core_list.block_count>Block_Protect_HighDuty_COUNT)
//			{
//				mc_core_list.MC_error.bits.gBlock=1;
//			}
//		
//		}
//	}
//	else 
//	{
//		mc_core_list.block_count=0;
//	
//	}
//	
//}

void pwm_set(void)
{
		static u8 pwm_add_count=0;
  	static u8 pwm_sub_count=0;
	
		static u8 pwm_add_fast_count=0;
  	static u8 pwm_sub_fast_count=0;
		u8 limit_current_on=0;

	#if(LIMIT_PEAK_CURRENT_EN)
    uint16_t IBusPeak_Adc_temp=0;
	#endif
	#if(LIMIT_AVG_CURRENT_EN)
			u32 limit_current_temp=0;
		uint16_t IBusAvg_Adc_temp=0;
	#endif
	//u32 block_ipeak_data=mc_core_list.Board_ADC_DATA.u16I_Peak;
		#if(LIMIT_PEAK_CURRENT_EN)
			if(mc_core_list.Board_ADC_DATA.u16I_Peak>mc_core_list.CurrentOffset_ad_data)
		{
			IBusPeak_Adc_temp=mc_core_list.Board_ADC_DATA.u16I_Peak-mc_core_list.CurrentOffset_ad_data;
			IBusPeak_Adc_temp=((((u32)IBusPeak_Adc_temp)*user_list.Vcc_cal_gain)>>15);
		}
		else 
		{
			IBusPeak_Adc_temp=0;
		}
		#endif
		
		#if(LIMIT_AVG_CURRENT_EN)
		if( mc_core_list.Board_ADC_DATA.u16I_SUM_Avg> mc_core_list.CurrentOffset_ad_data)
		{
			IBusAvg_Adc_temp=mc_core_list.Board_ADC_DATA.u16I_SUM_Avg-mc_core_list.CurrentOffset_ad_data;
			IBusAvg_Adc_temp=((((u32)IBusAvg_Adc_temp)*user_list.Vcc_cal_gain)>>15);
		}
		else 
		{
			IBusAvg_Adc_temp=0;
		}
		#endif
		#if(LIMIT_PEAK_CURRENT_EN)
		if(IBusPeak_Adc_temp>LIMIT_PEAK_CURRENT_AdcValue)
		{
		
			limit_current_on=1;
		}
		#endif
	
		#if(LIMIT_AVG_CURRENT_EN)
		if(user_list.flag.bits.gToolDir==Tool_Dir_CCW_Value)
		{
			limit_current_temp=LIMIT_AVG_CURRENT_AdcValue_CCW;
			
//				if(user_list.gears_ccw_level==0)
//				{
//					limit_current_temp=LIMIT_AVG_CURRENT_AdcValue_CCW;
//				}
//				else if(user_list.gears_ccw_level==1)
//				{
//					limit_current_temp=LIMIT_AVG_CURRENT2_AdcValue;
//				}
//					else if(user_list.gears_ccw_level==2)
//				{
//					limit_current_temp=LIMIT_AVG_CURRENT1_AdcValue;
//				}
//				else
//				{
//					limit_current_temp=LIMIT_AVG_CURRENT_AdcValue_CCW;
//				}	
		}
		else 
		{
				if(user_list.gears_level==0)
				{
					limit_current_temp=LIMIT_AVG_CURRENT1_AdcValue;
				}
				else if(user_list.gears_level==1)
				{
					limit_current_temp=LIMIT_AVG_CURRENT2_AdcValue;
				}
				else
				{
					limit_current_temp=LIMIT_AVG_CURRENT3_AdcValue;
				}				
			
		}
			if(IBusAvg_Adc_temp>limit_current_temp)
		{
		
			limit_current_on=1;
		}
		#endif
		if((motor_control_list.pwm_duty_virtual<motor_control_list.pwm_duty_aim))
		{
					pwm_add_count++;
					if(pwm_add_count>=PWM_ADD_GAP)
					{
						
						pwm_add_count=0;
						motor_control_list.pwm_duty_virtual+=PWM_ADD_VALUE;
						if(motor_control_list.pwm_duty_virtual>motor_control_list.pwm_duty_aim)
						{
							motor_control_list.pwm_duty_virtual=motor_control_list.pwm_duty_aim;
						
						}
						
					}
								
		}
		else if(motor_control_list.pwm_duty_virtual>motor_control_list.pwm_duty_aim)
		{
			pwm_sub_count++;
			if(pwm_sub_count>=PWM_SUB_GAP)
			{
				pwm_sub_count=0;
				if(motor_control_list.pwm_duty_virtual>PWM_SUB_VALUE)
				{
					motor_control_list.pwm_duty_virtual-=PWM_SUB_VALUE;
				}
				if(motor_control_list.pwm_duty_virtual<motor_control_list.pwm_duty_aim)
				{
					motor_control_list.pwm_duty_virtual=motor_control_list.pwm_duty_aim;
				
				}
			}
		}
		
		
		if(motor_control_list.pwm_duty_virtual<LIMIT_MIN_PWN_DUTY)
		{
			motor_control_list.pwm_duty_virtual=LIMIT_MIN_PWN_DUTY;
		}
		if(motor_control_list.pwm_duty_virtual>LIMIT_MAX_PWN_DUTY)
		{
			motor_control_list.pwm_duty_virtual=LIMIT_MAX_PWN_DUTY;
		}
		
	
		
		 #if LIMIT_SPEED_EN
				if((mc_core_list.pwm_actual_runing_data<motor_control_list.pwm_duty_aim2)&&(limit_current_on==0))
		{
		
					pwm_add_fast_count++;
				if(pwm_add_fast_count>=PWM_ADD_FAST_GAP)
				{
					pwm_add_fast_count=0;
					mc_core_list.pwm_actual_runing_data+=PWM_ADD_FAST_VALUE;
					if(mc_core_list.pwm_actual_runing_data>motor_control_list.pwm_duty_aim2)
					{
						mc_core_list.pwm_actual_runing_data=motor_control_list.pwm_duty_aim2;
					
					}
				}
			
			

		}
		if(mc_core_list.pwm_actual_runing_data>motor_control_list.pwm_duty_aim2||limit_current_on)
		{
			
					pwm_sub_fast_count++;
				if(pwm_sub_fast_count>=PWM_SUB_FAST_GAP)
				{
					pwm_sub_fast_count=0;
					if(mc_core_list.pwm_actual_runing_data>PWM_SUB_FAST_VALUE)
					{
						mc_core_list.pwm_actual_runing_data-=PWM_SUB_FAST_VALUE;
					}
					if((mc_core_list.pwm_actual_runing_data<motor_control_list.pwm_duty_aim2)&&(limit_current_on==0))
					{
						mc_core_list.pwm_actual_runing_data=motor_control_list.pwm_duty_aim2;
					
					}
				}
		}	
		#else 
			
		if((mc_core_list.pwm_actual_runing_data<motor_control_list.pwm_duty_virtual)&&(limit_current_on==0))
		{
		
					pwm_add_fast_count++;
				if(pwm_add_fast_count>=PWM_ADD_FAST_GAP)
				{
					pwm_add_fast_count=0;
					mc_core_list.pwm_actual_runing_data+=PWM_ADD_FAST_VALUE;
					if(mc_core_list.pwm_actual_runing_data>motor_control_list.pwm_duty_virtual)
					{
						mc_core_list.pwm_actual_runing_data=motor_control_list.pwm_duty_virtual;
					
					}
				}
			
			

		}
		if(mc_core_list.pwm_actual_runing_data>motor_control_list.pwm_duty_virtual||limit_current_on)
		{
			
					pwm_sub_fast_count++;
				if(pwm_sub_fast_count>=PWM_SUB_FAST_GAP)
				{
					pwm_sub_fast_count=0;
					if(mc_core_list.pwm_actual_runing_data>PWM_SUB_FAST_VALUE)
					{
						mc_core_list.pwm_actual_runing_data-=PWM_SUB_FAST_VALUE;
					}
					if((mc_core_list.pwm_actual_runing_data<motor_control_list.pwm_duty_virtual)&&(limit_current_on==0))
					{
						mc_core_list.pwm_actual_runing_data=motor_control_list.pwm_duty_virtual;
					
					}
				}
		}		
		#endif
//		if(limit_current_on)
//		{
//		GPIO_Toggle(GPIOA,GPIO_Pin_4);
//		}
		#if(RESTAR_EN&&Wrench_EN)
		if(user_list.Load_exist||(user_list.flag.bits.gToolDir==Tool_Dir_CCW_Value&&user_list.flag.bits.gAutoStopEn))
		{
			if(mc_core_list.pwm_actual_runing_data<RESTAR_MIN_PWM_LIMIT_DUTY)
			{
			
				mc_core_list.pwm_actual_runing_data=RESTAR_MIN_PWM_LIMIT_DUTY;
			}
		}
		#endif
		if(mc_core_list.pwm_actual_runing_data<LIMIT_MIN_PWN_DUTY)
		{
			mc_core_list.pwm_actual_runing_data=LIMIT_MIN_PWN_DUTY;
		}
		if(mc_core_list.pwm_actual_runing_data>LIMIT_MAX_PWN_DUTY)
		{
			mc_core_list.pwm_actual_runing_data=LIMIT_MAX_PWN_DUTY;
		}
		 PwmDutyUpdate(mc_core_list.pwm_actual_runing_data);


}

void CurrentOffsetCalibration(void)
{
		u16 t_ccr4_temp;
		u8 i=0;
		u32 ibus_adc_sum=0;
		Stop_Motor();
			TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);	// 关闭TIM1更新中断
	 	systick_delay_us(500);
				t_ccr4_temp=TIM1->CCR4;
			TIM1->CCR4 = 65535;
		  	systick_delay_us(2000);
			while(ADC1->ADSTA&0x00000004)
			{;}
		ADC_ExternalTrigConvCmd(ADC1, DISABLE);			// 关闭ADC外部触发转换
		systick_delay_us(500);
		ADC_Cmd(ADC1, DISABLE);
		ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);		// 关闭ADC转换中断
		TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);	// 关闭TIM1更新中断
		
    ADC_AnyChannelNumCfg(ADC1, 0);
    ADC_AnyChannelSelect(ADC1, 0, OP_O_CHANNEL);	
    ADC_AnyChannelCmd(ADC1, ENABLE);
	
	
	
		ADC_ExternalTrigConvCmd(ADC1, DISABLE);			// 关闭ADC外部触发转换
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);					// 清除ADC转换标志位
	  ADC_SampleTimeConfig(ADC1,0, ADC_SampleTime_3_5);
		ADC_Cmd(ADC1, ENABLE);
		systick_delay_us(500);
	
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
		ibus_adc_sum=0;
		for(i=0;i<16;i++)
		{
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
			while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
			ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
			ibus_adc_sum += (uint16_t)GET_ADC_VALUE(OP_O_CHANNEL);
		}
		mc_core_list.CurrentOffset_ad_data=ibus_adc_sum>>4;
		ADC_Cmd(ADC1, DISABLE);
		systick_delay_us(100);
		Board_ADC_Init();
		TIM1->CCR4=t_ccr4_temp;
		motor_control_list.u8Calibration_OK_flag=1;
}

void mos_check_current(u8 mose_check_inedx)
{

			u8 i=0;
			u16 mos_check_ad=0;
			switch(mose_check_inedx)
			{
				case 0:
						UL_HOffLOn();
					break;
				case 1:
						VL_HOffLOn();
					break;
				case 2:
						WL_HOffLOn();
					break;
				case 3:
					UH_HOnLOff();
					break;
				case 4:
					VH_HOnLOff();
					break;
				case 5:
					WH_HOnLOff();
					break;
				default :
					break;				
			
			}
			for(i=0;i<20;i++)
		{
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
			while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
			ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
			mos_check_ad= (uint16_t)GET_ADC_VALUE(OP_O_CHANNEL);
			if(mos_check_ad>mc_core_list.CurrentOffset_ad_data)
			{
			
			mos_check_ad=mos_check_ad-mc_core_list.CurrentOffset_ad_data;
			}
			else 
			{
			
			mos_check_ad=0;
			}
			if(mos_check_ad>MOS_SHORT_CURRENT_AdcValue||	machine_error_hold.userErr_hold.bits.gIBusAcmpOCP==1)
			{
				Stop_Motor();
				machine_error_hold.userErr_hold.bits.gIBusAcmpOCP=0;
				if(mose_check_inedx<3)
				{
					user_list.userErr.bits.gMosBridgeH=1;
				}
				else 
				{
					user_list.userErr.bits.gMosBridgeL=1;
				}
				
				break;
			}
		}
		
		Stop_Motor();
}

void mose_check(void)
{
		u8 i=0;
		u16 t_ccr4_temp;
		Stop_Motor();
			TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);	// 关闭TIM1更新中断
	 	systick_delay_us(500);
			t_ccr4_temp=TIM1->CCR4;
			TIM1->CCR4 = 65535;
		  	systick_delay_us(2000);
			while(ADC1->ADSTA&0x00000004)
			{;}
		ADC_ExternalTrigConvCmd(ADC1, DISABLE);			// 关闭ADC外部触发转换
		systick_delay_us(500);
		ADC_Cmd(ADC1, DISABLE);
		ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);		// 关闭ADC转换中断
		TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);	// 关闭TIM1更新中断
		ADC_ExternalTrigConvCmd(ADC1, DISABLE);			// 关闭ADC外部触发转换
    ADC_AnyChannelNumCfg(ADC1, 0);
    ADC_AnyChannelSelect(ADC1, 0, OP_O_CHANNEL);	
	
		
    ADC_AnyChannelCmd(ADC1, ENABLE);
	  ADC_Cmd(ADC1, ENABLE);
	  systick_delay_us(500);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
   for(i=0;i<6;i++)
	{
		mos_check_current(i);
		
		systick_delay_us(800);
	}
	
//		Systick_delay_us(2);

	
	
//		Systick_delay_us(2);



		ADC_Cmd(ADC1, DISABLE);
			systick_delay_us(500);
			Board_ADC_Init();

		 ADC_ClearFlag(ADC1, ADC_FLAG_EOC);					// 清除ADC转换标志位
			TIM1->CCR4=t_ccr4_temp;
//		 ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);		// 
//		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);	// 关闭TIM1更新中断

}

void start_mc_init(void)
{
//	mc_core_list.last_step_index=255;
	mc_core_list.aglin_count=0;

}
void align_control(void)
{
		mc_core_list.bldc_step_current=0;
		step_control(mc_core_list.bldc_step_current,mc_core_list.mc_dir);

}

void drag_control_init(void)
{
	
		mc_core_list.drag_step_count=600;
		mc_core_list.drag_step_count_compare=500;

}
u8 drag_control(void)
{
//	u16 pwm_ad_data;
	u8 res=0;
		mc_core_list.drag_step_count++;
		if(mc_core_list.drag_step_count>(mc_core_list.drag_step_count_compare))
		{
				if(mc_core_list.drag_step_count_compare>80)
				{
					res=0;
				mc_core_list.drag_step_count_compare=mc_core_list.drag_step_count_compare-10;
				}
				else 
				{
					res=1;
				}
				 change_phase();
			mc_core_list.crossed_zero_timer_count=read_timer_count();
//			mc_core_list.crossed_zero_timer_count=TIM13->CNT;
//			mc_core_list.swith_timer_count=mc_core_list.swith_timer_count+(((u32)mc_core_list.tim13_h_count)<<16);
		mc_core_list.drag_step_count=0;
		}

	

		return res;


}



void MC_Machine_State(void)
{
	static u16 wait_break_count=0;
	static u16 hbreak_count=0;
	static u16 sbreak_count=0;
	static u16 break2stop_count=0;
	static u8 motionless_count=0;

	switch (motor_control_list.bldc_state)
	{
		case MOTOR_STOP :
			Stop_Motor();
		  PI_sum_i=0;
			PI_p=0;
	    pwm_duty_aim_PI_OUT=0;
			
		
		mc_core_stop_reset();
	  	mc_core_list.pwm_actual_runing_data=0;
			motor_control_list.pwm_duty_virtual=0;
		motor_control_list.motor_run_count=0;
			if(mc_core_list.motor_en==1&&machine_error_hold.userErr_hold.word==0&&machine_error_hold.MC_error_hold.word==0)
			{
//				start_mc_init();
//				mc_core_list.ipd_star_flag=1;
				motor_control_list.bldc_state=MOTOR_MOTION;
				mc_core_list.mc_moving=0;
				mc_core_list.motion_processed_result=0;
				motionless_count=0;
//				start_mc_init();
//				mc_core_list.ipd_star_flag=1;
//				mc_core_list.bldc_state=Motor_Align;
			}
			break;
		case MOTOR_MOTION :
					
				if(mc_core_list.motor_en==0||machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
				{
						motor_control_list.bldc_state=MOTOR_STOP;
						mc_core_list.mc_moving=0;	
				}
				if(mc_core_list.Board_ADC_DATA.u16BemfU>Motion_ADC_THRESHOLD||mc_core_list.Board_ADC_DATA.u16BemfV>Motion_ADC_THRESHOLD||mc_core_list.Board_ADC_DATA.u16BemfW>Motion_ADC_THRESHOLD)
				{
					motion_handle();
					if(mc_core_list.motion_processed_result==1)
					{
						motor_control_list.bldc_state=MOTOR_READY;//滑行检测失败
					
					}
					else if(mc_core_list.motion_processed_result==2)
					{
						#if(0)
						mc_core_list.mc_moving=0;	//检测到电机转动直接运行
						mc_core_list.motor_bemf_runing=1;
						motor_control_list.bldc_state=MOTOR_RUN;
						#else
						
						if(mc_core_list.motion_en==0) //检测到电机转动,p判断是否滑行切入使能，不使能则刹车一下，进行IPD
						{
							motor_control_list.bldc_state=MOTOR_READY;
						}
						else 
						{
							mc_core_list.mc_moving=0;	
							mc_core_list.motor_bemf_runing=1;
							motor_control_list.bldc_state=MOTOR_RUN;
						}
						#endif
					
					}
						
						motionless_count=0;
				}
				else
				{
					motionless_count++;
					if(motionless_count>10)
					{
						if(mc_core_list.mc_moving)//滑行检测失败
						{
						  motor_control_list.bldc_state=MOTOR_READY;
							hbreak_count=0;
						}
						else 
						{
						motor_control_list.bldc_state=MOTOR_READY;
							hbreak_count=0;
//						mc_core_list.bldc_bemf_detected=0;
//						motor_control_list.bldc_state=MOTOR_POSITION;
//						mc_core_list.ipd_star_flag=1;
						}
					}
				
				}
			break;
		case MOTOR_READY:
				if(mc_core_list.mc_moving)
				{
					hbreak_count=0;
					mc_core_list.mc_moving=0;
				
				}
				if(mc_core_list.motor_en==0||machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
				{
					motor_control_list.bldc_state=MOTOR_STOP;
				}
				Brake_Motor();
				hbreak_count++;
				if(hbreak_count>Motion_Fail_BrakeTime_Count)
				{
					mc_core_list.bldc_bemf_detected=0;
					motor_control_list.bldc_state=MOTOR_POSITION;
						mc_core_list.ipd_star_flag=1;
				}
				
				break;			
				
		case Motor_Align :
			if(mc_core_list.motor_en==0||machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
			{
				if(machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
				{
					motor_control_list.bldc_state=MOTOR_STOP;
				}
				else if(motor_control_list.sbrake_en)
				{
					motor_control_list.bldc_state=MOTOR_SBRAKE;
					sbreak_count=0;
				}
				else if(motor_control_list.hbrake_en)
				{
					motor_control_list.bldc_state=MOTOR_HBRAKE;
					hbreak_count=0;
				}
				else 
				{
					motor_control_list.bldc_state=MOTOR_STOP;
				}
			}
			
			if(mc_core_list.aglin_count<200)
			{
				mc_core_list.aglin_count++;
			}
			else 
			{
				drag_control_init();
				mc_core_list.bldc_step_current++;
				if(mc_core_list.bldc_step_current>=6)
				{
					mc_core_list.bldc_step_current=0;
					
				}
//				mc_core_list.bldc_step_current++;
//				if(mc_core_list.bldc_step_current>=6)
//				{
//					mc_core_list.bldc_step_current=0;
//					
//				}
				TIM_Cmd(TIM14, DISABLE);	
				TIM_ClearFlag(TIM14, TIM_FLAG_Update);
				mc_core_list.bldc_bemf_detected=0;
				mc_core_list.motor_bemf_runing=1;
				motor_control_list.bldc_state=MotorStart_Drag_OPENLOOP;
				mc_core_list.lock_timer_time=12000;
			
			}
			 align_control();
			break;
			
			case MOTOR_POSITION:
//					if(mc_core_list.motor_en==0)
//					{
//							motor_control_list.bldc_state=MOTOR_STOP;
//							mc_core_list.mc_moving=0;	
//					}
				

				if(mc_core_list.motor_en==0||machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
				{
					mc_core_list.motor_bemf_runing=0;
					if(machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
					{
						motor_control_list.bldc_state=MOTOR_STOP;
					}
					else if(motor_control_list.sbrake_en)
					{
						motor_control_list.bldc_state=MOTOR_SBRAKE;
						sbreak_count=0;
					}
					else if(motor_control_list.hbrake_en)
					{
						motor_control_list.bldc_state=MOTOR_HBRAKE;
						hbreak_count=0;
					}
					else 
					{
						motor_control_list.bldc_state=MOTOR_STOP;
					}
				}
				
				if(mc_core_list.ipd_star_flag==0) //等待IPD结束
				{
//					drag_control_init();

					motor_to_bemf_runing_pre();	
					motor_control_list.bldc_state=MOTOR_RUN;
			  }
			break;
		case MotorStart_Drag_OPENLOOP :
				pwm_set();
			if(mc_core_list.motor_en==0||machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
			{
				if(machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
				{
					motor_control_list.bldc_state=MOTOR_STOP;
				}
				else if(motor_control_list.sbrake_en)
				{
					motor_control_list.bldc_state=MOTOR_SBRAKE;
					sbreak_count=0;
				}
				else if(motor_control_list.hbrake_en)
				{
					motor_control_list.bldc_state=MOTOR_HBRAKE;
					hbreak_count=0;
				}
				else 
				{
					motor_control_list.bldc_state=MOTOR_STOP;
				}
			}
			if(drag_control())
			{
				mc_core_list.motor_bemf_runing=1;
				motor_control_list.bldc_state=MOTOR_RUN;
			
			}
//			if(mc_core_list.bldc_bemf_detected==1)
//			{
//			motor_control_list.bldc_state=MOTOR_RUN;
//			}
		break;
			
			

		case MOTOR_RUN :
			motor_control_list.motor_run_last_dir=mc_core_list.mc_dir;
				pwm_set();
			if(motor_control_list.motor_run_count<0xffffffff)
			{
			motor_control_list.motor_run_count++;
			}
			if(mc_core_list.motor_en==0||machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
			{
				mc_core_list.motor_bemf_runing=0;
				if(machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
				{
					#if(RESTAR_EN)
					if(machine_error_hold.MC_error_hold.word&0xfffb)
					{
						motor_control_list.bldc_state=MOTOR_STOP;
					}
					else
					{
						if(user_list.Load_exist<30000)
						{
							user_list.Load_exist++;
						}
						if(user_list.Load_exist<RESTAR_COUNT)
						{
						
							mc_core_list.MC_error.bits.gBlock=0;
							machine_error_hold.MC_error_hold.bits.gBlock=0;
							mc_core_list.block_count=0;
							mc_core_list.block_count1=0;
							mc_core_list.block_count2=0;
							mc_core_list.block_count3=0;
							mc_core_list.block_count4=0;
							mc_core_list.block_count_runing=0;
							motor_control_list.bldc_state=MOTOR_STOP;
						}
						else 
						{
						
							motor_control_list.bldc_state=MOTOR_STOP;
						}
					
					}
						
					#else
					motor_control_list.bldc_state=MOTOR_STOP;
					#endif
				}
				else if(motor_control_list.sbrake_en)
				{
					#if BREAKE_WAIT_EN
					motor_control_list.bldc_state=MOTOR_BRAKE_WAIT;
					wait_break_count=0;
					#else
					motor_control_list.bldc_state=MOTOR_SBRAKE;
					
					#endif
					sbreak_count=0;
				}
				else if(motor_control_list.hbrake_en)
				{
					#if BREAKE_WAIT_EN
					motor_control_list.bldc_state=MOTOR_BRAKE_WAIT;
					wait_break_count=0;
					#else
					motor_control_list.bldc_state=MOTOR_HBRAKE;
					#endif
					hbreak_count=0;
				}
				else 
				{
					motor_control_list.bldc_state=MOTOR_STOP;
				}
			}
			break;
		case MOTOR_BRAKE2STOP:
			Stop_Motor();
			break2stop_count++;
			if(break2stop_count>160)//10ms
			{
			   	motor_control_list.bldc_state=MOTOR_STOP;
			}
			
			break;
			case MOTOR_BRAKE_WAIT:
			mc_core_list.pwm_actual_runing_data=0;
			motor_control_list.pwm_duty_virtual=0;
			Stop_Motor();
			wait_break_count++;
			if(machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
			{
				motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			}
				else if(mc_core_list.motion_en&&mc_core_list.motor_en&&(motor_control_list.motor_run_last_dir==mc_core_list.mc_dir))
			{
			
		  	motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			}
			else if(wait_break_count>=WAIT_Brake_Time_MS_CNT)
			{
						 if(motor_control_list.sbrake_en)
						{
			
							motor_control_list.bldc_state=MOTOR_SBRAKE;
							
							sbreak_count=0;
						}
						else if(motor_control_list.hbrake_en)
						{
							motor_control_list.bldc_state=MOTOR_HBRAKE;
							hbreak_count=0;
						}
						else 
						{
							motor_control_list.bldc_state=MOTOR_STOP;
						}
			
			}
			
						break;
		case MOTOR_HBRAKE:
			mc_core_list.pwm_actual_runing_data=0;
			motor_control_list.pwm_duty_virtual=0;
			Brake_Motor();
			hbreak_count++;
			if(machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
			{
				motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			}
			else if(mc_core_list.motion_en&&mc_core_list.motor_en&&(motor_control_list.motor_run_last_dir==mc_core_list.mc_dir))
			{
			
		  	motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			}
				else if(hbreak_count>HBrake_MIN_Time_MS_CNT&&mc_core_list.motor_en&&mc_core_list.motion_en)
			{
				motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			
			}
			else if(hbreak_count>HBrake_Time_MS_CNT||machine_error_hold.userErr_hold.bits.gIBusAcmpOCP)
			{
				motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			
			}

//				Stop_Motor();
			
		break;
		case MOTOR_SBRAKE:
			if(sbreak_count==0)
			{
				motor_control_list.sbreak_pwm_duty=Sbrake_Duty_Min;
			}
			sbreak_count++;
			PwmDutyUpdate(motor_control_list.sbreak_pwm_duty);
			
			motor_control_list.sbreak_pwm_duty+=Sbrake_Duty_Add;
			if(motor_control_list.sbreak_pwm_duty>Sbrake_Duty_Max)
			{
				motor_control_list.sbreak_pwm_duty=Sbrake_Duty_Max;
			}
			UVWL_HOffLPwm();	
			if(machine_error_hold.userErr_hold.word!=0||machine_error_hold.MC_error_hold.word!=0)
			{
				motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			}
			else if(mc_core_list.motion_en&&mc_core_list.motor_en&&(motor_control_list.motor_run_last_dir==mc_core_list.mc_dir))
			{
			
		  	motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			}

			else if(mc_core_list.motion_en&&mc_core_list.motor_en&&sbreak_count>SBrake_MIN_Time_MS_CNT)
			{
				if(motor_control_list.hbrake_en)
				{
					motor_control_list.bldc_state=MOTOR_HBRAKE;
					hbreak_count=0;
				}
				else 
				{
					motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
					break2stop_count=0;
					Stop_Motor();
				}
			
			}
			else if(machine_error_hold.userErr_hold.bits.gIBusAcmpOCP)
			{
				motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
				break2stop_count=0;
				Stop_Motor();
			
			}
			else if(sbreak_count>Sbrake_Time_MS)
			{
			 if(motor_control_list.hbrake_en)
				{
					motor_control_list.bldc_state=MOTOR_HBRAKE;
					hbreak_count=0;
				}
				else 
				{
					motor_control_list.bldc_state=MOTOR_BRAKE2STOP;
					break2stop_count=0;
					Stop_Motor();
				}
				
			}
			
		
//			motor_control_list.bldc_state=MOTOR_STOP;
//			Stop_Motor();
			break;
		default:
			
		 break;
	}

}

void phase_time_filter(void)
{
	motor_control_list.time_count_60_degree_filter=((mc_core_list.time_count_60_degree*9830)>>15)+((motor_control_list.time_count_60_degree_filter*22938)>>15);

}
void get_speed(void)
{
	if(motor_control_list.bldc_state==MOTOR_RUN&&mc_core_list.bldc_bemf_detected==1)
	{
	phase_time_filter();
	motor_control_list.motor_speed=(u32)(75000000.0f/((float)motor_control_list.time_count_60_degree_filter));
	}
	else 
	{
	
		motor_control_list.time_count_60_degree_filter=0;
		motor_control_list.motor_speed=0;
	}

}
#if LIMIT_SPEED_EN
void  Speed_PI_init(void)
{
	Speed_Pid.ki=(u32)(32768*0.002f);
	Speed_Pid.kp2=(u32)(32768*2.0f);
	Speed_Pid.kp=(u32)(32768*0.002f);
	Speed_Pid.refValue=48000;
	Speed_Pid.Limit_max_data=PWM_PRIOD_LOAD;
	Speed_Pid.Limit_min_data=(PWM_PRIOD_LOAD*0.15f);

	

	
}

void  Speed_PI_reset(void)
{

	  Speed_Pid.error = 0;
    Speed_Pid.error1 = 0;
    Speed_Pid.error2 = 0;
    Speed_Pid.output = 0;       
    Speed_Pid.pTemp = 0;
    Speed_Pid.iTemp = 0;
    Speed_Pid.deltaUk = 0; //deltaUk = Uk - Uk_1;  Uk为位置式PI输出
    Speed_Pid.deltaOut = 0;
    Speed_Pid.rpmSetPwmDuty = 0;

	Speed_Pid.errorIntegral= 0;
	Speed_Pid.KpDuty = 0;
	Speed_Pid.KiDuty = 0;
	Speed_Pid.OutDuty= 0;
}
int32_t MC_PI_Handler( PidPI_t *pi,int32_t errorBack)
{

    pi->error = (pi->refValue - errorBack)>>0;
//   	if(pi->error<0)
//		{
//			pi->error=((pi->error*pi->kp2)>>15);
//		} 
    if(pi->error > 32768)
    {
		pi->error = 32768;
    }
    else if(pi->error <(- 32768))
    {
			
		pi->error = -32768;
    }

    
	pi->pTemp = pi->kp*(pi->error-pi->error1);

	
	pi->iTemp = 0;

		if(pi->OutDuty >=pi->Limit_max_data)
    {
			if( pi->error < 0)
			pi->iTemp = pi->ki*(pi->error);
    }
    else    if(pi->OutDuty <= pi->Limit_min_data)
    {
			if( pi->error > 0)
			pi->iTemp = pi->ki*(pi->error);
    }
    else
    {
			pi->iTemp = pi->ki*(pi->error);
    }


	pi->output += (pi->pTemp + pi->iTemp);

	if(pi->output > (pi->Limit_max_data<<15))
    {
		pi->output = (pi->Limit_max_data<<15);
    }
    else    if(pi->output < (pi->Limit_min_data<<15))
    {
		pi->output = ((pi->Limit_min_data<<15));
    }
    
	pi->OutDuty = pi->output>>15;

    if(pi->OutDuty>pi->Limit_max_data)
    {
		pi->OutDuty = pi->Limit_max_data;
    }
    else    if(pi->OutDuty < pi->Limit_min_data)
    {
		pi->OutDuty = pi->Limit_min_data;
    }


    pi->rpmSetPwmDuty = pi->OutDuty;
 
	
	pi->error2 = pi->error1;
	pi->error1 = pi->error;

    return pi->rpmSetPwmDuty;
}

#endif
