#ifdef GLOBALS 
#define EXT
#else
#define EXT extern 
#endif

#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"


#define JOY_UP       GPIO_Pin_2  /* PD.08 */
#define JOY_DOWN     GPIO_Pin_3 /* PD.14 */
#define JOY_LEFT     GPIO_Pin_4  /* PE.01 */
#define JOY_RIGHT    GPIO_Pin_5  /* PE.00 */
#define JOY_LEFT_BUTTON    GPIO_Pin_13  /* PC.13 */
#define JOY_RIGHT_BUTTON    GPIO_Pin_12  /* PC.12 */

#define Led_ON()   GPIO_SetBits(GPIOB, GPIO_Pin_5);  	      //LED1ÁÁ
#define Led_OFF()  GPIO_ResetBits(GPIOB, GPIO_Pin_5); 	      //LED2Ãð



