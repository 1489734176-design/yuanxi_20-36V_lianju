#include "board.h"
#include "led.h"

#include "user_control.h"
#include "parameter.h"
BITS_UNION ledData;
User_LED_Control_t user_led_control;
void LED1_BRIGHT(void);
void LED2_BRIGHT(void);
void LED3_BRIGHT(void);
void LED4_BRIGHT(void);
void LED5_BRIGHT(void);
void LED6_BRIGHT(void);

#define LED1_BRIGHT_MAP LED4_BRIGHT()
#define LED2_BRIGHT_MAP LED5_BRIGHT()
#define LED3_BRIGHT_MAP LED6_BRIGHT()
#define LED4_BRIGHT_MAP LED1_BRIGHT()
#define LED5_BRIGHT_MAP LED2_BRIGHT()
#define LED6_BRIGHT_MAP LED3_BRIGHT()


#define LED_ERROR_ON                	         {LED1 = LED2 = LED3 = led_on;}
#define LED_ERROR_OFF               	          {LED1 = LED2 = LED3 = led_off;}
//#define LED_ERROR_ON                	         { LED4 = led_on;}
//#define LED_ERROR_OFF               	         {LED4 = led_off;}

//#define LED_ERROR_ON                	         {	GPIO_WriteBit(GPIOA, LED_LIGHT , Bit_RESET);}
//#define LED_ERROR_OFF               	          {	GPIO_WriteBit(GPIOA, LED_LIGHT , Bit_SET);}
void led_init(void)
{
	


    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA_Msk|RCC_AHBENR_GPIOB_Msk, ENABLE);

//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_1);
//		GPIO_WriteBit(GPIOA, LED_LIGHT , Bit_RESET);
    GPIO_StructInit(&GPIO_InitStruct);
			GPIO_InitStruct.GPIO_Pin   = LED_PIN2|COM_PIN1|COM_PIN2|COM_PIN3 ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_FLOATING;
	   GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin=LED_PIN1;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
//		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
//		GPIO_InitStruct.GPIO_Pin=COM_PIN3;
//		GPIO_Init(COM_PIN3_PORT, &GPIO_InitStruct);
//    GPIO_StructInit(&GPIO_InitStruct);
//			GPIO_InitStruct.GPIO_Pin   = LED_PIN1 ;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
//    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
//	   GPIO_Init(LED_PIN1_PORT, &GPIO_InitStruct);
//		GPIO_WriteBit(COM_PIN3_PORT, COM_PIN3 , Bit_SET);

}

#if(0)
void  LED_PIN1_OUT(void) 	
{
	MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE13, 0x01U << GPIO_CRH_MODE13_Pos);
}
void LED_PIN2_OUT(void) 
{	
	MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE14, 0x01U << GPIO_CRH_MODE14_Pos);
}
void LED_PIN3_OUT(void) 
{
	MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE4, 0x01U << GPIO_CRL_MODE4_Pos);
}

void  LED_PIN1_IN(void) 
{
	MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE13, 0x00U << GPIO_CRH_MODE13_Pos);
}
void LED_PIN2_IN(void) 
{	
	MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE14, 0x00U << GPIO_CRH_MODE14_Pos);
}
void LED_PIN3_IN(void)  
{
	MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE4, 0x00U << GPIO_CRL_MODE4_Pos);
}

void LED_PIN1_HIGHT(void)
{	
	WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS13);
}
void  LED_PIN1_LOW(void)  
{	
	WRITE_REG(GPIOA->BRR, GPIO_BRR_BR13);
}
void LED_PIN2_HIGHT(void) 
{
	WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS14);
}
void LED_PIN2_LOW(void)   
{
	WRITE_REG(GPIOA->BRR, GPIO_BRR_BR14);
}
void LED_PIN3_HIGHT(void) 
{
	WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS4);
}
void LED_PIN3_LOW(void)  
{	
	WRITE_REG(GPIOA->BRR, GPIO_BRR_BR4);
}
#endif

