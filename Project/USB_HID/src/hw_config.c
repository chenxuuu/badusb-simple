/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : hw_config.c
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

#include "demo.h"

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"	 
#include "usb_pwr.h"
#include "usb_lib.h" 


ErrorStatus HSEStartUpStatus;
extern __IO uint8_t PrevXferComplete;

/*******************************************************************************
* Function Name  : USB_Disconnect_Config
* Description    : Disconnect pin configuration
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Disconnect_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable USB_DISCONNECT GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* USB_DISCONNECT_PIN used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

#ifdef STM32L1XX_MD
  Device_Serial0 = *(uint32_t*)(0x1FF80050);
  Device_Serial1 = *(uint32_t*)(0x1FF80054);
  Device_Serial2 = *(uint32_t*)(0x1FF80064);
#else  
  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);
#endif /* STM32L1XX_MD */
  
  Device_Serial0 += Device_Serial2;
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
//  /* Set the device state to suspend */
//  bDeviceState = SUSPENDED;
//
//  /* Clear EXTI Line18 pending bit */
//  EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
//
//  /* Request to enter STOP mode with regulator in low power mode */
//  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {}
  
#ifdef  STM32F10X_CL
  /* Enable PLL2 */
  RCC_PLL2Cmd(ENABLE);  

  /* Wait till PLL2 is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
  {}
#endif /* STM32F10X_CL */
  
  /* Enable PLL1 */
  RCC_PLLCmd(ENABLE);

  /* Wait till PLL1 is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {}

  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

  /* Wait till PLL is used as system clock source */
#ifdef STM32L1XX_MD  
  while (RCC_GetSYSCLKSource() != 0x0C)
#else   
  while (RCC_GetSYSCLKSource() != 0x08)
#endif /* STM32L1XX_MD */ 
  {}  
  
  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}


/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{  
  if (NewState != DISABLE)
  {
    //GPIO_ResetBits(GPIOC, GPIO_Pin_13);
  }
  else
  {
    //GPIO_SetBits(GPIOC, GPIO_Pin_13);
  }	
}

/*******************************************************************************
* Function Name : JoyState.
* Description   : Decodes the Joystick direction.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
u8 JoyState(void)
{
  /* "right" key is pressed */
  if (!GPIO_ReadInputDataBit(GPIOE, JOY_RIGHT))
  {
    return RIGHT;
  }
  /* "left" key is pressed */
  if (!GPIO_ReadInputDataBit(GPIOE, JOY_LEFT))
  {
    return LEFT;
  }
  /* "up" key is pressed */
  if (!GPIO_ReadInputDataBit(GPIOD, JOY_UP))
  {
    return UP;
  }
  /* "down" key is pressed */
  if (!GPIO_ReadInputDataBit(GPIOD, JOY_DOWN))
  {
    return DOWN;
  }
  if (!GPIO_ReadInputDataBit(GPIOC, JOY_LEFT_BUTTON))
  {
    return LEFT_BUTTON;
  }
   if (!GPIO_ReadInputDataBit(GPIOC, JOY_RIGHT_BUTTON))
  {
    return RIGHT_BUTTON;
  }
  /* No key is pressed */
  else
  {
    return 0;
  }
}
#define KEY_LEFT_CTRL   0x80
#define KEY_LEFT_SHIFT    0x81
#define KEY_LEFT_ALT    0x82
#define KEY_LEFT_GUI    0x83
#define KEY_RIGHT_CTRL    0x84
#define KEY_RIGHT_SHIFT   0x85
#define KEY_RIGHT_ALT   0x86
#define KEY_RIGHT_GUI   0x87

