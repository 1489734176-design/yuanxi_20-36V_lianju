#ifndef __LED_H
#define __LED_H
#include <stdio.h>
#include "mm32_device.h"
#include "hal_conf.h"
#define LED_RCC_CLOCKGPIO          (RCC_AHBENR_GPIOA_Msk)
#define LED_PIN1_PORT                   GPIOB
#define LED_PIN1                    GPIO_Pin_8

#define LED_PIN2_PORT                   GPIOA
#define LED_PIN2                    GPIO_Pin_14

#define LED_PIN3_PORT                   GPIOA
#define LED_PIN3                   GPIO_Pin_10

#define LED_LIGHT_PORT                   GPIOA
#define LED_LIGHT                   GPIO_Pin_10

#define COM_PIN1_PORT                   GPIOA
#define COM_PIN1                    GPIO_Pin_3

#define COM_PIN2_PORT                  GPIOA
#define COM_PIN2                    GPIO_Pin_4

#define COM_PIN3_PORT                  GPIOA
#define COM_PIN3                    GPIO_Pin_10

#define COM_PIN4_PORT                  GPIOA
#define COM_PIN4                    GPIO_Pin_2

#if(0)
#define LED_PIN1_OUT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE13, 0x01U << GPIO_CRH_MODE13_Pos)
#define LED_PIN2_OUT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE14, 0x01U << GPIO_CRH_MODE14_Pos)
#define LED_PIN3_OUT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE10, 0x01U << GPIO_CRH_MODE10_Pos)

#define LED_PIN1_IN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE13, 0x00U << GPIO_CRH_MODE13_Pos)
#define LED_PIN2_IN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE14, 0x00U << GPIO_CRH_MODE14_Pos)
#define LED_PIN3_IN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE10, 0x00U << GPIO_CRH_MODE10_Pos)

#define LED_PIN1_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS13)
#define LED_PIN1_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR13)

#define LED_PIN2_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS14)
#define LED_PIN2_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR14)

#define LED_PIN3_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS10)
#define LED_PIN3_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR10)
#endif

#if(1)

#define LED_PIN1_OUT()  MODIFY_REG(GPIOB->CRH, GPIO_CRH_MODE8, 0x01U << GPIO_CRH_MODE8_Pos)
#define LED_PIN3_OUT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE13, 0x01U << GPIO_CRH_MODE13_Pos)
#define LED_PIN2_OUT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE14, 0x01U << GPIO_CRH_MODE14_Pos)

#define COM_PIN1_OUT()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE3, 0x01U << GPIO_CRL_MODE3_Pos)
#define COM_PIN2_OUT()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE4, 0x01U << GPIO_CRL_MODE4_Pos)
#define COM_PIN3_OUT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE10, 0x01U << GPIO_CRH_MODE10_Pos)
#define COM_PIN4_OUT()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE2, 0x01U << GPIO_CRL_MODE2_Pos)

#define LED_PIN1_IN()  MODIFY_REG(GPIOB->CRH, GPIO_CRH_MODE8, 0x00U << GPIO_CRH_MODE8_Pos)
#define LED_PIN3_IN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE13, 0x00U << GPIO_CRH_MODE13_Pos)
#define LED_PIN2_IN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE14, 0x00U << GPIO_CRH_MODE14_Pos)

#define COM_PIN1_IN()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE3, 0x00U << GPIO_CRL_MODE3_Pos)
#define COM_PIN2_IN()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE4, 0x00U << GPIO_CRL_MODE4_Pos)
#define COM_PIN3_IN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE10, 0x00U << GPIO_CRH_MODE10_Pos)
#define COM_PIN4_IN()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE2, 0x00U << GPIO_CRL_MODE2_Pos)

#define LED_PIN1_AIN()  MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF8, 0x00U << GPIO_CRH_MODE8_Pos)
#define LED_PIN3_AIN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF13, 0x00U << GPIO_CRH_CNF13_Pos)
#define LED_PIN2_AIN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF14, 0x00U << GPIO_CRH_CNF14_Pos)


#define COM_PIN1_AIN()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF3, 0x00U << GPIO_CRL_CNF3_Pos)
#define COM_PIN2_AIN()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF4, 0x00U << GPIO_CRL_CNF4_Pos)
#define COM_PIN3_AIN()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF10, 0x00U << GPIO_CRH_CNF10_Pos)
#define COM_PIN4_AIN()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF2, 0x00U << GPIO_CRL_CNF2_Pos)