#if 0
void LED1_BRIGHT(void)
{
		LED_PIN1_OUT();	
		LED_PIN2_OUT();
		LED_PIN3_IN();
	
		LED_PIN1_HIGHT();
		LED_PIN2_LOW();
}

void LED2_BRIGHT(void)
{
		LED_PIN1_OUT();	
		LED_PIN2_OUT();
		LED_PIN3_IN();
	
		LED_PIN1_LOW();
		LED_PIN2_HIGHT();

}


void LED3_BRIGHT(void)
{
		LED_PIN1_OUT();	
		LED_PIN2_IN();
		LED_PIN3_OUT();
		
		
		LED_PIN1_LOW();
	  LED_PIN3_HIGHT();
		
}


void LED4_BRIGHT(void)
{
		LED_PIN1_OUT();	
		LED_PIN2_IN();
		LED_PIN3_OUT();
		
		LED_PIN1_HIGHT();
		LED_PIN3_LOW();
		
}
	

void LED5_BRIGHT(void)
{
		LED_PIN1_IN();	
		LED_PIN2_OUT();
		LED_PIN3_OUT();
		
		LED_PIN2_HIGHT();
		LED_PIN3_LOW();
		
}

void LED6_BRIGHT(void)
{
		LED_PIN1_IN();	
		LED_PIN2_OUT();
		LED_PIN3_OUT();
	
		LED_PIN2_LOW();
		LED_PIN3_HIGHT();
	
}


void LED_Scan(void)
{
		static u8 led_scan_index=0;
		led_scan_index++;
		if(led_scan_index>6)
		{
			led_scan_index=1;
		}
		LED_PIN1_IN();
		LED_PIN2_IN();
		LED_PIN3_IN();
		switch(led_scan_index)
		{
			case 1:
				if(LED1)
				{
					LED1_BRIGHT_MAP;

				}
				break;
			case 2:
				if(LED2)
				{
					LED2_BRIGHT_MAP;
				}
				break;
			case 3:
				if(LED3)
				{
					LED3_BRIGHT_MAP;
				}
				break;
			case 4:
				if(LED4)
				{
					LED4_BRIGHT_MAP;
				}
				break;
			case 5:
				if(LED5)
				{
					LED5_BRIGHT_MAP;
				}
				break;
			case 6:
				if(LED6)
				{
					LED6_BRIGHT_MAP;
				}
				break;	
			default:
				break;
		
		}

}
#endif

#if 1

void LED1_BRIGHT(void)
{
		LED_PIN1_OUT();	
		COM_PIN1_OUT();
	
		LED_PIN1_HIGHT();
		COM_PIN1_LOW();
}

void LED2_BRIGHT(void)
{
		LED_PIN1_OUT();	
		COM_PIN2_OUT();
	
		LED_PIN1_HIGHT();
		COM_PIN2_LOW();

}


void LED3_BRIGHT(void)
{
		LED_PIN1_OUT();	
		COM_PIN3_OUT();
	
		LED_PIN1_HIGHT();
		COM_PIN3_LOW();
		
}


void LED4_BRIGHT(void)
{
		LED_PIN2_OUT();	
		COM_PIN1_OUT();
	
		LED_PIN2_HIGHT();
		COM_PIN1_LOW();
		
}
	

void LED5_BRIGHT(void)
{
		LED_PIN2_OUT();	
		COM_PIN2_OUT();
	
		LED_PIN2_HIGHT();
		COM_PIN2_LOW();
		
}

void LED6_BRIGHT(void)
{
		LED_PIN2_OUT();	
		COM_PIN3_OUT();
	
		LED_PIN2_HIGHT();
		COM_PIN3_LOW();
	
}