#define KEY_UP_ARROW    0xDA
#define KEY_DOWN_ARROW    0xD9
#define KEY_LEFT_ARROW    0xD8
#define KEY_RIGHT_ARROW   0xD7
#define KEY_BACKSPACE   0xB2
#define KEY_TAB       0xB3
#define KEY_RETURN      0xB0
#define KEY_ESC       0xB1
#define KEY_INSERT      0xD1
#define KEY_DELETE      0xD4
#define KEY_PAGE_UP     0xD3
#define KEY_PAGE_DOWN   0xD6
#define KEY_HOME      0xD2
#define KEY_END       0xD5
#define KEY_CAPS_LOCK   0xC1
#define KEY_F1        0xC2
#define KEY_F2        0xC3
#define KEY_F3        0xC4
#define KEY_F4        0xC5
#define KEY_F5        0xC6
#define KEY_F6        0xC7
#define KEY_F7        0xC8
#define KEY_F8        0xC9
#define KEY_F9        0xCA
#define KEY_F10       0xCB
#define KEY_F11       0xCC
#define KEY_F12       0xCD
/*******************************************************************************
* Function Name : Joystick_Send.
* Description   : prepares buffer to be sent containing Joystick event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
void Joystick_Send(u8 win_buf,u8 key_buf)
{
u8 Mouse_Buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
/* prepare buffer to send */
Mouse_Buffer[0]=win_buf;    
Mouse_Buffer[2]=key_buf;       
/*copy mouse position info in ENDP1 Tx Packet Memory Area*/
UserToPMABufferCopy(Mouse_Buffer, GetEPTxAddr(ENDP1), 8);
/* enable endpoint for transmission */
SetEPTxValid(ENDP1);
}

