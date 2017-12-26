/*
 *�ļ�����MAIN.C
 *
 *���ߣ�СARM�˲�
 *
 * Q Q:925295580     ^_^



 ��Ӧ2012��12��25�յ����������
 *
*/


#include <stdio.h>
#include "M051Series.h"
#include "DrvSYS.h"
#include "DrvUART.h"
#include "DrvFMC.h"
#include "DrvGPIO.h"
#include "main.h"
#include "USART.h"
#include "GPIO.h"
#include "DrvTIMER.h"
#include"SI4432.H"
#include "FMC.H"
#include "SI4432_proto.h"


  
   
uint8_t RX_BUF[100];
uint32_t UID;
uint8_t ID_TIME_OUT_EN=0;
uint8_t ID_TIME_OUT=0;
uint32_t Authenticated[3];
extern  unsigned char tx_timeout_en=0,tx_timeout=0;	//��ʱ����

 uint8_t SI433_ST1=0x88;;
 uint8_t SI433_ST2;
 uint8_t SI433_ST3=0x88;



 void time1_callback(uint32_t pad)
{


		 if(pad)
		 {


		 }

}

void time0_callback(uint32_t pad)
{


		 if(pad)
		 {
			 
			   if(tx_timeout_en)
			   {
			   	  tx_timeout++;
			   }

			   if(ID_TIME_OUT_EN ){

					  ID_TIME_OUT++;
					  
					  }

          }


}


//����ƽ���˲��㷨������ƽ���˲�����
int FilterI=0;
//ADNumΪ��õ�AD��
//nΪ����value_buf[]��Ԫ�ظ������ú�����Ҫ�����ã����ò��������鴫ֵ
int GlideFilterAD(int value_buf[],int n,int ADNum)
{
  int sum=0;
  int count;
  value_buf[FilterI++]=ADNum;
  if(FilterI==n)
   { FilterI=0; //�Ƚ��ȳ�������ƽ��ֵ
  for(count=0;count<n;count++)
    sum+=value_buf[count];
  return (int)(sum/n);
   }
}




int  main(void)
{

uint16_t adc;
uint16_t adc_buf[100];
uint16_t result;
	
unsigned int sum;//����32λ���޷��ű���
unsigned char buf[10]; //8λ�޷��ű���
unsigned char res;//8λ�޷��ű���
//��1+2+3=6�Ĳ���

	   buf[0]=1;
	   buf[1]=2;
	   buf[2]=3;


	  //���
	  sum =	 buf[0] + buf[1] + buf[2];
	  sum = ~sum;  //ȡ��
	  sum+=1;	  //+1
	  sum=sum&0x000000ff;//�������λ
	  res = sum; //ȡ��У��ֵ





		res++;



//initsys();
while(1)
{
		   adc=4096;
  result = GlideFilterAD((int*)adc_buf,10,(int)adc);
	   adc_buf[80]=result ;

	    adc_buf[80]=  sum;

		adc_buf[80]=  res;
}											                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

}



 /*---------------------------------------------------------------------------------------------------------*/
/* initalization system  function  ϵͳ��ʼ��                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/  
 void initsys()
 {
 	
	while(DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1)!=FALSE);  //ʹ���ⲿ12MHZ����
    while( DrvSYS_SelectHCLKSource(0) !=FALSE);  //HCLK=12MHZ
	DrvSYS_SetClockDivider(E_SYS_HCLK_DIV , 0);  //HCLK1��Ƶ
	while(DrvSYS_SelectSysTickSource(0)!=FALSE);  //ѡ��ϵͳ���ʱ��ԴΪ�ⲿ12MHZ����
 	
 }

 void initTMR0()
 {
  DrvSYS_SetIPClock(E_SYS_TMR0_CLK,1); //ʹ��ʼ��
  DrvSYS_SelectIPClockSource(E_SYS_TMR0_CLKSRC,2 );//���TIM0�ǽӵ�APB���ߣ����������ⲿ�����ֲ���Ǵ���ģ�Ӧ�ÿ������APBPCLK��ʱ��
  DrvTIMER_Init();

  DrvTIMER_Open(E_TMR0,10,E_PERIODIC_MODE);	//100MS
  DrvTIMER_EnableInt(E_TMR0);  
  DrvTIMER_SetTimerEvent(E_TMR0, 1, time0_callback, 1);//1s
  DrvTIMER_Start(E_TMR0);
 
 }
  void initTMR1()
 {
  DrvSYS_SetIPClock(E_SYS_TMR1_CLK,1); //ʹ��ʼ��
  DrvSYS_SelectIPClockSource(E_SYS_TMR1_CLKSRC,2 );//���TIM1�ǽӵ�APB���ߣ����������ⲿ�����ֲ���Ǵ���ģ�Ӧ�ÿ������APBPCLK��ʱ��
  DrvTIMER_Init();
  DrvTIMER_Open(E_TMR1,10,E_PERIODIC_MODE);	//10MS
  DrvTIMER_EnableInt(E_TMR1);  
  DrvTIMER_SetTimerEvent(E_TMR1, 2, time1_callback, 1);//20ms�¼���
  DrvTIMER_Start(E_TMR1);
 
 }





























