/**
 * @file     board.c
 * @author   MindMotion Motor Team : Wesson
 * @brief    This file provides all the functions for the board level support package.
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
#define _BOARD_C_

/** Files includes */
#include "board.h"
#include "drv_inc.h"
#include "parameter.h"
#include "led.h"
/**
 * @addtogroup MM32_Hardware_Driver_Layer
 * @{
 */

/**
 * @addtogroup Bsp
 * @{
 */

/**
  * @brief OPAM  GPIO Initialization 
  * @param None
  * @retval None
  */
void Bsp_Op_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
	
    /* GPIO Ports Clock Enable */
    RCC_AHBPeriphClockCmd(OPAMP_GPIO_CLK, ENABLE);

    /*Configure GPIO pin : OPAMP1_Pin */
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AIN;
	
	  GPIO_InitStructure.GPIO_Pin     = OPAMP1_OUT_PIN;
    GPIO_Init(OPAMP1_OUT_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin     = OPAMP1_INM_PIN;
    GPIO_Init(OPAMP1_INM_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin     = OPAMP1_INP_PIN;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AIN;

    GPIO_Init(OPAMP1_INP_PORT, &GPIO_InitStructure);
}
/**
  * @brief ADC GPIO Initialization 
  * @param None
  * @retval None
  */

void Bsp_Adc_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_AHBPeriphClockCmd(ADC_GPIO_CLK, ENABLE);			// OK

    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AIN;

    GPIO_InitStructure.GPIO_Pin     = VR_PIN;
    GPIO_Init(VR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin     = VBUS_PIN;
    GPIO_Init(VBUS_PORT, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin     = ISUMAVG_PIN;
    GPIO_Init(ISUMAVG_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin     = IRMS_PIN;
    GPIO_Init(IRMS_PORT, &GPIO_InitStructure);
	
//	   GPIO_InitStructure.GPIO_Pin     = DIR_PIN;
//    GPIO_Init(DIR_PORT, &GPIO_InitStructure);
		
		  GPIO_InitStructure.GPIO_Pin     = T_MOS_PIN;
    GPIO_Init(T_MOS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin     = BEMF_U_PIN;
    GPIO_Init(BEMF_U_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin     = BEMF_V_PIN;
    GPIO_Init(BEMF_V_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin     = BEMF_W_PIN;
    GPIO_Init(BEMF_W_PORT, &GPIO_InitStructure);
//		
//	  GPIO_InitStructure.GPIO_Pin     = FLY_MACHINE_PIN;
//    GPIO_Init(FLY_MACHINE_PORT, &GPIO_InitStructure);
		
}

void Bsp_Comp_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
		/* COMP Clock Enable */
    RCC_AHBPeriphClockCmd((COMP_GPIO_CLK), ENABLE);

		/*Configure GPIO pin : COMP_Pin */
    GPIO_InitStructure.GPIO_Pin = COMP_INP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_Init(COMP_INP_PORT, &GPIO_InitStructure);
}

/**
  * @brief Debug IO Initialization 
  * @param None
  * @retval None
  */
void Bsp_Debug_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    RCC_AHBPeriphClockCmd(DEBUG_IO_RCC_CLOCKGPIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin     =  DEBUG_IO1_PIN;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_Init(DEBUG_IO1_PORT, &GPIO_InitStructure);
}

//void Bsp_LED_Init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_StructInit(&GPIO_InitStructure);
//    RCC_AHBPeriphClockCmd(LED_RCC_CLOCKGPIO, ENABLE);

//    GPIO_InitStructure.GPIO_Pin     =  LED1_PIN;
//    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_High;
//    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
//    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
//}

/**
  * @brief Key GPIO Initialization 
  * @param None
  * @retval None
  */
void Bsp_Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
	
		/* LED Clock Enable */
    RCC_AHBPeriphClockCmd(KEY_RCC_CLOCKGPIO, ENABLE);		// OK
	
		/*Configure GPIO pin : LED_Pin */
		GPIO_InitStructure.GPIO_Pin     =  KEY1_PIN;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);	
}

void En_Port_init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

	
	
			/* LED Clock Enable */
    RCC_AHBPeriphClockCmd(EN_PIN_RCC_CLOCKGPIO, ENABLE);		// OK
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_1);
		/*Configure GPIO pin : LED_Pin */
		GPIO_InitStructure.GPIO_Pin     =  EN_PIN;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_Init(EN_PORT, &GPIO_InitStructure);	
		GPIO_SetBits(EN_PORT, EN_PIN);
}
void Bsp_dir_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

	
	
			/* LED Clock Enable */
    RCC_AHBPeriphClockCmd(DIR_RCC_CLOCKGPIO, ENABLE);		// OK
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_1);
		/*Configure GPIO pin : LED_Pin */
		GPIO_InitStructure.GPIO_Pin     =  DIR_PIN;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
    GPIO_Init(DIR_PORT, &GPIO_InitStructure);	


}
/**
  * @brief PWM GPIO Initialization 
  * @param None
  * @retval None
  */

