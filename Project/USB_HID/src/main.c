#define GLOBALS
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "stdio.h"	   
#include "demo.h"
#include "hw_config.h"
#include "delay.h"
#include "stm32f10x_tim.h"
#include "misc.h"
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);

extern void USB_Disconnect_Config(void);
__IO uint8_t PrevXferComplete = 1;

int config_mode = 0, key_bit = 0, key_buff = 0, light_time = 0, ledcount = 0, R, G, B, light_choice = 0,color = 1;

void delay(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=120;  // �Լ�����
      while(i--) ;    
   }
}
void delay_fast(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  // �Լ�����
      while(i--) ;    
   }
}

/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void){

  SystemInit();	  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}


/****************************************************************************
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void NVIC_Configuration(void)
{
  /*  �ṹ����*/
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��  */    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	    //USB�����ȼ��ж�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//��ռ���ȼ� 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;			//USB�����ȼ��ж�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//��ռ���ȼ� 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 
}


void IRQHandler(void)   //�ж�ִ�к���
{
    Joystick_Send(0, 0x1B);
    Joystick_Send(0, 0);
}

/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ�������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //��ʼ��GPIOʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //��ʼ��GPIOʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    TIM3_Int_Init(99,71);  //�ж�1us
    /*
    TIM3_Int_Init(4999,7199);
    ((4999+1)*(7199+1))/7200=5000  10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
    
    Tout = ( (arr + 1) * (psc + 1) ) / Tclk
    Tclk��TIM3 ������ʱ��Ƶ�ʣ���λΪ Mhz��
    Tout��TIM3 ���ʱ�䣨��λΪ us�� 
    */
    
  RCC_Configuration(); 		//����ϵͳʱ�� 
  USB_Disconnect_Config();	//����USB���ӿ�����    	   
  NVIC_Configuration();     //USBͨ���ж�����
  USB_Init();				//USB��ʼ��
  while (1)
  {

  }
}
//ͨ�ö�ʱ��3�жϳ�ʼ��

//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
    
    //��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ    
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx                     
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־
        IRQHandler();
    }
}