void LED_Scan(void)
{
		static u8 led_scan_index=0;
	if(user_list.led_scan_start_flag==0)
	{
//		user_list.Key_Data=GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN);
//		user_list.Dir_Data=GPIO_ReadInputDataBit(DIR_PORT,DIR_PIN);
//		user_list.Tmos_Adc=(uint16_t)mc_core_list.Board_ADC_DATA.u16NTC_MOS;
////				user_list.Tbat_Adc;
//		user_list.Vbus_Adc=	mc_core_list.Board_ADC_DATA.u16Vbus	;
		return;
	}
		led_scan_index++;
		if(led_scan_index>12)
		{
			led_scan_index=1;
		}
//		LED1_ON;
//		LED2_ON;
//		LED3_ON;
//		
//		LED4_ON;
//		LED5_ON;
//		LED6_ON;
		
//		if(	LED7)
//		{
//			LED_PIN3_LOW();
//		}
//		else 
//		{
//			LED_PIN3_HIGHT();
//		}
		if(led_scan_index<=6)
		{
			LED_PIN1_IN();
			LED_PIN2_IN();
			LED_PIN1_AIN();//AIN状态，变为输出时为推挽输出
			LED_PIN2_AIN();
//			COM_PIN1_IN();
//			COM_PIN2_IN();
//			COM_PIN3_IN();
			
			COM_PIN1_HIGHT();
			COM_PIN2_HIGHT();
			COM_PIN3_HIGHT();
			
;
//			COM_PIN1_AIN();
//			COM_PIN2_AIN();
//			COM_PIN3_AIN();	
			
			COM_PIN1_OUT();
			COM_PIN2_OUT();
			COM_PIN3_OUT();
			
		}
		switch(led_scan_index)
		{
			case 1:
				if(LED1)
				{
					LED1_BRIGHT_MAP;

				}
				
//				if(	LED7)
//				{
//					LED_PIN3_LOW();
//				}
				break;
			case 2:
				if(LED2)
				{
					LED2_BRIGHT_MAP;
				}
				break;
			case 3:
				if(LED3)
				{
					LED3_BRIGHT_MAP;
				}
				break;
			case 4:
				if(LED4)
				{
					LED4_BRIGHT_MAP;
				}
				break;
			case 5:
				if(LED5)
				{
					LED5_BRIGHT_MAP;
				}
				break;
			case 6:
				if(LED6)
				{
					LED6_BRIGHT_MAP;
				}
				break;	
			case 7://注意bat_NTC脚转AIN前为高状态
				

					LED_PIN1_IN();
					LED_PIN2_IN();
	;
					COM_PIN1_IN();
					COM_PIN2_IN();
					COM_PIN3_IN();
			
					LED_PIN1_AIN();//AIN状态，变为输出时为推挽输出
					LED_PIN2_AIN();
			
				  COM_PIN1_AIN();
					COM_PIN2_AIN();
					COM_PIN3_AIN();
				break;
				case 8:
					break;
				case 9:
					user_list.led_reuse_adc_flag=1;
//				user_list.Key_Data=GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN);
//				user_list.Dir_Data=GPIO_ReadInputDataBit(DIR_PORT,DIR_PIN);
//				user_list.Tmos_Adc=(uint16_t)mc_core_list.Board_ADC_DATA.u16NTC_MOS;
////				user_list.Tbat_Adc;
//				user_list.Vbus_Adc=	mc_core_list.Board_ADC_DATA.u16Vbus	;
				break;
				case 10:
					break;
				case 11:
					user_list.led_reuse_adc_flag=0;
					COM_PIN1_HIGHT();
					COM_PIN2_HIGHT();
					COM_PIN3_HIGHT();
					COM_PIN1_OUT();
					COM_PIN2_OUT();
					COM_PIN3_OUT();
					COM_PIN1_HIGHT();
					COM_PIN2_HIGHT();
					COM_PIN3_HIGHT();
				
					LED_PIN1_IN();
					LED_PIN2_IN();
					LED_PIN1_IPU();//AIN状态，变为输出时为推挽输出
					LED_PIN2_IPU();
			

					break;		
				case 12:
				user_list.Key_Data=GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN);
				user_list.Dir_Data=GPIO_ReadInputDataBit(DIR_PORT,DIR_PIN);
				LED_PIN1_AIN();//AIN状态，变为输出时为推挽输出
				LED_PIN2_AIN();
				LED_PIN1_LOW();
				LED_PIN2_LOW();
				LED_PIN1_OUT();
				LED_PIN2_OUT();
					break;
			default:
				break;
		
		}

}