void Bsp_Pwm_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
	
		/* PWM Clock Enable */
    RCC_AHBPeriphClockCmd(BLDC1_GPIO_CLK, ENABLE);
	
		/*selects the pin to used as Alternate function of PWM*/
    GPIO_PinAFConfig(BLDC1_UH_PORT, BLDC1_UH_PIN_SRC, BLDC1_UH_PIN_AF);
    GPIO_PinAFConfig(BLDC1_VH_PORT, BLDC1_VH_PIN_SRC, BLDC1_VH_PIN_AF);
    GPIO_PinAFConfig(BLDC1_WH_PORT, BLDC1_WH_PIN_SRC, BLDC1_WH_PIN_AF);

    GPIO_PinAFConfig(BLDC1_UL_PORT, BLDC1_UL_PIN_SRC, BLDC1_UL_PIN_AF);
    GPIO_PinAFConfig(BLDC1_VL_PORT, BLDC1_VL_PIN_SRC, BLDC1_VL_PIN_AF);
    GPIO_PinAFConfig(BLDC1_WL_PORT, BLDC1_WL_PIN_SRC, BLDC1_WL_PIN_AF);	

		/*Configure GPIO pin : PWM_Pin */
    GPIO_InitStructure.GPIO_Pin     = BLDC1_UH_PIN;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
    GPIO_Init(BLDC1_UH_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BLDC1_VH_PIN;
    GPIO_Init(BLDC1_VH_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BLDC1_WH_PIN;
    GPIO_Init(BLDC1_WH_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BLDC1_UL_PIN;
    GPIO_Init(BLDC1_UL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BLDC1_VL_PIN;
    GPIO_Init(BLDC1_VL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BLDC1_WL_PIN;
    GPIO_Init(BLDC1_WL_PORT, &GPIO_InitStructure);	
}

/**
  * @brief Initialize all configured GPIO
  * @param None
  * @retval None
  */
void Bsp_Gpio_Init(void)
{
    Bsp_Op_Init();
		Bsp_Comp_Init();
    Bsp_Adc_Init();
		Bsp_Pwm_Init();
//		led_init();

//		Bsp_LED_Init();
//		LED1_ON();
//		Bsp_dir_init();
//		Bsp_Key_Init();
}
/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
void Board_ADC_Init(void)
{
    /*ADC  RANK Array*/
    ADC_Channel_TypeDef sUserAdc1Channel[9];

    /* Configure the ADC RANK Sequence*/
    sUserAdc1Channel[0].u8Rank 			= BEMF_U_RANK;
    sUserAdc1Channel[0].sAdcChannel = BEMF_U_CHANNEL;
    sUserAdc1Channel[0].pNext 			= &sUserAdc1Channel[1];
		
    sUserAdc1Channel[1].u8Rank 			= BEMF_V_RANK;
    sUserAdc1Channel[1].sAdcChannel = BEMF_V_CHANNEL;
    sUserAdc1Channel[1].pNext 			= &sUserAdc1Channel[2];
		
    sUserAdc1Channel[2].u8Rank 			= BEMF_W_RANK;
    sUserAdc1Channel[2].sAdcChannel = BEMF_W_CHANNEL;
    sUserAdc1Channel[2].pNext 			= &sUserAdc1Channel[3];
		
    sUserAdc1Channel[3].u8Rank 			= OP_O_RANK;
    sUserAdc1Channel[3].sAdcChannel = OP_O_CHANNEL;
    sUserAdc1Channel[3].pNext 			= &sUserAdc1Channel[4];
		
    sUserAdc1Channel[4].u8Rank 			= ISUMAVG_RANK;
    sUserAdc1Channel[4].sAdcChannel = ISUMAVG_CHANNEL;
    sUserAdc1Channel[4].pNext 			= &sUserAdc1Channel[5];

    sUserAdc1Channel[5].u8Rank 			= VR_RANK ;
    sUserAdc1Channel[5].sAdcChannel = VR_CHANNEL;
    sUserAdc1Channel[5].pNext 			= &sUserAdc1Channel[6];
		
    sUserAdc1Channel[6].u8Rank 			= VBUS_RANK;
    sUserAdc1Channel[6].sAdcChannel = VBUS_CHANNEL ;
    sUserAdc1Channel[6].pNext =  &sUserAdc1Channel[7];
		
		
		sUserAdc1Channel[7].u8Rank 			= DIR_RANK;
    sUserAdc1Channel[7].sAdcChannel = DIR_CHANNEL ;
    sUserAdc1Channel[7].pNext =  &sUserAdc1Channel[8];
		
		
		sUserAdc1Channel[8].u8Rank 			= T_MOS_RANK;
    sUserAdc1Channel[8].sAdcChannel = T_MOS_CHANNEL ;
    sUserAdc1Channel[8].pNext =NULL;
		
		     /* Select the ADC sample time*/
    Drv_Adc_Channel_Init(ADC1, sUserAdc1Channel, ADC_SampleTime_3_5); 
    /* Select the ADC external trigger source of the ADC is T1_CC4*/
    Drv_Adc_Basic_Init(ADC1, ADC_ExtTrig_T1_CC4);

 
}

volatile COMP_TypeDef *pComp;

void Board_Comp_Init(void)
{
    COMP_Input_TypeDef sUserCompInput;
	  /* Select the inverting input of the comparator */
    sUserCompInput.sCompInvertingInput = COMP_INVERTING;
	  /* Select the non inverting input of the comparator*/
    sUserCompInput.sCompNonInvertingInput = COMP_NON_INVERTING;
	  /* Select comparator external reference voltage */
    sUserCompInput.u8CompCrvSelect = COMP_CRV_VOLTAGE_SELECT;
	  /* Initializes the COMP according to the specified parameters in the COMP_Input_TypeDef */
    Drv_Comp_Init(COMP_NUMBER,&sUserCompInput);
}

/**
* @brief    : This function describes the underlying configuration of the op-amp.
* @param    : None
* @retval   : None
*/
void Board_Opamp_Init(void)
{
	 /* op-amp Clock Enable */
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_OPA1_Msk, ENABLE);
    /*Enable the specified OPAMP peripheral*/
    OPAMP_Cmd(OPAMP1,ENABLE);
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
void Drv_TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		/* TIM2 Clock Enable */
		RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2_Msk, ENABLE); //Note: TIM2 is a 32-bit up-counter/down-counter

		/* Configure TIM2 */
    TIM_TimeBaseStructure.TIM_Period = 0x000FFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CR1_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM2, DISABLE);
    TIM_UpdateDisableConfig(TIM2, DISABLE);
    TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Regular);
		/* Clear Update flag */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		/* Reset cnt */
    TIM_SetCounter(TIM2, 0); 
		/* Enable TIM2 */
    TIM_Cmd(TIM2, ENABLE);
}

