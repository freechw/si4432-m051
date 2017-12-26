#include <stdio.h>
#include "M051Series.h"
#include "DrvGPIO.h"
#include "DrvSYS.h"
#include "DrvUART.h"
#include "DrvFMC.h"
#include "USART.h"

extern uint32_t  u32Count;//���ڼ���
extern uint8_t   uart_flage;
extern uint8_t   tabuart[10];//���մ�������
STR_UART_T  config;	//��������
void funcallback(uint32_t uartstart)
 {		   
		UART_T * tUART;
 		    tUART = (UART_T *)(uint32_t)UART0 ; 
 		  if(uartstart!=NULL)
		  {

	             tabuart[u32Count] = tUART->DATA;	                       
         	     u32Count++;	  //���ܼ���
			  if(u32Count==7)	  //��������
				 {
				 u32Count=0;	  //���ڼ�������
				 uart_flage=1;//�������
				 }   
		   

		  } 
					
					
					  
		   
 
 
 }
 void inituart()
 {


      DrvSYS_SetIPClock(E_SYS_UART0_CLK,1);//ʹ��UART0ʱ��
	
      while(DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0)!=FALSE); //1��ƵHCLK
      while(DrvSYS_SetClockDivider(E_SYS_UART_DIV, 0)!=FALSE); //1��ƵUART
	  DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC, 0);//UARTѡ���ⲿ12Mhz��ʱ��Դ
	  DrvGPIO_InitFunction(E_FUNC_UART0);//����IO���UARTģʽ
		   /*�ر����д����ж�*/
      DrvUART_DisableInt (UART_PORT0,DRVUART_WAKEUPINT|
                                        DRVUART_RLSINT|
								        DRVUART_MOSINT|
								       DRVUART_THREINT| 
								       DRVUART_TOUTINT);	 
									
    
	 
				/*���ô��ڲ���*/
		config.u32BaudRate=9600;//������38400
		config.u8cDataBits=DRVUART_DATABITS_8 ;//����λ8λ
		config.u8cStopBits=DRVUART_STOPBITS_1; //ֹͣλ1λ
		config.u8cParity=DRVUART_PARITY_NONE ; //NONEУ��
		config.u8cRxTriggerLevel=DRVUART_FIFO_1BYTES;//����FIFO 8�ֽ�

    DrvUART_Open(UART_PORT0, &config);//����UART�˿�

			/*��װ�ص��������������������жϣ���ʵ֤�����˺�������֮ǰ����ʹ�ÿ����˿�����*/
    DrvUART_EnableInt(	UART_PORT0,DRVUART_RDAINT,funcallback);
}