#endif



void UO_Led_Handler(void)
{	


	if(user_list.user_state == TOOL_POWER_DOWN)
	{
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;
		LED5_OFF;
		LED6_OFF;
		return;
	}



	
	{
		// 异常显示
		if(LedError_Control()&&user_list.flag.bits.gToolTrigger)
		{
			LedError_Run();
		}  
		// 电量显示
		else
		{		
//					GPIO_WriteBit(GPIOA, LED_LIGHT , Bit_RESET);
		  user_led_control.LedDeal=0;
			user_led_control.LedRunTimes=0;
			LedSoc_Run(user_list.VBus_Vx100,1,0);

		}
		#if(Wrench_EN)
		if(mc_core_list.mc_dir==Tool_Dir_CW_Value)
		{
			if(user_list.gears_level==0)
			{
				LED4_ON;
				LED5_OFF;
				LED6_OFF;
			}
			else if(user_list.gears_level==1)
			{
				LED4_ON;
				LED5_ON;
				LED6_OFF;
			
			}
			else
			{
				LED4_ON;
				LED5_ON;
				LED6_ON;
			}
		}
		else 
		{
			LED4_OFF;
			LED6_OFF;
			if(user_list.flag.bits.gAutoStopEn)
			{
			
				LED5_ON;
			}
			else
			{
				LED5_OFF;
			}
		}
		

		//
		#elif GRINDER_EN
			if(user_list.gears_level==0)
			{
				LED4_OFF;
				LED5_OFF;
				LED6_ON;
			}
			else if(user_list.gears_level==1)
			{
				LED4_OFF;
				LED5_ON;
				LED6_ON;
			
			}
			else 
			{
				LED4_ON;
				LED5_ON;
				LED6_ON;
			
			}
		#else
		//LED显示
		LED4_OFF;
		LED5_OFF;
		LED6_OFF;

					
			
		#endif
		
		
	}


	

	
}


