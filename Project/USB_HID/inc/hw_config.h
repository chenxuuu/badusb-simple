/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : hw_config.h
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "usb_type.h"

#define CTRL_L 0x01
#define SHIFT_L 0x02
#define ALT_L 0x04
#define WIN_L 0x08


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define DOWN            1
#define LEFT            2
#define RIGHT           3
#define UP              4
#define LEFT_BUTTON     5
#define RIGHT_BUTTON    6
#define CURSOR_STEP     3
/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void Set_USBClock(void);
void GPIO_AINConfig(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void Joystick_Send(u8 win_buf,u8 key_buf);
u8 JoyState(void);
void Get_SerialNum(void);
void Key_Input(char ch);
void push_lines(char *s);
#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