///**
//  * @brief TIM3 Initialization Function
//  * @param None
//  * @retval None
//  */
void Drv_TIM13_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
		/* TIM3 Clock Enable */
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM13_Msk, ENABLE); 

		TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 		// 0xFFFF
		TIM_TimeBaseStructure.TIM_Prescaler = 119; 		// 2us 计数一次
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CR1_CKD_DIV1; 			
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);	
	
		/* Enable TIM3 */
    TIM_Cmd(TIM13, ENABLE);
}


void Drv_TIM14_Init_1(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
		/* TIM3 Clock Enable */
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM14_Msk, ENABLE); 

		TIM_TimeBaseStructure.TIM_Period = 0xffff; 		// 0xFFFF
		TIM_TimeBaseStructure.TIM_Prescaler = 7; 		// 0.5us 计数一次
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CR1_CKD_DIV1; 			
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);	
	
		/* Enable TIM4 */
    TIM_Cmd(TIM14, ENABLE);
}

void Drv_TIM6_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
		/* TIM3 Clock Enable */
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6_Msk, ENABLE); 

		TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 		// 0xFFFF
		TIM_TimeBaseStructure.TIM_Prescaler = 119; 		// 2us 计数一次
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CR1_CKD_DIV1; 			
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);	
	
		/* Enable TIM4 */
    TIM_Cmd(TIM6, DISABLE);
}





/**
  * @brief COMP Sensorless BEMF Initialization Function
  * @param None
  * @retval None
  */

