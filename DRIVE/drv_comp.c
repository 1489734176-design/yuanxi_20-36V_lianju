/** 
 * @file     drv_comp.c
 * @author   MindMotion Motor Team : Wesson
 * @brief    This file provides all the driver functions for the COMP.
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
#define _DRV_COMP_C_

/** Files includes */
#include "drv_comp.h"

/** 
 * @addtogroup MM32_Hardware_Driver_Layer
 * @{
 */

/** 
 * @addtogroup Drv_COMP
 * @{
 */

/**
 * @brief    : This function describes the underlying configuration of the comparator.
 * @param      sSelection : Comp number
 *             pCompInput : Comp struct,include NonInvertingInput/InvertingInput/CrvSelect
 * @retval   : None
 */
 
 uint8_t Comp_CH_Tab[6] =
{
	 COMP_CH_BEMF_W,       
   COMP_CH_BEMF_V,    
   COMP_CH_BEMF_U,   
	 COMP_CH_BEMF_W,       
   COMP_CH_BEMF_V,    
   COMP_CH_BEMF_U


};


 uint8_t Comp_CH_Tab1[6] =
{
	 COMP_CH_BEMF_V,
	 COMP_CH_BEMF_W,       
   COMP_CH_BEMF_U, 
   COMP_CH_BEMF_V, 
	 COMP_CH_BEMF_W,       
   COMP_CH_BEMF_U
};
 
void select_comp_ch_for_sector(u8 sector_index,u8 dir)
{
	if(dir==1)
	{
		setv(COMP2->COMPx_CSR,BS7,BW2,Comp_CH_Tab[sector_index]); 
	}
	else 
	{
		setv(COMP2->COMPx_CSR,BS7,BW2,Comp_CH_Tab1[sector_index]); 
	}
	if((sector_index&0x01)==0)//falling
	{
	     EXTI->RTSR &= ~EXTI_Line20;
       EXTI->FTSR &= ~EXTI_Line20;
		
			EXTI->FTSR |= EXTI_Line20;
	}
	else 
	{
			 EXTI->RTSR &= ~EXTI_Line20;
       EXTI->FTSR &= ~EXTI_Line20;
		
			 EXTI->RTSR  |= EXTI_Line20;
		
		
	}
}

void Drv_Comp_Init(COMP_TypeDef *comp,COMP_Input_TypeDef * pCompInput)
{
    COMP_InitTypeDef COMP_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_COMP_Msk, ENABLE);

    COMP_DeInit(comp);
    COMP_StructInit(&COMP_InitStructure);
    COMP_InitStructure.COMP_NonInvert = pCompInput->sCompNonInvertingInput;
    COMP_InitStructure.COMP_Invert    = pCompInput->sCompInvertingInput;
    COMP_InitStructure.COMP_Output            = COMP_Output_TIM1BKIN;
    COMP_InitStructure.COMP_OutputPol         = COMP_Pol_NonInvertedOut;
    COMP_InitStructure.COMP_Hysteresis        = COMP_Hysteresis_Medium;
    COMP_InitStructure.COMP_Mode              = COMP_Mode_Low1_Power;
    COMP_InitStructure.COMP_OFLT              = COMP_Filter_128_Period;   
  
    COMP_Init(comp, &COMP_InitStructure);   
  
    COMP_CrvCmd(comp,ENABLE);
  
  
    if (COMP_InvertingInput_IO3 == pCompInput->sCompInvertingInput)
    {
      COMP_SetCrv(comp,COMP_CRV_Src_VDDA,pCompInput->u8CompCrvSelect);
    }
    COMP_Cmd(comp, ENABLE);
}



/**
  * @}
*/

/**
  * @}
*/