#define LED_PIN1_FLOAT()  MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF8, 0x01U << GPIO_CRH_MODE8_Pos)
#define LED_PIN3_FLOAT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF13, 0x01U << GPIO_CRH_CNF13_Pos)
#define LED_PIN2_FLOAT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF14, 0x01U << GPIO_CRH_CNF14_Pos)


#define LED_PIN1_IPU()  WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BS8);MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF8, 0x02U << GPIO_CRH_CNF8_Pos)
#define LED_PIN3_IPU()  WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS13);MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF13, 0x02U << GPIO_CRH_CNF13_Pos)
#define LED_PIN2_IPU()  WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS14);MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF14, 0x02U << GPIO_CRH_CNF14_Pos)

#define COM_PIN1_FLOAT()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF3, 0x00U << GPIO_CRL_CNF3_Pos)
#define COM_PIN2_FLOAT()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF4, 0x00U << GPIO_CRL_CNF4_Pos)
#define COM_PIN3_FLOAT()  MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF10, 0x00U << GPIO_CRH_CNF10_Pos)
#define COM_PIN4_FLOAT()  MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF2, 0x00U << GPIO_CRL_CNF2_Pos)


#define LED_PIN1_HIGHT() WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BS8)
#define LED_PIN1_LOW()   WRITE_REG(GPIOB->BRR, GPIO_BRR_BR8)

#define LED_PIN3_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS13)
#define LED_PIN3_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR13)

#define LED_PIN2_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS14)
#define LED_PIN2_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR14)


#define COM_PIN1_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS3)
#define COM_PIN1_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR3)

#define COM_PIN2_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS4)
#define COM_PIN2_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR4)

#define COM_PIN3_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS10)
#define COM_PIN3_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR10)

#define COM_PIN4_HIGHT() WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS2)
#define COM_PIN4_LOW()   WRITE_REG(GPIOA->BRR, GPIO_BRR_BR2)
#endif
typedef union
{
    unsigned char  Word;
    struct
    {
        unsigned char gB0:1;
        unsigned char gB1:1;
        unsigned char gB2:1;
        unsigned char gB3:1;
        unsigned char gB4:1;
        unsigned char gB5:1;
        unsigned char gB6:1;
        unsigned char gB7:1;
    }bits;
}BITS_UNION;


/*output */
typedef struct 
{
    uint8_t     LedDeal;
	uint8_t		LedRunMode;/**/
    uint16_t	LedRunTimes;	/*led循环次数*/
    uint16_t	LedRunCnt_Once;/*led一次几闪*/



    uint8_t     gSocUpdate;
    uint8_t     gSocDislay;
}User_LED_Control_t;

extern BITS_UNION ledData;



#define LED1 											ledData.bits.gB1
#define LED2 											ledData.bits.gB2
#define LED3 											ledData.bits.gB3
#define LED4 											ledData.bits.gB4
#define LED5 											ledData.bits.gB5
#define LED6 											ledData.bits.gB6 
#define LED7 											ledData.bits.gB7
#define led_on		1
#define led_off		0

#define LED1_ON                     	  			LED1 = led_on
#define LED1_OFF                    	        	LED1 = led_off
#define LED1_FLASH                  	      		LED1 = ~LED1
#define LED2_ON                     	       	    LED2 = led_on
#define LED2_OFF                    	      	    LED2 = led_off
#define LED2_FLASH                  	     		LED2 = ~LED2
#define LED3_ON                     	   			LED3 = led_on
#define LED3_OFF                    	            LED3 = led_off
#define LED3_FLASH                  	       	    LED3 = ~LED3

#define LED4_ON                     	  			LED4 = led_on
#define LED4_OFF                    	        	LED4 = led_off
#define LED4_FLASH                  	      		LED4 = ~LED4
#define LED5_ON                     	       	    LED5 = led_on
#define LED5_OFF                    	      	    LED5 = led_off
#define LED5_FLASH                  	     		LED5 = ~LED5
#define LED6_ON                     	   			LED6 = led_on
#define LED6_OFF                    	            LED6 = led_off
#define LED6_FLASH                  	       	    LED6 = ~LED6

#define LED7_ON                     	   			LED7 = led_on
#define LED7_OFF                    	            LED7 = led_off
#define LED7_FLASH                  	       	    LED7 = ~LED7
void led_init(void);
void LED_Scan(void);
void LedError_Run(void);
uint8_t LedError_Control(void);
void LedSoc_Run(uint32_t volt_vx100,uint8_t display_en,uint8_t fast_mode);
void UO_Led_Handler(void);
void led_timer_control(void);
void soc_disp(void);
#endif

