
// File Name:GPIO.C
// Author:СARM�˲�
// Date: 2012��
 //Q Q:925295580
 #include "DrvGPIO.h"
#include "GPIO.h"	  
#include"SI4432.H" 
#include "M051Series.h"
#include "DrvSYS.h"
#include "SI4432_proto.h"

extern SI4432Ctrl_Frame RX_SI4432_HADE;
extern uint8_t Req_Send_Myself_Id_To_Host_Driver(void);					                                                                

void initGPIO()
 {
 

			   	   
						 //�˿�ģʽ
						 
				  			 	   
    DrvGPIO_Open(E_PORT0,E_PIN6, E_IO_INPUT);//input miso
   	DrvGPIO_Open(E_PORT3,E_PIN2, E_IO_INPUT);//input nirq
	DrvGPIO_Open(E_PORT0,E_PIN5, E_IO_OUTPUT); //mosi
   	DrvGPIO_Open(E_PORT0,E_PIN7, E_IO_OUTPUT); //sck
	DrvGPIO_Open(E_PORT0,E_PIN4, E_IO_OUTPUT); //nsel
    DrvGPIO_Open(E_PORT4,E_PIN1, E_IO_OUTPUT); //sdn
					 

               		 //led
    DrvGPIO_Open(E_PORT2,E_PIN7, E_IO_OUTPUT); //led
			   
		     		 //led
    DrvGPIO_Open(E_PORT4,E_PIN4, E_IO_INPUT);  //ѧϰ����

	DrvGPIO_Open(E_PORT1,E_PIN6, E_IO_OUTPUT);  //MOS

 
MOS1_CLOSE ;			 

 }