/***********************************************************************************************************************
* Function Name:
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
uint8_t LedError_Control(void)
{	
	uint8_t ret = 0;
	#if(RESTAR_EN)
if((machine_error_hold.userErr_hold.word==0) &&( (machine_error_hold.MC_error_hold.word&0xfffb) == 0)&&\
	(machine_error_hold.MC_error_hold.bits.gBlock==0||(user_list.Load_exist<RESTAR_COUNT)))
	#else 
	if((machine_error_hold.userErr_hold.word==0) &&( (machine_error_hold.MC_error_hold.word) == 0))
	#endif
	{
		//LED_ERROR_OFF;
//		PORT_SetBit(PORT2,PIN3);
		user_led_control.LedRunCnt_Once = 0;
		user_led_control.LedDeal = 0;		
		user_led_control.LedRunTimes = 0;
		return 0;
	}
        
	if(machine_error_hold.userErr_hold.word || machine_error_hold.MC_error_hold.word) 
	{	
		if((0 == user_led_control.LedDeal) &&(!user_led_control.LedRunTimes))
		{
			user_led_control.LedDeal = 1;
			user_led_control.LedRunTimes = 10;
			#if 0
			if(machine_error_hold.MC_error_hold.word)
			{
				/*n个电机异常排列先gIPDOCP 1闪,gLackPhase 2闪*/
	            for( i=0;i<16;i++ )  
	            {
	                if(machine_error_hold.MC_error_hold.word&(0x0001<<i))
	                {
	                    user_led_control.LedRunCnt_Once = i+1;
	                }
	            }
	            user_led_control.LedRunMode = 0;
			}
			else
			{
				/*输入异常排在电机异常后计数*/
				/*gIBusAcmpOCP 5闪*/
				for( i=0;i<24;i++ )
	            {
	                if(machine_error_hold.userErr_hold.word &(0x0001<<i))
	                {
	                    user_led_control.LedRunCnt_Once = i+1;
	                }
	            }
	            user_led_control.LedRunMode = 1;
			}
			#endif
			#if(1)
			if(machine_error_hold.userErr_hold.bits.gIBusAcmpOCP)
			{
			
				user_led_control.LedRunCnt_Once=1;
				user_led_control.LedRunMode = 1;
			}
			else if(machine_error_hold.userErr_hold.bits.gNtcBat)
			{
				user_led_control.LedRunCnt_Once=2;
				user_led_control.LedRunMode = 1;
			
			}
			else if(machine_error_hold.userErr_hold.bits.gNtcMos==1||machine_error_hold.userErr_hold.bits.gMos_overload)
			{
				
				user_led_control.LedRunCnt_Once=3;
				user_led_control.LedRunMode = 1;
			}
			else if(machine_error_hold.MC_error_hold.bits.gBlock)
			{
				user_led_control.LedRunCnt_Once=4;
				user_led_control.LedRunMode = 1;
			}	
			else if(machine_error_hold.userErr_hold.word&0x00000001e)
			{
				user_led_control.LedRunCnt_Once=5;
				user_led_control.LedRunMode = 1;
			
			
			}
			else if(machine_error_hold.userErr_hold.bits.gVBusStbUVP||machine_error_hold.userErr_hold.bits.gVBusRunUVP1||machine_error_hold.userErr_hold.bits.gVBusRunUVP2)
			{
			
				user_led_control.LedRunCnt_Once=6;
				user_led_control.LedRunMode = 1;
			}
			else if(machine_error_hold.userErr_hold.bits.fly_machine)
			{
			
				user_led_control.LedRunCnt_Once=7;
				user_led_control.LedRunMode = 1;
			}	

			else if(machine_error_hold.userErr_hold.bits.gVBusOVP)
			{
					
				user_led_control.LedRunCnt_Once=8;
				user_led_control.LedRunMode = 1;
			
			}
			else if(machine_error_hold.userErr_hold.bits.gMosBridgeH||machine_error_hold.userErr_hold.bits.gMosBridgeL)
			{
				user_led_control.LedRunCnt_Once=9;
				user_led_control.LedRunMode = 1;
			
			}
			else if(machine_error_hold.MC_error_hold.bits.gLackPhase)
			{
				user_led_control.LedRunCnt_Once=10;
				user_led_control.LedRunMode = 1;
			
			}	
			else if(machine_error_hold.MC_error_hold.bits.gLosStep)
			{
				user_led_control.LedRunCnt_Once=11;
				user_led_control.LedRunMode = 1;
			
			}	
			else 
			{
				user_led_control.LedRunCnt_Once=12;
				user_led_control.LedRunMode = 1;
			}
			
			#endif      


			
		}
		
		
        
        if(user_led_control.LedRunCnt_Once)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
		
	}
	
	return ret;
	
}


