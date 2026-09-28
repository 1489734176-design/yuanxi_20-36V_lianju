/**
 * @file     main.c
 * @author   MindMotion Motor Team : Wesson
 * @brief    This file provides the main functions and test samples.
 *
 * @attention
 *
 * THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
 * CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
 * TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
 * HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
 * CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
 *
 * <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
 */

/** Define to prevent recursive inclusion */
#define _MAIN_C_

/** Files includes */
#include "main.h"
#include "drv_inc.h"
#include "board.h"
#include "user_control.h"
#include "drv_pwm.h"
#include "parameter.h"
#include "mc_core.h"
#include "systick.h"
#include "motor_control.h"
#include "flash_data_save.h"
#include "led.h"
/**
 * @addtogroup MM32_User_Layer
 * @{
 */
 
//DiagFlag 								DiagnoseFAULT;
//Motor_TypeDef 					Motor_1st;
//UserFilt_TypeDef				Motor_User;

//LoopCMP_T 							RPValue;
//MovingAvgData 					SpeedFdk;
//NormalizationType 			RP;
//NormalizationType 			SPEEDRPM_Cmd;
//LoopCMP_T 							SPEEDRPM_Cmd_Value;

//ADC_OnflyDetect_TypeDef	ADC_OnflyDetect_lib;
//CMP_Onfly_TypeDef				CMP_Onfly_lib;
//BLDC_Control_TypeDef		BLDC_Control_lib;
//ADC_Zero_TypeDef				ADC_ZeroCross_lib;
//CMP_Zero_TypeDef				CMP_ZeroCross_lib;

/**
 * @addtogroup User_Main
 * @{
 */
void GPIO_Toggle(GPIO_TypeDef *GPIOn, uint16_t PINn)
{
    if (Bit_RESET == GPIO_ReadOutputDataBit(GPIOn, PINn))
    {
        GPIO_SetBits(GPIOn, PINn);
    }
    else
    {
        GPIO_ResetBits(GPIOn, PINn);
    }
}
void io_debug_init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA_Msk, ENABLE);

//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_7);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_1);
//		GPIO_WriteBit(GPIOA, LED_LIGHT , Bit_RESET);
    GPIO_StructInit(&GPIO_InitStruct);
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2 ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	   GPIO_Init(GPIOA, &GPIO_InitStruct);

}
int main(void)
{
//	 /* Configure the system clock */

		u8 i;
//			led_init();
	#if LIMIT_SPEED_EN
	u16 speed_pi_fer_count=0;
	#endif
	Systick_Init(0x00ffffff);
	systick_delay_us(500);//等待触发ADC采样
	#if GRINDER_EN
		  Bsp_Adc_Init();
	    Board_ADC_Init();
			power_on_fly_machine_handle();
	#endif
//	while(1);
    Bsp_Gpio_Init();

    Peripheral_Init();
		user_para_init();

	 systick_delay_us(100000);//等待触发ADC采样
	Vcc_5V_CAL();
		CurrentOffsetCalibration();//放user_para_init()完后
		mose_check();	//放user_para_init()完后
		systick_delay_us(1000);
		read_user_data();//放user_para_init()完后
		mc_init();

		En_Port_init();
		GPIO_WriteBit(EN_PORT, EN_PIN , Bit_SET);
			led_init();

		Interrupt_Init();
		user_list.led_reuse_adc_flag=0;
		user_list.led_scan_start_flag=1;
	 systick_delay_us(5000);//等待触发ADC采样
		for(i=0;i<20;i++)  //在mc_init（）后，mc_init会先初始化方向
		{
			#if GRINDER_EN
			 user_list.flag.bits.gToolDir=Tool_Dir_CW_Value;
		   mc_core_list.mc_dir=user_list.flag.bits.gToolDir;
				
			#else
				user_direction_handle();
			#endif
		}
		systick_delay_us(1000);
		#if GRINDER_EN
		for(i=0;i<20;i++)  //在mc_init（）后，mc_init会先初始化方向
		{
				motor_on_off_control();
			
		}
		#endif
		Volt_Handler_fast();
		Volt_Handler();
		Volt_Handler();//上电时检测电压
		for(i=0;i<5;i++)
		{
			LedSoc_Run(user_list.VBus_Vx100,0,1);//电量初始化
			User_Temperature_Handler_Fast();
		}
		error_hold_updata();//更新错误
		#if LIMIT_SPEED_EN
		Speed_PI_init();
		Speed_PI_reset();
		#endif
//		mc_lose_step_init();
//		io_debug_init();
		user_list.gears_level=2;
    while(1)
    {
			 /*IWDG_ReloadCounter*/
        IWDG_RELOAD_COUNT();
			if(user_list.u8_1ms_flag)
			{
				
				user_list.u8_1ms_flag=0;
				#if !GRINDER_EN
				user_direction_handle();
				#endif
				Volt_Handler();
//				debug_motor_on_off();
				motor_on_off_control();
				user_oc_handle();
				User_Temperature_Handler();
				pwm_duty_aim_deal();
				
				#if(Wrench_EN)
				user_WrenchKey_Handler();
				#elif GRINDER_EN
				user_gears_for_grinder_handle();
				#else
				user_gears_handle();//
				#endif
				#if LIMIT_SPEED_EN

				if(motor_control_list.bldc_state==MOTOR_RUN&&mc_core_list.bldc_bemf_detected==1)
				{
						get_speed();
					 speed_pi_fer_count++;
					if(speed_pi_fer_count>10)
					{
						speed_pi_fer_count=0;
					Speed_Pid.Limit_max_data=motor_control_list.pwm_duty_virtual;

					Speed_Pid.Limit_min_data= (PWM_PRIOD_LOAD*0.15f) ;
					

					Speed_Pid.refValue=27000;			

					motor_control_list.pwm_duty_aim2=MC_PI_Handler(&Speed_Pid,motor_control_list.motor_speed);

					}
				}
				else 
				{
					Speed_PI_reset();
					motor_control_list.time_count_60_degree_filter=0;
					motor_control_list.motor_speed=0;
				}
				#endif
				user_state_control();
				error_hold_updata();
				UO_Led_Handler();
				if(user_list.flag.bits.gToolEn)
				{
					mc_core_list.motor_en=1;
				
				}
				else 
				{
					mc_core_list.motor_en=0;
				}
				if(mc_core_list.ipd_star_flag==1)
				{
				
					ipd_detect();
					mc_core_list.ipd_star_flag=0;
				}
				
				
			}

			
    }
}

/**
  * @}
*/

/**
  * @}
*/
