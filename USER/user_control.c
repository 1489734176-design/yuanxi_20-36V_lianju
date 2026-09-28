#include "user_control.h"
#include "parameter.h"
#include "board.h"
#include "motor_control.h"
#include "drv_adc.h"
#include "flash_data_save.h"


#include "systick.h"
#include "motor_config.h"
#include "drv_pwm.h"
User_list_Struct user_list;

Machine_ERROR_HOLD machine_error_hold;

	u32 VERF_adc_data_avg=0;
	uint16_t CalibrationData = 0;
void Vcc_5V_CAL(void)
{
		u32 CalibrationData_temp;
	  ADC_InitTypeDef ADC_InitStruct;
	  CalibrationData = *(uint16_t *)(0x1FFFF7E0);
	
		CalibrationData_temp=(((u32)CalibrationData)*21626)>>15;
		if(CalibrationData_temp<904||CalibrationData_temp>1061)
		{
			user_list.Vcc_cal_gain=32768;
		}
		else 
		{
				Stop_Motor();


				RCC_APB1PeriphClockCmd(RCC_APB1Periph_ADC1, ENABLE);
				ADC_ExternalTrigConvCmd(ADC1, DISABLE);			// 关闭ADC外部触发转换
				systick_delay_us(200);
				ADC_Cmd(ADC1, DISABLE);
				systick_delay_us(200);
				ADC_DeInit(ADC1);
				systick_delay_us(200);
				ADC_Cmd(ADC1, DISABLE);
				systick_delay_us(200);
						ADC_AnyChannelCmd(ADC1, DISABLE);
				ADC_StructInit(&ADC_InitStruct);
				ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
				ADC_InitStruct.ADC_Prescaler  = ADC_Prescaler_16;
				ADC_InitStruct.ADC_Mode       = ADC_Mode_Imm;
				ADC_InitStruct.ADC_DataAlign  = ADC_DataAlign_Right;
				ADC_Init(ADC1, &ADC_InitStruct);

				ADC_SampleTimeConfig(ADC1, ADC_Channel_VoltReference, ADC_SampleTime_240_5);

				ADC_ChannelCmd(ADC1, ADC_Channel_VoltReference, ENABLE);

				ADC_TempSensorCmd(ENABLE);

				ADC_Cmd(ADC1, ENABLE);
				systick_delay_us(200);
						ADC_SoftwareStartConvCmd(ADC1, ENABLE);
						while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
						{
						}

						ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
								systick_delay_us(200);
						ADC_SoftwareStartConvCmd(ADC1, ENABLE);
						while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
						{
						}

						ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

						VERF_adc_data_avg = ADC_GetConversionValue(ADC1);
						user_list.Vcc_cal_gain=(u32)(32768.0f*((float)CalibrationData_temp)/((float)VERF_adc_data_avg));
						
				ADC_Cmd(ADC1, DISABLE);
				systick_delay_us(200);
				ADC_DeInit(ADC1);
				systick_delay_us(200);
				Board_ADC_Init();
		}
	
}