/***********************************************************************************************************************
* Function Name:
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
uint16_t deltaVolt = 0.2*100;
void LedError_Run(void)
{
	static uint16_t bzCnt = 0;
	static uint16_t bzTime = 0; 

	if(user_led_control.LedDeal)
	{
		LED_ERROR_OFF;

		bzCnt = user_led_control.LedRunCnt_Once;
		bzTime = 0;
		user_led_control.LedDeal = 0;		
	}
	
	if(!user_led_control.LedRunTimes)
	{
		LED_ERROR_OFF;

		bzCnt = 0;
		user_led_control.LedRunTimes = 0;
		bzTime = 0;
		return;
	}
	else
	{		
		if(bzCnt)
		{
			bzTime++;
			if((bzTime>=1)&&(bzTime <=500))   //200
			{
				LED_ERROR_OFF;

			}
							
			else if(bzTime < 700)
			{				
				LED_ERROR_ON;

			}
			
			if(user_led_control.LedRunMode == 0)
			{
				if((bzTime>=700)&&(bzTime <=900))   //200
				{
					LED_ERROR_OFF;

				}			
				else if((bzTime>=900)&&(bzTime <=1100))
				{				
					LED_ERROR_ON;

				}

				if(bzTime > 1100)
				{	
					bzTime = 0;
					bzCnt--;
				}
			}
			else
			{
				if(bzTime > 700)
				{	
					bzTime = 0;
					bzCnt--;
				}	
			}


		}
		else
		{		
			LED_ERROR_OFF;
			if(bzTime ++>= 1500)
			{
				bzTime = 0;	
				bzCnt = user_led_control.LedRunCnt_Once;
				user_led_control.LedRunTimes--;
			}			
		}
	}
}
void LedSoc_Run(uint32_t volt_vx100,uint8_t display_en,uint8_t fast_mode)
{
	static uint16_t ms500cnt = 0;
	static uint16_t msUpdateCnt = 0;
	static uint8_t PackSocStatus = 0;

//10次滤波增加稳定性

	ms500cnt++;
 
	if(user_led_control.gSocUpdate||fast_mode)
	{	
		if(PackSocStatus == 0)
		{
			PackSocStatus = 1;
		}
		
		switch(PackSocStatus)
		{
			case 4:
		            
				if(volt_vx100< (SOC3_MV -deltaVolt ))
				{
					PackSocStatus = 3;
				}
			
				break;
		        
			case 3:
		        if(volt_vx100> (SOC3_MV  ))
		        {
		            PackSocStatus = 4;
		        }
		        if(volt_vx100< (SOC2_MV -deltaVolt ))
		        {
		            PackSocStatus = 2;
		        }
		        break;
		        
			case 2:
		        if(volt_vx100> (SOC2_MV  ))
		        {
		            PackSocStatus = 3;
		        }
		        if(volt_vx100< (SOC1_MV -deltaVolt ))
		        {
		            PackSocStatus = 1;
		        }
		        break;
		        
			case 1:
		        if(volt_vx100> (SOC1_MV  ))
		        {
		            PackSocStatus = 2;
		        }

		        break;	             
		}
		
		user_led_control.gSocUpdate = 0;
		
	}	
	else
	{
		
		if(user_list.user_state!=TOOL_RUN)
		{
			msUpdateCnt++;
		}
		else 
		{
			msUpdateCnt=0;
		}
		if(msUpdateCnt> 20)
		{
				user_led_control.gSocUpdate = 1;
		    msUpdateCnt =0;
		}
//		msUpdateCnt++;
//		if(msUpdateCnt> 500)
//		{
//		    user_led_control.gSocUpdate = 1;
//		    msUpdateCnt =0;
//		}
	}
	
	if(ms500cnt >= 500)  //500ms
	{
		ms500cnt  = 0;
	}
	
	
/*soc display*/	
	if(display_en == 0)
	{
		LED3_OFF;
		LED2_OFF;
		LED1_OFF;	
		
		return;	        
	}
	
	switch(PackSocStatus)
	{				
		case 4 :
		
		    LED1_ON;
		    LED2_ON;
				LED3_ON;

		break;
		
		case 3 :
		
		    LED1_ON;
		    LED2_ON;
			LED3_OFF;

		break;
		
		case 2 :
		    LED1_ON;
		    LED2_OFF;
			LED3_OFF;

		break;
		
		case 1 :
		
				if(ms500cnt == 250)
				{
					LED1_FLASH;                
				}
		        LED2_OFF;
		        LED3_OFF;

		break;
		
		case 0 :
		
		        LED1_OFF;
		        LED2_OFF;
		        LED3_OFF;

		break;
                
		default :
		break;	
	
	}
}