void Key_Input(char ch)
{
    u16 i = 0, time = 10;
    if(ch == 'a' || ch == 'A')
    {
        Joystick_Send(0, 0x04);
    }
    else if(ch == 'b' || ch == 'B')
    {
        Joystick_Send(0, 0x05);
    }
    else if(ch == 'c' || ch == 'C')
    {
        Joystick_Send(0, 0x06);
    }
    else if(ch == 'd' || ch == 'D')
    {
        Joystick_Send(0, 0x07);
    }
    else if(ch == 'e' || ch == 'E')
    {
        Joystick_Send(0, 0x08);
    }
    else if(ch == 'f' || ch == 'F')
    {
        Joystick_Send(0, 0x09);
    }
    else if(ch == 'g' || ch == 'G')
    {
        Joystick_Send(0, 0x0A);
    }
    else if(ch == 'h' || ch == 'H')
    {
        Joystick_Send(0, 0x0B);
    }
    else if(ch == 'i' || ch == 'I')
    {
        Joystick_Send(0, 0x0C);
    }
    else if(ch == 'j' || ch == 'J')
    {
        Joystick_Send(0, 0x0D);
    }
    else if(ch == 'k' || ch == 'K')
    {
        Joystick_Send(0, 0x0E);
    }
    else if(ch == 'l' || ch == 'L')
    {
        Joystick_Send(0, 0x0F);
    }
    else if(ch == 'm' || ch == 'M')
    {
        Joystick_Send(0, 0x10);
    }
    else if(ch == 'n' || ch == 'N')
    {
        Joystick_Send(0, 0x11);
    }
    else if(ch == 'o' || ch == 'O')
    {
        Joystick_Send(0, 0x12);
    }
    else if(ch == 'p' || ch == 'P')
    {
        Joystick_Send(0, 0x13);
    }
    else if(ch == 'q' || ch == 'Q')
    {
        Joystick_Send(0, 0x14);
    }
    else if(ch == 'r' || ch == 'R')
    {
        Joystick_Send(0, 0x15);
    }
    else if(ch == 's' || ch == 'S')
    {
        Joystick_Send(0, 0x16);
    }
    else if(ch == 't' || ch == 'T')
    {
        Joystick_Send(0, 0x17);
    }
    else if(ch == 'u' || ch == 'U')
    {
        Joystick_Send(0, 0x18);
    }
    else if(ch == 'v' || ch == 'V')
    {
        Joystick_Send(0, 0x19);
    }
    else if(ch == 'w' || ch == 'W')
    {
        Joystick_Send(0, 0x1A);
    }
    else if(ch == 'x' || ch == 'X')
    {
        Joystick_Send(0, 0x1B);
    }
    else if(ch == 'y' || ch == 'Y')
    {
        Joystick_Send(0, 0x1C);
    }
    else if(ch == 'z' || ch == 'Z')
    {
        Joystick_Send(0, 0x1D);
    }
    else if(ch == '\n')
    {
        Joystick_Send(0, 0x58);
    }
    else if(ch == '1')
    {
        Joystick_Send(0, 0x1E);
    }
    else if(ch == '2')
    {
        Joystick_Send(0, 0x1F);
    }
    else if(ch == '3')
    {
        Joystick_Send(0, 0x20);
    }
    else if(ch == '4')
    {
        Joystick_Send(0, 0x21);
    }
    else if(ch == '5')
    {
        Joystick_Send(0, 0x22);
    }
    else if(ch == '6')
    {
        Joystick_Send(0, 0x23);
    }
    else if(ch == '7')
    {
        Joystick_Send(0, 0x24);
    }
    else if(ch == '8')
    {
        Joystick_Send(0, 0x25);
    }
    else if(ch == '9')
    {
        Joystick_Send(0, 0x26);
    }
    else if(ch == '0')
    {
        Joystick_Send(0, 0x27);
    }
    else if(ch == '!')
    {
        Joystick_Send(0x02, 0x1E);
    }
    else if(ch == '@')
    {
        Joystick_Send(0x02, 0x1F);
    }
    else if(ch == '#')
    {
        Joystick_Send(0x02, 0x20);
    }
    else if(ch == '$')
    {
        Joystick_Send(0x02, 0x21);
    }
    else if(ch == '%')
    {
        Joystick_Send(0x02, 0x22);
    }
    else if(ch == '^')
    {
        Joystick_Send(0x02, 0x23);
    }
    else if(ch == '&')
    {
        Joystick_Send(0x02, 0x24);
    }
    else if(ch == '*')
    {
        Joystick_Send(0x02, 0x25);
    }
    else if(ch == '(')
    {
        Joystick_Send(0x02, 0x26);
    }
    else if(ch == ')')
    {
        Joystick_Send(0x02, 0x27);
    }
    else if(ch == ' ')
    {
        Joystick_Send(0, 0x2C);
    }
    else if(ch == '-')
    {
        Joystick_Send(0, 0x2D);
    }
    else if(ch == '=')
    {
        Joystick_Send(0, 0x2E);
    }
    else if(ch == '[')
    {
        Joystick_Send(0, 0x2F);
    }
    else if(ch == ']')
    {
        Joystick_Send(0, 0x30);
    }
    else if(ch == '\\')
    {
        Joystick_Send(0, 0x31);
    }
    else if(ch == ';')
    {
        Joystick_Send(0, 0x33);
    }
    else if(ch == '\'')
    {
        Joystick_Send(0, 0x34);
    }
    else if(ch == ',')
    {
        Joystick_Send(0, 0x36);
    }
    else if(ch == '.')
    {
        Joystick_Send(0, 0x37);
    }
    else if(ch == '/')
    {
        Joystick_Send(0, 0x38);
    }
    else if(ch == '_')
    {
        Joystick_Send(0x02, 0x2D);
    }
    else if(ch == '+')
    {
        Joystick_Send(0x02, 0x2E);
    }
    else if(ch == '{')
    {
        Joystick_Send(0x02, 0x2F);
    }
    else if(ch == '}')
    {
        Joystick_Send(0x02, 0x30);
    }
    else if(ch == '|')
    {
        Joystick_Send(0x02, 0x31);
    }
    else if(ch == '~')
    {
        Joystick_Send(0x02, 0x32);
    }
    else if(ch == ':')
    {
        Joystick_Send(0x02, 0x33);
    }
    else if(ch == '"')
    {
        Joystick_Send(0x02, 0x34);
    }
    else if(ch == '<')
    {
        Joystick_Send(0x02, 0x36);
    }
    else if(ch == '>')
    {
        Joystick_Send(0x02, 0x37);
    }
    else if(ch == '?')
    {
        Joystick_Send(0x02, 0x38);
    }
    while(time--)
    {
        i = 120; // 自己定义
        while(i--) ;
    }
    Joystick_Send(0, 0);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