void user_para_init(void)
{
	user_list.userErr.word=0;
	user_list.user_state=TOOL_STOP;
	user_list.Key_Stop_Lock=0;
		user_list.flag.bits.gToolDir= Tool_Dir_Init;

}
#if 0
void user_oc_handle(void)
{
		static u16 IBusAvgOCP1_count=0;
	static u16 IBusAvgOCP2_count=0;
	static u16 IBusAvgOCP3_count=0;
	static u16 IBusPeakOCP1_count=0;
		static u16 IBusAvg_rec_count=0;
		#if (!Multistage_Integral_Peak_OCP_EN)
	static u16 IBusPeakOCP2_count=0;
	static u16 IBusPeakOCP3_count=0;
	static u16 IBusPeakREC_count=0;
	#endif
	u16  ibus_temp=0;
	   
		
		if(mc_core_list.Board_ADC_DATA.u16I_SUM_Avg>mc_core_list.CurrentOffset_ad_data)
		{
			ibus_temp=mc_core_list.Board_ADC_DATA.u16I_SUM_Avg-mc_core_list.CurrentOffset_ad_data;
		}
		else 
		{
			ibus_temp=0;
		
		}
			if(mc_core_list.Board_ADC_DATA.u16I_Peak>mc_core_list.CurrentOffset_ad_data)
		{
			user_list.IBusPeak_Adc=mc_core_list.Board_ADC_DATA.u16I_Peak-mc_core_list.CurrentOffset_ad_data;
		}
		else 
		{
			user_list.IBusPeak_Adc=0;
		
		}	
		
		
//		user_list.IBusAvg1_Adc=(u16)((float)mc_core_list.pwm_actual_runing_data *ibus_temp/PWM_PRIOD_LOAD);	
//		user_list.IBusAvg_Adc=(user_list.IBusAvg1_Adc>>3)+user_list.IBusAvg_Adc-(user_list.IBusAvg_Adc>>3);
		user_list.IBusAvg_Adc=ibus_temp;
		//------------------ocp1
		if(user_list.IBusAvg_Adc>IBus_Avg_OCP1_AdcValue)
		{
				
			IBusAvgOCP1_count++;
			if(IBusAvgOCP1_count>IBus_Avg_OCP1_MS_CONUT)
			{
				IBusAvgOCP1_count=IBus_Avg_OCP1_MS_CONUT;
				user_list.userErr.bits.gIBusAvgOCP1=1;
				IBusAvg_rec_count=0;
			}
		}
		else 
		{
			if(IBusAvgOCP1_count>0)
			{
				IBusAvgOCP1_count--;
			}
			else
			{
				user_list.userErr.bits.gIBusAvgOCP1=0;
			
			}
		}
			////------------------ocp2
			if(user_list.IBusAvg_Adc>IBus_Avg_OCP2_AdcValue)
		{
				
			IBusAvgOCP2_count++;
			if(IBusAvgOCP2_count>IBus_Avg_OCP2_MS_CONUT)
			{
				IBusAvgOCP2_count=IBus_Avg_OCP2_MS_CONUT;
				user_list.userErr.bits.gIBusAvgOCP2=1;
				IBusAvg_rec_count=0;
			}
		}
		else 
		{
			if(IBusAvgOCP2_count>0)
			{
				IBusAvgOCP2_count--;
			}
			else
			{
				user_list.userErr.bits.gIBusAvgOCP2=0;
			
			}
			
		}
		
			//------------------ocp3
			if(user_list.IBusAvg_Adc>IBus_Avg_OCP3_AdcValue)
		{
				
			IBusAvgOCP3_count++;
			if(IBusAvgOCP3_count>IBus_Avg_OCP3_MS_CONUT)
			{
				IBusAvgOCP3_count=IBus_Avg_OCP3_MS_CONUT;
				user_list.userErr.bits.gIBusAvgOCP3=1;
				IBusAvg_rec_count=0;
			}
		}
		else 
		{
			if(IBusAvgOCP3_count>0)
			{
				IBusAvgOCP3_count--;
			}
			else
			{
				user_list.userErr.bits.gIBusAvgOCP3=0;
			
			}
			
		}	
		
		if(user_list.IBusAvg_Adc<IBus_Avg_OCP1_AdcValue&&motor_control_list.bldc_state==MOTOR_STOP)
		{
			IBusAvg_rec_count++;
			if(IBusAvg_rec_count>IBus_Avg_REC_MS_CONUT)
			{
				IBusAvg_rec_count=0;
				user_list.userErr.bits.gIBusAvgOCP3=0;
				user_list.userErr.bits.gIBusAvgOCP2=0;
				user_list.userErr.bits.gIBusAvgOCP1=0;
				IBusAvgOCP3_count=0;
				IBusAvgOCP2_count=0;
				IBusAvgOCP1_count=0;
			}
			
		}


#if Multistage_Integral_Peak_OCP_EN
		
			  if(user_list.IBusPeak_Adc > IBus_Smart_Peak_OCP3_AdcValue)
        {

						IBusPeakOCP1_count+=IBus_Smart_OCP3_MS_Operation_Value;
            if(IBusPeakOCP1_count >= 32768 )
            {
               IBusPeakOCP1_count= 32768;
							//mos过载处理
							if(user_list.userErr.bits.gIBusPeakOCP1==0)
							{
//								user_list.Mos_overload_count++;
							
							}
							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
		    else  if(user_list.IBusPeak_Adc > IBus_Smart_Peak_OCP2_AdcValue)
        {

						IBusPeakOCP1_count+=IBus_Smart_OCP2_MS_Operation_Value;
            if(IBusPeakOCP1_count >= 32768 )
            {
               IBusPeakOCP1_count= 32768;
							//mos过载处理
							if(user_list.userErr.bits.gIBusPeakOCP1==0)
							{
//								user_list.Mos_overload_count++;
							
							}
							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
				else if(user_list.IBusPeak_Adc > IBus_Smart_Peak_OCP1_AdcValue)
				{
				
						IBusPeakOCP1_count+=IBus_Smart_OCP1_MS_Operation_Value;
            if(IBusPeakOCP1_count >= 32768 )
            {
               IBusPeakOCP1_count= 32768;
							//mos过载处理
							if(user_list.userErr.bits.gIBusPeakOCP1==0)
							{
//								user_list.Mos_overload_count++;
							
							}
							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
				
				
				}
        else  
					
        {
					
						if(motor_control_list.bldc_state==MOTOR_STOP)
						{
								if(IBusPeakOCP1_count>IBus_Smart_REC_MS_Operation_Value)
							{
									IBusPeakOCP1_count-=IBus_Smart_REC_MS_Operation_Value;
							}
							else
							{
								IBusPeakOCP1_count=0;
									user_list.userErr.bits.gIBusPeakOCP1 = 0;
							}
						}
						else
						{
					
							if(IBusPeakOCP1_count>IBus_Smart_Peak_OCP1_AdcValue)
							{
									IBusPeakOCP1_count-=IBus_Smart_Peak_OCP1_AdcValue;
							}
							else
							{
								IBusPeakOCP1_count=0;
									user_list.userErr.bits.gIBusPeakOCP1 = 0;
							}
						}
        }
		
#else
        if(user_list.IBusPeak_Adc > IBus_Peak_OCP1_AdcValue)
        {
            if(IBusPeakOCP1_count++ >= IBus_Peak_OCP1_MS_CONUT )
            {
               IBusPeakOCP1_count= IBus_Peak_OCP1_MS_CONUT;

							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
        else
        {
            if(IBusPeakOCP1_count)
            {
                IBusPeakOCP1_count--;
            }
            else
            {
//								user_list.userErr.bits.gIBusPeakOCP1 = 0;
            }
        }
				
				
				
				  if(user_list.IBusPeak_Adc > IBus_Peak_OCP2_AdcValue)
        {
            if(IBusPeakOCP2_count++ >= IBus_Peak_OCP2_MS_CONUT )
            {
               IBusPeakOCP2_count= IBus_Peak_OCP2_MS_CONUT;

							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
        else
        {
            if(IBusPeakOCP2_count)
            {
                IBusPeakOCP2_count--;
            }
            else
            {
//								user_list.userErr.bits.gIBusPeakOCP1 = 0;
            }
        }
				
				
					if(user_list.IBusPeak_Adc > IBus_Peak_OCP3_AdcValue)
        {
            if(IBusPeakOCP3_count++ >= IBus_Peak_OCP3_MS_CONUT )
            {
               IBusPeakOCP3_count= IBus_Peak_OCP3_MS_CONUT;

							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
        else
        {
            if(IBusPeakOCP3_count)
            {
                IBusPeakOCP3_count--;
            }
            else
            {
//								user_list.userErr.bits.gIBusPeakOCP1 = 0;
            }
        }
				
				if(user_list.userErr.bits.gIBusPeakOCP1)
				{
					if(user_list.IBusPeak_Adc < IBus_Peak_OCP1_AdcValue)
					{
						IBusPeakREC_count++;
						if(IBusPeakREC_count>IBus_Peak_REC_MS_CONUT)
						{
							user_list.userErr.bits.gIBusPeakOCP1=0;
						}
										
					}
				}
				else 
				{
					IBusPeakREC_count=0;
				
				}
				
#endif		
}

void Volt_Handler_fast(void)
{
				#if(1)
        if((mc_core_list.Board_ADC_DATA.u16Vbus < VBus_Stb_UVP_ADC)) // 0 可以改为电机停止
        {
      
								user_list.userErr.bits.gVBusStbUVP = 1; //静态欠压保护

        }
        else
        {

			      	user_list.userErr.bits.gVBusStbUVP = 0;
        }

				#endif
        if(mc_core_list.Board_ADC_DATA.u16Vbus < VBus_Run_UVP1_ADC)
        {
  
     
                user_list.userErr.bits.gVBusRunUVP1 = 1;
                

        }
        else
        {

							user_list.userErr.bits.gVBusRunUVP1 = 0;

        }
        
       
        if(mc_core_list.Board_ADC_DATA.u16Vbus < VBus_Run_UVP2_ADC)
        {
    
  
              user_list.userErr.bits.gVBusRunUVP2 = 1;
     
        }
        else
        {

					  user_list.userErr.bits.gVBusRunUVP2 = 0;
        }
        if(mc_core_list.Board_ADC_DATA.u16Vbus > VBus_OVP_ADC)
        {
              user_list.userErr.bits.gVBusOVP = 1;
        } 
        else
        {
							user_list.userErr.bits.gVBusOVP = 0;
        }
}
void Volt_Handler(void)
{


	static u16 VBusStbUVP_Cnt=0;//上电只需检测一次
	static u16 VBusRunUVP1_Cnt=0;
	static u16 VBusRunUVP2_Cnt=0;
	static u16 	VBusOVP_Cnt=0;
    #if EN_VOLATAGE_PROTECT    
				user_list.VBus_Vx100=(mc_core_list.Board_ADC_DATA.u16Vbus*VBUS_BASE_X100)>>12;
        if((mc_core_list.Board_ADC_DATA.u16Vbus < VBus_Stb_UVP_ADC)&&(user_list.user_state != TOOL_RUN)) // 0 可以改为电机停止
        {
            if(VBusStbUVP_Cnt++ >= VBus_Stb_UVP_FiltMS_COUNT)
            {
                VBusStbUVP_Cnt= VBus_Stb_UVP_FiltMS_COUNT;

								user_list.userErr.bits.gVBusStbUVP = 1; //静态欠压保护
            }
        }
        else
        {
            if(VBusStbUVP_Cnt)
            {
                VBusStbUVP_Cnt--;
            }
            else
            {
			      	user_list.userErr.bits.gVBusStbUVP = 0;
            }
        }


        if(mc_core_list.Board_ADC_DATA.u16Vbus < VBus_Run_UVP1_ADC)
        {
            if(VBusRunUVP1_Cnt++ >= VBus_Run_UVP1_FiltMS_COUNT)
            {
                VBusRunUVP1_Cnt= VBus_Run_UVP1_FiltMS_COUNT;
     
                user_list.userErr.bits.gVBusRunUVP1 = 1;
                
            }
        }
        else
        {
            if(VBusRunUVP1_Cnt)
            {
                VBusRunUVP1_Cnt--;
            }
			else
            {
							user_list.userErr.bits.gVBusRunUVP1 = 0;
            }
        }
        
       
        if(mc_core_list.Board_ADC_DATA.u16Vbus < VBus_Run_UVP2_ADC)
        {
            if(VBusRunUVP2_Cnt++ >=VBus_Run_UVP2_FiltMS_COUNT)
            {
                VBusRunUVP2_Cnt= VBus_Run_UVP2_FiltMS_COUNT;
  
                user_list.userErr.bits.gVBusRunUVP2 = 1;
            }
        }
        else
        {
            if(VBusRunUVP2_Cnt)
            {
                VBusRunUVP2_Cnt--;
            }
						else
            {
					  	user_list.userErr.bits.gVBusRunUVP2 = 0;
            }
        }



        if(mc_core_list.Board_ADC_DATA.u16Vbus > VBus_OVP_ADC)
        {
            if(VBusOVP_Cnt++ >= VBus_OVP_FiltMS_COUNT)
            {
                VBusOVP_Cnt= VBus_OVP_FiltMS_COUNT;

                user_list.userErr.bits.gVBusOVP = 1;
            }
        } 
        else
        {
            if(VBusOVP_Cnt)
            {
                VBusOVP_Cnt--;
            }
						else
            {
							user_list.userErr.bits.gVBusOVP = 0;
            }
        }


		#endif
}



void User_Temperature_Handler_Fast(void)
{
	 #if EN_MOS_TEMP_DETECT
				 if(mc_core_list.Board_ADC_DATA.u16NTC_MOS> RSM_MOS_TEMP_OVER_THD_ADC)//        
				{

						user_list.userErr.bits.gNtcMos=0;
				}
				 if(
				((mc_core_list.Board_ADC_DATA.u16NTC_MOS < MOS_TEMP_OVER_THD_ADC))
				 )//按键复用
				 {
				 
					user_list.userErr.bits.gNtcMos=1;
				 
				 }
		#endif

}

void User_Temperature_Handler(void)
{

		static u16 NtcMosErrClr_Cnt=0;
		static u16 NtcMosErr_Cnt=0;
    #if EN_MOS_TEMP_DETECT
  
        if(user_list.userErr.bits.gNtcMos == 1)
        {
            if(mc_core_list.Board_ADC_DATA.u16NTC_MOS> RSM_MOS_TEMP_OVER_THD_ADC)//        
            {
                if(NtcMosErrClr_Cnt++ >= RSM_MOS_TEMP_OVER_TIME)
                {
                    NtcMosErrClr_Cnt= RSM_MOS_TEMP_OVER_TIME;
                    user_list.userErr.bits.gNtcMos=0;
                }
            }
            else
            {
                if(NtcMosErrClr_Cnt)
                {
                    NtcMosErrClr_Cnt--;
                }

            }
            NtcMosErr_Cnt= 0;
        }
        else
        {
           NtcMosErrClr_Cnt= 0;
            if(
            (mc_core_list.Board_ADC_DATA.u16NTC_MOS <MOS_TEMP_OVER_THD_ADC )//
						//(strInput.NtcMos_Adc < strInput.NtcMosOTP_Adc)
            //||(strInput.NtcMos_Adc > strInput.NtcMosUTP_Adc)
            )
            {
                if(NtcMosErr_Cnt++ >= MOS_TEMP_OVER_TIME)
                {
                  NtcMosErr_Cnt= MOS_TEMP_OVER_TIME;
                    
									user_list.userErr.bits.gNtcMos=1;
                }
            }
            else
            {
                if(NtcMosErr_Cnt)
                {
                   NtcMosErr_Cnt--;
                }
            }
                    
            
        }
#endif


}

#else 
void user_oc_handle(void)
{
		static u16 IBusAvgOCP1_count=0;
	static u16 IBusAvgOCP2_count=0;
	static u16 IBusAvgOCP3_count=0;
		static u16 IBusAvgOCP4_count=0;
	static u16 IBusPeakOCP1_count=0;
		static u16 IBusAvg_rec_count=0;
		#if (!Multistage_Integral_Peak_OCP_EN)
	static u16 IBusPeakOCP2_count=0;
	static u16 IBusPeakOCP3_count=0;
	static u16 IBusPeakREC_count=0;
	#endif
	u16  ibus_temp=0;
	u16  ipeak_temp=0;
	   
		
		if(mc_core_list.Board_ADC_DATA.u16I_SUM_Avg>mc_core_list.CurrentOffset_ad_data)
		{
			ibus_temp=mc_core_list.Board_ADC_DATA.u16I_SUM_Avg-mc_core_list.CurrentOffset_ad_data;
			ibus_temp=((((u32)ibus_temp)*user_list.Vcc_cal_gain)>>15);
		}
		else 
		{
			ibus_temp=0;
		
		}
			if(mc_core_list.Board_ADC_DATA.u16I_Peak>mc_core_list.CurrentOffset_ad_data)
		{
			ipeak_temp=mc_core_list.Board_ADC_DATA.u16I_Peak-mc_core_list.CurrentOffset_ad_data;
			user_list.IBusPeak_Adc=((((u32)ipeak_temp)*user_list.Vcc_cal_gain)>>15);
		}
		else 
		{
			user_list.IBusPeak_Adc=0;
		
		}	
		
		
//		user_list.IBusAvg1_Adc=(u16)((float)mc_core_list.pwm_actual_runing_data *ibus_temp/PWM_PRIOD_LOAD);	
//		user_list.IBusAvg_Adc=(user_list.IBusAvg1_Adc>>3)+user_list.IBusAvg_Adc-(user_list.IBusAvg_Adc>>3);
		user_list.IBusAvg_Adc=ibus_temp;
		//------------------ocp1
		if(user_list.IBusAvg_Adc>IBus_Avg_OCP1_AdcValue)
		{
				
			IBusAvgOCP1_count++;
			if(IBusAvgOCP1_count>IBus_Avg_OCP1_MS_CONUT)
			{
				IBusAvgOCP1_count=IBus_Avg_OCP1_MS_CONUT;
				user_list.userErr.bits.gIBusAvgOCP1=1;
				IBusAvg_rec_count=0;
			}
		}
		else 
		{
			if(IBusAvgOCP1_count>0)
			{
				IBusAvgOCP1_count--;
			}
			else
			{
				user_list.userErr.bits.gIBusAvgOCP1=0;
			
			}
		}
			////------------------ocp2
			if(user_list.IBusAvg_Adc>IBus_Avg_OCP2_AdcValue)
		{
				
			IBusAvgOCP2_count++;
			if(IBusAvgOCP2_count>IBus_Avg_OCP2_MS_CONUT)
			{
				IBusAvgOCP2_count=IBus_Avg_OCP2_MS_CONUT;
				user_list.userErr.bits.gIBusAvgOCP2=1;
				IBusAvg_rec_count=0;
			}
		}
		else 
		{
			if(IBusAvgOCP2_count>0)
			{
				IBusAvgOCP2_count--;
			}
			else
			{
				user_list.userErr.bits.gIBusAvgOCP2=0;
			
			}
			
		}
		
			//------------------ocp3
			if(user_list.IBusAvg_Adc>IBus_Avg_OCP3_AdcValue)
		{
				
			IBusAvgOCP3_count++;
			if(IBusAvgOCP3_count>IBus_Avg_OCP3_MS_CONUT)
			{
				IBusAvgOCP3_count=IBus_Avg_OCP3_MS_CONUT;
				user_list.userErr.bits.gIBusAvgOCP3=1;
				IBusAvg_rec_count=0;
			}
		}
		else 
		{
			if(IBusAvgOCP3_count>0)
			{
				IBusAvgOCP3_count--;
			}
			else
			{
				user_list.userErr.bits.gIBusAvgOCP3=0;
			
			}
			
		}	
				//------------------ocp4
		if(user_list.IBusAvg_Adc>IBus_Avg_OCP4_AdcValue)
		{
				
			IBusAvgOCP4_count++;
			if(IBusAvgOCP4_count>IBus_Avg_OCP4_MS_CONUT)
			{
				IBusAvgOCP4_count=IBus_Avg_OCP4_MS_CONUT;
				user_list.userErr.bits.gIBusAvgOCP4=1;
				IBusAvg_rec_count=0;
			}
		}
		else 
		{
			if(IBusAvgOCP4_count>0)
			{
				IBusAvgOCP4_count--;
			}
			else
			{
				user_list.userErr.bits.gIBusAvgOCP4=0;
			
			}
			
		}
		
		
		if(user_list.IBusAvg_Adc<IBus_Avg_OCP1_AdcValue&&motor_control_list.bldc_state==MOTOR_STOP)
		{
			IBusAvg_rec_count++;
			if(IBusAvg_rec_count>IBus_Avg_REC_MS_CONUT)
			{
				IBusAvg_rec_count=0;
				user_list.userErr.bits.gIBusAvgOCP4=0;
				user_list.userErr.bits.gIBusAvgOCP3=0;
				user_list.userErr.bits.gIBusAvgOCP2=0;
				user_list.userErr.bits.gIBusAvgOCP1=0;
				IBusAvgOCP4_count=0;
				IBusAvgOCP3_count=0;
				IBusAvgOCP2_count=0;
				IBusAvgOCP1_count=0;
			}
			
		}


#if Multistage_Integral_Peak_OCP_EN
		
			  if(user_list.IBusPeak_Adc > IBus_Smart_Peak_OCP3_AdcValue)
        {

						IBusPeakOCP1_count+=IBus_Smart_OCP3_MS_Operation_Value;
            if(IBusPeakOCP1_count >= 32768 )
            {
               IBusPeakOCP1_count= 32768;
							//mos过载处理
							if(user_list.userErr.bits.gIBusPeakOCP1==0)
							{
//								user_list.Mos_overload_count++;
							
							}
							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
		    else  if(user_list.IBusPeak_Adc > IBus_Smart_Peak_OCP2_AdcValue)
        {

						IBusPeakOCP1_count+=IBus_Smart_OCP2_MS_Operation_Value;
            if(IBusPeakOCP1_count >= 32768 )
            {
               IBusPeakOCP1_count= 32768;
							//mos过载处理
							if(user_list.userErr.bits.gIBusPeakOCP1==0)
							{
//								user_list.Mos_overload_count++;
							
							}
							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
				else if(user_list.IBusPeak_Adc > IBus_Smart_Peak_OCP1_AdcValue)
				{
				
						IBusPeakOCP1_count+=IBus_Smart_OCP1_MS_Operation_Value;
            if(IBusPeakOCP1_count >= 32768 )
            {
               IBusPeakOCP1_count= 32768;
							//mos过载处理
							if(user_list.userErr.bits.gIBusPeakOCP1==0)
							{
//								user_list.Mos_overload_count++;
							
							}
							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
				
				
				}
        else  
					
        {
					
						if(motor_control_list.bldc_state==MOTOR_STOP)
						{
								if(IBusPeakOCP1_count>IBus_Smart_REC_MS_Operation_Value)
							{
									IBusPeakOCP1_count-=IBus_Smart_REC_MS_Operation_Value;
							}
							else
							{
								IBusPeakOCP1_count=0;
									user_list.userErr.bits.gIBusPeakOCP1 = 0;
							}
						}
						else
						{
					
							if(IBusPeakOCP1_count>IBus_Smart_Peak_OCP1_AdcValue)
							{
									IBusPeakOCP1_count-=IBus_Smart_Peak_OCP1_AdcValue;
							}
							else
							{
								IBusPeakOCP1_count=0;
									user_list.userErr.bits.gIBusPeakOCP1 = 0;
							}
						}
        }
		
#else
        if(user_list.IBusPeak_Adc > IBus_Peak_OCP1_AdcValue)
        {
            if(IBusPeakOCP1_count++ >= IBus_Peak_OCP1_MS_CONUT )
            {
               IBusPeakOCP1_count= IBus_Peak_OCP1_MS_CONUT;

							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
        else
        {
            if(IBusPeakOCP1_count)
            {
                IBusPeakOCP1_count--;
            }
            else
            {
//								user_list.userErr.bits.gIBusPeakOCP1 = 0;
            }
        }
				
				
				
				  if(user_list.IBusPeak_Adc > IBus_Peak_OCP2_AdcValue)
        {
            if(IBusPeakOCP2_count++ >= IBus_Peak_OCP2_MS_CONUT )
            {
               IBusPeakOCP2_count= IBus_Peak_OCP2_MS_CONUT;

							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
        else
        {
            if(IBusPeakOCP2_count)
            {
                IBusPeakOCP2_count--;
            }
            else
            {
//								user_list.userErr.bits.gIBusPeakOCP1 = 0;
            }
        }
				
				
					if(user_list.IBusPeak_Adc > IBus_Peak_OCP3_AdcValue)
        {
            if(IBusPeakOCP3_count++ >= IBus_Peak_OCP3_MS_CONUT )
            {
               IBusPeakOCP3_count= IBus_Peak_OCP3_MS_CONUT;

							user_list.userErr.bits.gIBusPeakOCP1 = 1; 
            }
        }
        else
        {
            if(IBusPeakOCP3_count)
            {
                IBusPeakOCP3_count--;
            }
            else
            {
//								user_list.userErr.bits.gIBusPeakOCP1 = 0;
            }
        }
				
				if(user_list.userErr.bits.gIBusPeakOCP1)
				{
					if(user_list.IBusPeak_Adc < IBus_Peak_OCP1_AdcValue)
					{
						IBusPeakREC_count++;
						if(IBusPeakREC_count>IBus_Peak_REC_MS_CONUT)
						{
							user_list.userErr.bits.gIBusPeakOCP1=0;
						}
										
					}
				}
				else 
				{
					IBusPeakREC_count=0;
				
				}
				
#endif		
}

void Volt_Handler_fast(void)
{
	
					u32 Vbus_temp=0;
				Vbus_temp=((user_list.Vcc_cal_gain*mc_core_list.Board_ADC_DATA.u16Vbus))>>15;
				#if(1)
	
	
        if((Vbus_temp < VBus_Stb_UVP_ADC)) // 0 可以改为电机停止
        {
      
								user_list.userErr.bits.gVBusStbUVP = 1; //静态欠压保护

        }
        else
        {

			      	user_list.userErr.bits.gVBusStbUVP = 0;
        }

				#endif
        if(Vbus_temp < VBus_Run_UVP1_ADC)
        {
  
     
                user_list.userErr.bits.gVBusRunUVP1 = 1;
                

        }
        else
        {

							user_list.userErr.bits.gVBusRunUVP1 = 0;

        }
        
       
        if(Vbus_temp < VBus_Run_UVP2_ADC)
        {
    
  
              user_list.userErr.bits.gVBusRunUVP2 = 1;
     
        }
        else
        {

					  user_list.userErr.bits.gVBusRunUVP2 = 0;
        }
        if(Vbus_temp > VBus_OVP_ADC)
        {
              user_list.userErr.bits.gVBusOVP = 1;
        } 
        else
        {
							user_list.userErr.bits.gVBusOVP = 0;
        }
}
void Volt_Handler(void)
{


	static u16 VBusStbUVP_Cnt=0;//上电只需检测一次
	static u16 VBusRunUVP1_Cnt=0;
	static u16 VBusRunUVP2_Cnt=0;
	static u16 	VBusOVP_Cnt=0;
		u32 Vbus_temp=0;
		Vbus_temp=((user_list.Vcc_cal_gain*mc_core_list.Board_ADC_DATA.u16Vbus))>>15;
	
    #if EN_VOLATAGE_PROTECT    
				user_list.VBus_Vx100=(Vbus_temp*VBUS_BASE_X100)>>12;
        if((Vbus_temp < VBus_Stb_UVP_ADC)&&(user_list.user_state != TOOL_RUN)) // 0 可以改为电机停止
        {
            if(VBusStbUVP_Cnt++ >= VBus_Stb_UVP_FiltMS_COUNT)
            {
                VBusStbUVP_Cnt= VBus_Stb_UVP_FiltMS_COUNT;

								user_list.userErr.bits.gVBusStbUVP = 1; //静态欠压保护
            }
        }
        else
        {
            if(VBusStbUVP_Cnt)
            {
                VBusStbUVP_Cnt--;
            }
            else
            {
			      	user_list.userErr.bits.gVBusStbUVP = 0;
            }
        }


        if(Vbus_temp < VBus_Run_UVP1_ADC)
        {
            if(VBusRunUVP1_Cnt++ >= VBus_Run_UVP1_FiltMS_COUNT)
            {
                VBusRunUVP1_Cnt= VBus_Run_UVP1_FiltMS_COUNT;
     
                user_list.userErr.bits.gVBusRunUVP1 = 1;
                
            }
        }
        else
        {
            if(VBusRunUVP1_Cnt)
            {
                VBusRunUVP1_Cnt--;
            }
			else
            {
							user_list.userErr.bits.gVBusRunUVP1 = 0;
            }
        }
        
       
        if(Vbus_temp < VBus_Run_UVP2_ADC)
        {
            if(VBusRunUVP2_Cnt++ >=VBus_Run_UVP2_FiltMS_COUNT)
            {
                VBusRunUVP2_Cnt= VBus_Run_UVP2_FiltMS_COUNT;
  
                user_list.userErr.bits.gVBusRunUVP2 = 1;
            }
        }
        else
        {
            if(VBusRunUVP2_Cnt)
            {
                VBusRunUVP2_Cnt--;
            }
						else
            {
					  	user_list.userErr.bits.gVBusRunUVP2 = 0;
            }
        }



        if(Vbus_temp > VBus_OVP_ADC)
        {
            if(VBusOVP_Cnt++ >= VBus_OVP_FiltMS_COUNT)
            {
                VBusOVP_Cnt= VBus_OVP_FiltMS_COUNT;

                user_list.userErr.bits.gVBusOVP = 1;
            }
        } 
        else
        {
            if(VBusOVP_Cnt)
            {
                VBusOVP_Cnt--;
            }
						else
            {
							user_list.userErr.bits.gVBusOVP = 0;
            }
        }


		#endif
}



void User_Temperature_Handler_Fast(void)
{
	 #if EN_MOS_TEMP_DETECT
				u32 T_mos_temp=0;
				T_mos_temp=((user_list.Vcc_cal_gain*mc_core_list.Board_ADC_DATA.u16NTC_MOS))>>15;
	
				 if(T_mos_temp> RSM_MOS_TEMP_OVER_THD_ADC)//        
				{

						user_list.userErr.bits.gNtcMos=0;
				}
				 if(
				((mc_core_list.Board_ADC_DATA.u16NTC_MOS < MOS_TEMP_OVER_THD_ADC))
				 )//按键复用
				 {
				 
					user_list.userErr.bits.gNtcMos=1;
				 
				 }
		#endif

}

void User_Temperature_Handler(void)
{

		static u16 NtcMosErrClr_Cnt=0;
		static u16 NtcMosErr_Cnt=0;
    #if EN_MOS_TEMP_DETECT
  			u32 T_mos_temp=0;
				T_mos_temp=((user_list.Vcc_cal_gain*mc_core_list.Board_ADC_DATA.u16NTC_MOS))>>15;
        if(user_list.userErr.bits.gNtcMos == 1)
        {
            if(T_mos_temp> RSM_MOS_TEMP_OVER_THD_ADC)//        
            {
                if(NtcMosErrClr_Cnt++ >= RSM_MOS_TEMP_OVER_TIME)
                {
                    NtcMosErrClr_Cnt= RSM_MOS_TEMP_OVER_TIME;
                    user_list.userErr.bits.gNtcMos=0;
                }
            }
            else
            {
                if(NtcMosErrClr_Cnt)
                {
                    NtcMosErrClr_Cnt--;
                }

            }
            NtcMosErr_Cnt= 0;
        }
        else
        {
           NtcMosErrClr_Cnt= 0;
            if(
            (T_mos_temp <MOS_TEMP_OVER_THD_ADC )//
						//(strInput.NtcMos_Adc < strInput.NtcMosOTP_Adc)
            //||(strInput.NtcMos_Adc > strInput.NtcMosUTP_Adc)
            )
            {
                if(NtcMosErr_Cnt++ >= MOS_TEMP_OVER_TIME)
                {
                  NtcMosErr_Cnt= MOS_TEMP_OVER_TIME;
                    
									user_list.userErr.bits.gNtcMos=1;
                }
            }
            else
            {
                if(NtcMosErr_Cnt)
                {
                   NtcMosErr_Cnt--;
                }
            }
                    
            
        }
#endif


}

#endif
void power_on_fly_machine_handle(void)
{
		u8 i=0;
		u32 fly_machine_adc_sum=0;
		u32 fly_machine_adc=0;
		ADC_Cmd(ADC1, DISABLE);
		ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);		// 关闭ADC转换中断
		TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);	// 关闭TIM1更新中断
		
    ADC_AnyChannelNumCfg(ADC1, 0);
    ADC_AnyChannelSelect(ADC1, 0, FLY_CHANNEL);	
    ADC_AnyChannelCmd(ADC1, ENABLE);
	
	
	
		ADC_ExternalTrigConvCmd(ADC1, DISABLE);			// 关闭ADC外部触发转换
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);					// 清除ADC转换标志位
	  ADC_SampleTimeConfig(ADC1,0, ADC_SampleTime_240_5);
		ADC_Cmd(ADC1, ENABLE);
		systick_delay_us(100);
	
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
		fly_machine_adc_sum=0;
		for(i=0;i<8;i++)
		{
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);								// 软件触发ADC转换, 获取峰值电流采样值
			while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);		// 等待ADC转换完成
			ADC_ClearFlag(ADC1, ADC_FLAG_EOC);										// 清除ADC转换完成标志位
			fly_machine_adc_sum += (uint16_t)GET_ADC_VALUE(FLY_CHANNEL);
		}
		fly_machine_adc=fly_machine_adc_sum>>3;
		if(fly_machine_adc<737)
		{
			machine_error_hold.userErr_hold.bits.fly_machine=1;
		
		}
		ADC_Cmd(ADC1, DISABLE);
		systick_delay_us(100);
		Board_ADC_Init();

}

void user_direction_handle(void)
{
	
	static u8 dir_cw_count=0;
	static u8 dir_ccw_count=0;
	#if(1)
//	if(GPIO_ReadInputDataBit(DIR_PORT,DIR_PIN)) 
	
	if(user_list.Dir_Data) 
	{
			dir_ccw_count=0;
			if(++dir_cw_count>10)
			{
				dir_cw_count=10;
				if(motor_control_list.bldc_state==MOTOR_STOP||motor_control_list.bldc_state==MOTOR_HBRAKE||motor_control_list.bldc_state==MOTOR_SBRAKE||motor_control_list.bldc_state==MOTOR_BRAKE2STOP)
				{
					user_list.flag.bits.gToolDir=Tool_Dir_CCW_Value;
					mc_core_list.mc_dir=user_list.flag.bits.gToolDir;
				}
				
			}
			
			

	}
	else 
	{
			dir_cw_count=0;
			if(++dir_ccw_count>10)
			{
				dir_ccw_count=10;
				if(motor_control_list.bldc_state==MOTOR_STOP||motor_control_list.bldc_state==MOTOR_HBRAKE||motor_control_list.bldc_state==MOTOR_SBRAKE||motor_control_list.bldc_state==MOTOR_BRAKE2STOP)
				{
					user_list.flag.bits.gToolDir=Tool_Dir_CW_Value;
					mc_core_list.mc_dir=user_list.flag.bits.gToolDir;
				}
			
			}
	
	}
	#endif
	
	#if(0)
	user_list.dir_adc=(uint16_t)GET_ADC_VALUE(DIR_CHANNEL);
//	mc_core_list.mc_dir=1;
		if(user_list.dir_adc>3600) 
	{
			dir_ccw_count=0;
			if(++dir_cw_count>10)
			{
				dir_cw_count=10;
				if(motor_control_list.bldc_state==MOTOR_STOP||motor_control_list.bldc_state==MOTOR_HBRAKE||motor_control_list.bldc_state==MOTOR_SBRAKE||motor_control_list.bldc_state==MOTOR_BRAKE2STOP)
				{
					user_list.flag.bits.gToolDir=Tool_Dir_CCW_Value;
					mc_core_list.mc_dir=user_list.flag.bits.gToolDir;
				}
			}
			
			

	}
	else if(user_list.dir_adc<3600&&user_list.dir_adc>2000)
	{
			dir_cw_count=0;
			if(++dir_ccw_count>10)
			{
				dir_ccw_count=10;
				if(motor_control_list.bldc_state==MOTOR_STOP||motor_control_list.bldc_state==MOTOR_HBRAKE||motor_control_list.bldc_state==MOTOR_SBRAKE||motor_control_list.bldc_state==MOTOR_BRAKE2STOP)
				{
					user_list.flag.bits.gToolDir=Tool_Dir_CW_Value;
					mc_core_list.mc_dir=user_list.flag.bits.gToolDir;
				
				}
				
			}
	
	}
	#endif
}
u8 key_scan(void)
{
	static u16 key_count=0;
	u8 res=0;
//	user_list.key_adc=(uint16_t)GET_ADC_VALUE(DIR_CHANNEL);
		user_list.key_adc =user_list.Key_Data;
	if(!user_list.key_adc)
	{
		if(key_count<2000)
		{
			key_count++;
		}

		if(key_count==1000)
		{
			res=2;
		}
	}
	else 
	{
		if(key_count>50&&key_count<1000)
		{
			res=1;
		}
		key_count=0;
	
	}
	return res;


}

//void gears_deal(void)
//{
//	if(key_scan())
//	{
//		user_list.gears_level++;
//		if(user_list.gears_level>1)
//		{
//		
//			user_list.gears_level=0;
//		}
//	}

//}
u32 PI_sum_i=0;
u32 PI_p=0;
u32	pwm_duty_aim_PI_OUT=0;
void pwm_duty_aim_deal(void)
{
	
	#if(1)
		#if GRINDER_EN
		motor_control_list.pwm_duty_aim_per=(u16)(PWM_PRIOD_LOAD);
		#else 
		if(mc_core_list.Board_ADC_DATA.u16VR<MIN_TRG_ADC)
		{
		
			mc_core_list.Board_ADC_DATA.u16VR=MIN_TRG_ADC;
		}
		
	 motor_control_list.pwm_duty_aim_per=(uint16_t)((float)(mc_core_list.Board_ADC_DATA.u16VR - MIN_TRG_ADC)*K_ADC_DUTY+ MIN_TOOL_DUTY);
	#endif
		//  	motor_control_list.pwm_duty_aim=motor_control_list.pwm_duty_aim_per;
	#endif
	#if(0)
	
		
		#if 0
	if(motor_run_count<50)
	
	{
		pwm_duty_aim_PI_OUT=0;
	
	}
	else 
	{
		if(mc_core_list.time_count_60_degree>((u32)(1500*MOTOR_TIMR_FRQ_MHZ)))
		{
			PI_p=mc_core_list.time_count_60_degree-((u32)(1500*MOTOR_TIMR_FRQ_MHZ));
			PI_p=(PI_p>>2);
			if(PI_sum_i<PWM_PRIOD_LOAD)
			{
				PI_sum_i++;
				
			}
			pwm_duty_aim_PI_OUT=PI_sum_i+PI_p;
		}
		
		else 
		{
			PI_p=((u32)(1500*MOTOR_TIMR_FRQ_MHZ))-mc_core_list.time_count_60_degree;
			PI_p=(PI_p>>2);
			if(PI_sum_i>LIMIT_MIN_PWN_DUTY)
			{
				PI_sum_i--;
			}
			if(PI_sum_i>PI_p)
			{
				pwm_duty_aim_PI_OUT=PI_sum_i-PI_p;
			}
			else 
			{
				pwm_duty_aim_PI_OUT=LIMIT_MIN_PWN_DUTY;
			
			}
		}
		

	}
	
		if(pwm_duty_aim_PI_OUT>LIMIT_MAX_PWN_DUTY)
		{
			pwm_duty_aim_PI_OUT=LIMIT_MAX_PWN_DUTY;
		
		}
			if(pwm_duty_aim_PI_OUT>motor_control_list.pwm_duty_aim_per)
		{
			motor_control_list.pwm_duty_aim=pwm_duty_aim_PI_OUT;
		}
		else 
		{
		
			motor_control_list.pwm_duty_aim=motor_control_list.pwm_duty_aim_per;
		}
		#else
//		motor_control_list.pwm_duty_aim_per=(u16)((PWM_PRIOD_LOAD)*1.0f);
		motor_control_list.pwm_duty_aim=motor_control_list.pwm_duty_aim_per;
		#endif
	#endif

}	
uint8_t pulse_mode=0;
uint16_t pulse_max_duty=0;
void pulse_th_handle(void)
{
	if(motor_control_list.pwm_duty_aim_per<((u32)(PWM_PRIOD_LOAD*0.8f)))
	{
		
		pulse_mode=1;
	}
	else 
	{
		pulse_mode=0;
	}
	if(user_list.flag.bits.gToolTrigger==0)
	{
		pulse_max_duty=((u32)(PWM_PRIOD_LOAD*0.14f));
	
	}


}
void user_gears_for_grinder_handle(void)
{
	#if 0
	u8 res=0;
	res=key_scan();
	if(res)
	{
		if(user_list.gears_level<2)
		{
			user_list.gears_level++;
			
		}
		else
		{
			user_list.gears_level=0;
		}
		user_list.gears_level_changed=1;
	}
	#endif
	if(user_list.gears_level==0)
	{
		motor_control_list.pwm_duty_aim=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS1)>>15;
	}
	else if(user_list.gears_level==1)
	{
		motor_control_list.pwm_duty_aim=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS2)>>15;
	}
	else 
	{
	
		motor_control_list.pwm_duty_aim=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS3)>>15;
	}
	
	if(user_list.gears_level_changed&&user_list.user_state!=TOOL_RUN)
	{
		user_list.gears_level_changed=0;
		save_user_data();
	}
}
void user_gears_handle(void)
{
	
	#if 0
		u8 res=0;
		static uint8_t pulse_dir=0;
		static uint16_t pulse_cout=0;
		pulse_th_handle();
		res=key_scan();
		if(user_list.user_state==TOOL_STOP)
		{
				if(res==1)
				{
					if(user_list.gears_level<1)
					{
						user_list.gears_level++;
						
					}
					else
					{
						user_list.gears_level=0;
					}
					save_user_data();	
			//		user_list.gears_level_changed=1;
				}
		}

	if(user_list.gears_level==0)
	{
		if(pulse_mode)
		{
			motor_control_list.pwm_duty_aim_per=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS1)>>15;
		
			if(pulse_dir==0)
			{
				
				if(motor_control_list.pwm_duty_aim>LIMIT_MIN_PWN_DUTY)
				{
					motor_control_list.pwm_duty_aim-=4;
				}
				pulse_cout++;
				if(pulse_cout>=250)
				{
					pulse_dir=1;
				}
			}
			else 
			{
				if(motor_control_list.pwm_duty_aim<pulse_max_duty||motor_control_list.pwm_duty_aim<((u32)(PWM_PRIOD_LOAD*0.20)))
				{
					motor_control_list.pwm_duty_aim+=4;

					
				}
				
				if(pulse_cout) 
				{
					pulse_cout--;
					
				}
				else 
				{
					pulse_max_duty+=((u32)(PWM_PRIOD_LOAD*0.02f));
					if(pulse_max_duty>((u32)(PWM_PRIOD_LOAD*0.45f)))
					{
						
						pulse_max_duty=((u32)(PWM_PRIOD_LOAD*0.45f));
					}
					pulse_dir=0;
				}
			
			}
		}
		else 
		{
			motor_control_list.pwm_duty_aim=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS1)>>15;
		
		}
		
	}
	else 
	{
	
		motor_control_list.pwm_duty_aim=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS3)>>15;
	}
	#else 
	motor_control_list.pwm_duty_aim=motor_control_list.pwm_duty_aim_per;
	#endif
}
void user_WrenchKey_Handler(void)
{
//	gears_deal();//扫描档位
	uint16_t pwm_duty_aim_temp=0;
	static uint16_t auto_current_ad=0;
	static uint16_t auto_stop_time=0;
	u8 res=0;
	res=key_scan();
	if(motor_control_list.bldc_state!=MOTOR_RUN&&motor_control_list.bldc_state!=MOTOR_MOTION&&motor_control_list.bldc_state!=MOTOR_POSITION)
	{
			if(user_list.flag.bits.gToolDir==Tool_Dir_CW_Value)
			{
				if(res)
				{
					user_list.gears_level++;
					if(user_list.gears_level>2)
					{
					
						user_list.gears_level=0;
					}
					save_user_data();
				}
			}
			else 
			{
//				if(res)
//				{
//					user_list.gears_ccw_level++;
//					if(user_list.gears_ccw_level>3)
//					{
//					
//						user_list.gears_ccw_level=0;
//					}
//					save_user_data();
//				}
				if(res)
				{
						user_list.flag.bits.gAutoStopEn=!user_list.flag.bits.gAutoStopEn;
					save_user_data();
				}
			
			}
	}
	
	if(user_list.flag.bits.gToolDir==Tool_Dir_CCW_Value)
	{
//		if(user_list.gears_ccw_level==0)
//		{
//			user_list.flag.bits.gAutoStopEn=1;
//		}
//		else 
//		{
//			user_list.flag.bits.gAutoStopEn=0;
//		}
			if(user_list.flag.bits.gAutoStopEn)
			{
				pwm_duty_aim_temp=PWM_PRIOD_LOAD;
			}
			else 
			{
				
//				if(motor_control_list.pwm_duty_aim_per<((u16)(0.3f*PWM_PRIOD_LOAD)))
//				{
//					motor_control_list.pwm_duty_aim_per=((u16)(0.3f*PWM_PRIOD_LOAD));
//				}
			//	motor_control_list.pwm_duty_aim=motor_control_list.pwm_duty_aim_per;

//					if(user_list.gears_ccw_level==0)
//					{
//						pwm_duty_aim_temp=PWM_PRIOD_LOAD;
//					}
//					else if(user_list.gears_ccw_level==1)
//					{
//						pwm_duty_aim_temp=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS2)>>15;
//					}
//					else if(user_list.gears_ccw_level==2)
//					{
//						pwm_duty_aim_temp=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS3)>>15;
//					}
//					else 
//					{
//					
//						pwm_duty_aim_temp=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS4)>>15;
//					}
//					if(pwm_duty_aim_temp<((u16)(PWM_PRIOD_LOAD*0.50f)))
//					{
//						pwm_duty_aim_temp=((u16)(PWM_PRIOD_LOAD*0.50f));
//					}
				
				pwm_duty_aim_temp=motor_control_list.pwm_duty_aim_per;
				
			}
			
	}
	else 
	{
	
		
		if(user_list.gears_level==0)
		{
			pwm_duty_aim_temp=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS1)>>15;
		}
		else if(user_list.gears_level==1)
		{
			pwm_duty_aim_temp=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS2)>>15;
		}
		else 
		{
		
			pwm_duty_aim_temp=(motor_control_list.pwm_duty_aim_per*PWM_DUTY_GEARS3)>>15;
		}
		
		
		if(pwm_duty_aim_temp>Wrench_CW_MAX_DUTY)
		{
			pwm_duty_aim_temp=(Wrench_CW_MAX_DUTY);
		}

	}
	
		#if(1)
		if(user_list.Load_exist)
		{
			#if 0
				if(motor_control_list.motor_run_count<(180*SLOWLOOP_1ms_CNT_LOAD))
				
				{
					pwm_duty_aim_PI_OUT=(0.65f*PWM_PRIOD_LOAD);
				
				}
				else 
				{
					if(mc_core_list.time_count_60_degree>((u32)(1000*MOTOR_TIMR_FRQ_MHZ)))
					{
						PI_p=mc_core_list.time_count_60_degree-((u32)(1000*MOTOR_TIMR_FRQ_MHZ));
						PI_p=(PI_p>>2);
						if(PI_sum_i<PWM_PRIOD_LOAD)
						{
							PI_sum_i++;
							
						}
						pwm_duty_aim_PI_OUT=PI_sum_i+PI_p;
					}
					
					else 
					{
						PI_p=((u32)(1000*MOTOR_TIMR_FRQ_MHZ))-mc_core_list.time_count_60_degree;
						PI_p=(PI_p>>2);
						if(PI_sum_i>LIMIT_MIN_PWN_DUTY)
						{
							PI_sum_i--;
						}
						if(PI_sum_i>PI_p)
						{
							pwm_duty_aim_PI_OUT=PI_sum_i-PI_p;
						}
						else 
						{
							pwm_duty_aim_PI_OUT=LIMIT_MIN_PWN_DUTY;
						
						}
					}
					

				}
				
					if(pwm_duty_aim_PI_OUT>LIMIT_MAX_PWN_DUTY)
					{
						pwm_duty_aim_PI_OUT=LIMIT_MAX_PWN_DUTY;
					
					}
						if(pwm_duty_aim_PI_OUT>pwm_duty_aim_temp)
					{
						motor_control_list.pwm_duty_aim=pwm_duty_aim_PI_OUT;
					}
					else 
					{
					
						motor_control_list.pwm_duty_aim=pwm_duty_aim_temp;
					}
					#endif
					if(pwm_duty_aim_temp<RESTAR_MIN_PWM_AIM_DUTY)
					{
						motor_control_list.pwm_duty_aim=RESTAR_MIN_PWM_AIM_DUTY;
					}
					else 
					{
					
					motor_control_list.pwm_duty_aim=pwm_duty_aim_temp;
					}
					
		}
		else 
		{
			motor_control_list.pwm_duty_aim=pwm_duty_aim_temp;
		}
	#else
	
	motor_control_list.pwm_duty_aim=pwm_duty_aim_temp;
	#endif
	
	if(user_list.flag.bits.gAutoStopEn&&user_list.flag.bits.gToolDir==Tool_Dir_CCW_Value&&user_list.user_state==TOOL_RUN)
	{
				if(user_list.CCW_Auto_Mask_count<63334)
				{
					user_list.CCW_Auto_Mask_count++;
				}
				if(user_list.CCW_Auto_Mask_count==CCW_AUTO_STOP_START_MASK_COUNT)
				{
					auto_current_ad=user_list.IBusAvg_Adc;
					auto_current_ad=((((u32)auto_current_ad)*((u32)21299))>>15);
//					if(auto_current_ad>CCW_AUTO_STOP_CURRENT_2_AdcValue)
//					{
//						auto_current_ad=(auto_current_ad-CCW_AUTO_STOP_CURRENT_2_AdcValue);
//					}
//					else 
//					{
//						auto_current_ad=0;
//					}
					if(auto_current_ad<CCW_AUTO_STOP_CURRENT_AdcValue)
					{
						auto_current_ad=CCW_AUTO_STOP_CURRENT_AdcValue;
					}
					
					if(auto_current_ad>CCW_AUTO_STOP_CURRENT_3_AdcValue)
					{
						auto_current_ad=CCW_AUTO_STOP_CURRENT_3_AdcValue;
					}
				
				}
				#if 0
				if(user_list.IBusAvg_Adc < auto_current_ad&&user_list.CCW_Auto_Mask_count>CCW_AUTO_STOP_START_MASK_COUNT)
				{						
						if(mc_core_list.Run_SectorCnt > AUTO_STOP_Sector_CNT) //
						{
								user_list.flag.bits.gAutoStop = 1;

								mc_core_list.Run_SectorCnt = 0;	
						}		
				}
				else
				{
								
								mc_core_list.Run_SectorCnt = 0;
				}
				#else 
				if(user_list.CCW_Auto_Mask_count>CCW_AUTO_STOP_START_MASK_COUNT)
				{
				
					if(user_list.IBusAvg_Adc < auto_current_ad)
					{
						auto_stop_time++;
						if(auto_stop_time > AUTO_STOP_Sector_CNT) //
						{
								user_list.flag.bits.gAutoStop = 1;
								auto_stop_time=0;
								mc_core_list.Run_SectorCnt = 0;	
						}	
					}
					else 
					{
						if(auto_stop_time)
						{
							auto_stop_time--;
						}
					}
					
				}
				else 
				{
				
					auto_stop_time=0;
				}
				
				#endif
				
	}
	else 
	{
			auto_stop_time=0;
			mc_core_list.Run_SectorCnt = 0;	
			user_list.CCW_Auto_Mask_count=0;
	}
	

}




void motor_on_off_control(void)
{
	
 static 	u8 gToolTrigger_off_count=0;
	static 	u8 gToolTrigger_on_count=0;
	static u8 key_release_count=0;
//		user_list.flag.bits.gToolTrigger=1;
	#if 1
	#if GRINDER_EN
		if(mc_core_list.Board_ADC_DATA.u16VR<(TRG_ON_ADC-20))
	#else
	
	if(mc_core_list.Board_ADC_DATA.u16VR<(TRG_ON_ADC-100))
		#endif
	{
		gToolTrigger_off_count++;
		gToolTrigger_on_count=0;
		if(gToolTrigger_off_count>10)
		{
			user_list.flag.bits.gToolTrigger=0;
		}
	
	}
	if(mc_core_list.Board_ADC_DATA.u16VR>TRG_ON_ADC)
	{
				gToolTrigger_off_count=0;
	   	gToolTrigger_on_count++;
			if(gToolTrigger_on_count>10)
			{
			user_list.flag.bits.gToolTrigger=1;
			}
	}
	
	#endif
	
	
	#if 0
		if(GPIO_ReadInputDataBit(DIR_PORT,DIR_PIN)==1) 
	{
		gToolTrigger_off_count++;
		gToolTrigger_on_count=0;
		if(gToolTrigger_off_count>10)
		{
			user_list.flag.bits.gToolTrigger=0;
		}
	
	}
	if(GPIO_ReadInputDataBit(DIR_PORT,DIR_PIN)==0) 
	{
				gToolTrigger_off_count=0;
	   	gToolTrigger_on_count++;
			if(gToolTrigger_on_count>10)
			{
			user_list.flag.bits.gToolTrigger=1;
			}
	}
	
	#endif

	/* KEY is active low; keep the VR trigger state independent. */
	if(user_list.Key_Data==0)
	{
		user_list.Key_Stop_Lock=1;
		key_release_count=0;
	}
	else if(user_list.Key_Stop_Lock)
	{
		#if GRINDER_EN
		if(mc_core_list.Board_ADC_DATA.u16VR<(TRG_ON_ADC-20))
		#else
		if(mc_core_list.Board_ADC_DATA.u16VR<(TRG_ON_ADC-100))
		#endif
		{
			if(key_release_count<=10)
			{
				key_release_count++;
			}
			/* Unlock only after KEY and VR have both been released. */
			if(key_release_count>10&&user_list.flag.bits.gToolTrigger==0)
			{
				user_list.Key_Stop_Lock=0;
				key_release_count=0;
			}
		}
		else
		{
			key_release_count=0;
		}
	}
	else
	{
		key_release_count=0;
	}

}

void error_hold_updata(void)
{
	machine_error_hold.MC_error_hold.word|=mc_core_list.MC_error.word;
	machine_error_hold.userErr_hold.word|=user_list.userErr.word;

}

void user_state_control(void)
{
	

				switch(user_list.user_state)
				{
					case TOOL_STOP:
						user_list.flag.bits.gToolEn=0;
					user_list.Load_exist=0;
						if(++user_list.sleep_cnt>SLEEP_TIME)
						{
							user_list.user_state=TOOL_POWER_DOWN;
							user_list.sleep_cnt=0;
						}
//						if(!user_list.key_adc)
//						{
//							user_list.sleep_cnt=0;
//						}
						if(machine_error_hold.MC_error_hold.word!=0||machine_error_hold.userErr_hold.word!=0)
						{
							user_list.user_state=TOOL_ERROR_STOP;
						}
					  else if(user_list.flag.bits.gAutoStop == 1)
						{
									if(user_list.flag.bits.gToolTrigger==0)
									{
										user_list.flag.bits.gAutoStop =0;
									}
						}
						else if(user_list.flag.bits.gToolTrigger&&user_list.Key_Stop_Lock==0)
						{
						
								user_list.flag.bits.gToolEn=1;
								user_list.user_state=TOOL_RUN;
								user_list.sleep_cnt=0;
						}
						
						
					break;
						case TOOL_ERROR_STOP:
							user_list.flag.bits.gToolEn=0;
//						user_list.Load_exist=0;
						if(++user_list.sleep_cnt>SLEEP_TIME)
						{
							user_list.user_state=TOOL_POWER_DOWN;
							user_list.sleep_cnt=0;
						}
//						if(!user_list.key_adc)
//						{
//							user_list.sleep_cnt=0;
//						}
						if(user_list.flag.bits.gToolTrigger==0)
						{
								
								if(user_list.userErr.word==0)
								{
									
									if(	machine_error_hold.userErr_hold.bits.gIBusAcmpOCP==1&&motor_control_list.bldc_state==MOTOR_STOP)
									{
									
										machine_error_hold.userErr_hold.bits.gIBusAcmpOCP=0;
										TIM_CtrlPWMOutputs(TIM1, ENABLE);
									}
									#if 0
									mc_core_list.MC_error.word=0;
									machine_error_hold.MC_error_hold.word=0;
									#else
									mc_core_list.MC_error.word&=(0x00000004);//堵转保护不直接清
									machine_error_hold.MC_error_hold.word&=(0x00000004);	
									if(mc_core_list.MC_error.bits.gBlock==0) //mc_core_list.MC_error.bits.gBlock为0后清除
									{
										machine_error_hold.MC_error_hold.bits.gBlock=0;
									}
									
									
									#endif
									machine_error_hold.userErr_hold.word&=0x00000001;
									
								
								}
								if(	machine_error_hold.MC_error_hold.word==0&&machine_error_hold.userErr_hold.word==0)
								{
									user_list.user_state=TOOL_STOP;
								}
//							user_list.userErr.word=0;
							
							
						}
						else
						{
							user_list.sleep_cnt=0;
						}
							break;
					case TOOL_RUN:
						#if(RESTAR_EN)
							if(machine_error_hold.userErr_hold.word!=0||((machine_error_hold.MC_error_hold.word&0xfffb) != 0)||\
					(machine_error_hold.MC_error_hold.bits.gBlock&&(user_list.Load_exist>=RESTAR_COUNT)))
						#else
						if(machine_error_hold.userErr_hold.word!=0||((machine_error_hold.MC_error_hold.word) != 0))
						#endif
						{
							user_list.user_state=TOOL_ERROR_STOP;
						
						}
						if(user_list.flag.bits.gToolTrigger==0)
						{
						
							user_list.user_state=TOOL_STOP;
						}
						
						   if(user_list.flag.bits.gAutoStop == 1)
            {
                user_list.flag.bits.gToolEn = 0;   /**/
//                user_list.ToolState_Cnt = 0;
                
                user_list.user_state = TOOL_STOP;    
            }
            
					
						break;
						case TOOL_POWER_DOWN:
							user_list.sleep_cnt=0;
							GPIO_ResetBits(EN_PORT, EN_PIN);
							if(user_list.flag.bits.gToolTrigger)
							{
									if(machine_error_hold.MC_error_hold.word!=0||machine_error_hold.userErr_hold.word!=0)
									{
									
										user_list.user_state = TOOL_ERROR_STOP; 
									}
									else 
									{
										user_list.user_state = TOOL_STOP;  
									}
							GPIO_SetBits(EN_PORT, EN_PIN);
							}
							break;
					default:
						break;
				
				
				
				}


	/* Revoke motor enable in this cycle without overwriting fault states. */
	if(user_list.Key_Stop_Lock)
	{
		user_list.flag.bits.gToolEn=0;
		if(user_list.user_state==TOOL_RUN)
		{
			user_list.user_state=TOOL_STOP;
		}
	}

}