void Board_SNS_Comp_Init(void)
{
	   EXTI_InitTypeDef EXTI_InitStruct;
		 NVIC_InitTypeDef NVIC_InitStruct;
	COMP_InitTypeDef COMP_InitStructure;
	/* COMP Clock Enable */

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	
	/* COMP Clock Enable */
	RCC_APB1PeriphClockCmd((RCC_APB1ENR_COMP_Msk), ENABLE);
	
	
	/* Configure COMP2 */
	COMP_DeInit(COMP2); 
//	COMP_InitStructure.COMP_Output    	= COMP_Output_TIM14IC1;
	COMP_StructInit(&COMP_InitStructure);
//	COMP_InitStructure.COMP_Output    	= COMP_Output_TIM14IC1;
	COMP_InitStructure.COMP_Output =	COMP_Output_None;
	COMP_InitStructure.COMP_OutputPol 	= COMP_Pol_NonInvertedOut;			
	COMP_InitStructure.COMP_Hysteresis 	= COMP_Hysteresis_High; 	
	COMP_InitStructure.COMP_Mode       	= COMP_Mode_Low2_Power;
	COMP_InitStructure.COMP_OFLT      	= COMP_Filter_128_Period; 
	COMP_InitStructure.COMP_Invert=COMP_InvertingInput_IO1;
	COMP_InitStructure.COMP_NonInvert=COMP_NonInvertingInput_IO1;
	COMP_InitStructure.COMP_OutAnaSel=COMP_AnalogOutput_Sync;
	COMP_Init(COMP2, &COMP_InitStructure);
	COMP2->COMPx_CSR&=(~(0x0000000F<<25));
	COMP2->COMPx_CSR|=((0x00000009<<25));
	/* Selct COMP2 INP & INM */
	SELECT_COMP2_INP_AS_BEMF_V();     
	SELECT_COMP2_INM_AS_BEMF_O();    	

	/* Enable COMP2 */
	COMP_Cmd(COMP2, ENABLE);	
	
	
	 

//    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA_Msk, ENABLE);

//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
// 

//    GPIO_StructInit(&GPIO_InitStruct);
//    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2 ;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
//    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_EXTI, ENABLE);

    EXTI_StructInit(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Line    = EXTI_Line20;
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = COMP1_2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief COMP sensorless mode GPIO Initialization 
  * @param None
  * @retval None
  */
void Bsp_Comp_Sensorless_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_StructInit(&GPIO_InitStructure);
		/* COMP Clock Enable */
		RCC_AHBPeriphClockCmd((COMP_SNS_GPIO_CLK), ENABLE);
	
		/*Configure BEMF pin  */
		GPIO_InitStructure.GPIO_Pin  = COMP_BEMF_U;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
		GPIO_Init(COMP_BEMF_U_PORT, &GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin  = COMP_BEMF_V;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
		GPIO_Init(COMP_BEMF_V_PORT, &GPIO_InitStructure);		

		GPIO_InitStructure.GPIO_Pin  =  COMP_BEMF_W;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
		GPIO_Init(COMP_BEMF_W_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin  =  COMP_BEMF_COM;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
		GPIO_Init(COMP_BEMF_COM_PORT, &GPIO_InitStructure);
}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */

//void Drv_TIM14_Init(uint32_t period)
//{
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		/* TIM14 Clock Enable */
//		RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM14_Msk, ENABLE);
//		
//		/* Configure TIM14 */
//		TIM_TimeBaseStructure.TIM_Period = period; 
//		TIM_TimeBaseStructure.TIM_Prescaler = 0; 	
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CR1_CKD_DIV1; 
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//		TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);	
//		/* Clear Update flag */
//		TIM_ClearFlag(TIM14, TIM_FLAG_Update);	
//		/* Reset cnt */
//		TIM_SetCounter(TIM14, 0); 
//	
//		if(Motor_1st.SNLS.u8SNLS_mode_Flag == 1)
//		{
//			/* Enable TIM14 */
//			TIM_Cmd(TIM14, ENABLE);		
//		}
//		else
//		{
//			TIM_Cmd(TIM14, DISABLE);		
//		}
//}

/**
  * @brief Initialize all configured peripherals
  * @param None
  * @retval None
  */
void Peripheral_Init(void)
{
    Board_ADC_Init();
    Board_Comp_Init();
    Board_Opamp_Init();
	  Drv_Pwm_Init(TIM1, PWM_PRIOD_LOAD, DEADTIME_LOAD);
	
		Drv_TIM13_Init();
		Drv_TIM14_Init_1();
//		Drv_TIM6_Init();
		/* Configure COMP BEMF Detect*/
		Bsp_Comp_Sensorless_Init();	
		Board_SNS_Comp_Init();
	
		/* Configure TIM14 */
//		Drv_TIM14_Init(Motor_1st.BLDC_CMP.u32TIM14Period);	
	
		/*Initialize WDT*/
//    Drv_Iwdg_Init();
	  /*Initialize divider*/
//    Drv_Hwdiv_Init();
}


/**
  * @}
*/

/**
  * @}
*/
