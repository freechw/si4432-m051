// File Name:SI4432.C
// Author:СARM�˲�
// Date: 2012��
 //Q Q:925295580

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

extern  uint8_t tx_timeout_en,tx_timeout;

 /*
** ������ : SPI_Write_OneByte
** ����ֵ : None
** ��  �� : u8_writedata--SPIд���һ�ֽ�����
** ��  �� : ������д���ݣ�ÿ��д�� 1 bit
*/
void SPI_Write_OneByte(uint8_t u8_writedata)
{
  uint8_t i;
  
  for(i=0;i<8;i++)
  {
    if(u8_writedata & 0x80)      //�ж����λ�����Ƿ������λ
     SDI_UP;                    //MOSI���1����������׼������1
    else
     SDI_DOWN;                   //MOSI���0����������׼������0
     
   	SCK_UP;                     //����������(SCK��0-->1)�����������ϵ�����д�뵽����
    u8_writedata <<= 1;          //���������Ѿ���������λ
    	SCK_DOWN;                     //����SCK�źţ���ʼ��Ϊ0
  }
}
/*
** ������ : SPI_Read_OneByte
** ����ֵ : temp--SPI��ȡ��һ�ֽ�����
** ��  �� : None
** ��  �� : �½��ض����ݣ�ÿ�ζ�ȡ 1 bit
*/
uint8_t SPI_Read_OneByte(void)
{
  uint8_t i;
  uint8_t temp = 0;
  
  for(i=0;i<8;i++)
  {
   temp <<= 1;       //��ȡMISO 8�������ֵ������temp��֮���Բ����ڡ�SCK = 0�����֮���λ������Ϊ��
                     //��ȡ���1byte�����һλ(��LSB)֮�󣬲�����������
   	SCK_UP;          
   if(GET_SDO)          //��ȡ���λ����������ĩβ��ͨ������λ��ɶ������ֽ�
     temp |= 0x01;
    else
     temp &= ~0x01;
  	SCK_DOWN;          //�½�������(SCK��1-->0)��MISO�ϵ����ݽ������ı䣬�ȶ����ȡ����temp
  }
  
  return temp;
}

/*
** ������: nRF24L01_ReadReg
** ����ֵ: value--��ȡ�Ĵ���ֵ
** �� �� : addr--�Ĵ�����ַ
** ˵ �� : nRF24L01�Ĵ���������
*/
uint8_t nRF24L01_ReadReg(uint8_t addr)
{
    uint8_t value;
    nSEL_DOWN;                   //CSƬѡ����
   SPI_Write_OneByte(addr|0);  //SPIд��ַ����
    value = SPI_Read_OneByte();  //SPI������
    nSEL_UP;                    //CSƬѡ����
    return value;
}

 /*
** ������ : SPI_WriteAndRead_OneByte
** ����ֵ : u8_readdata--SPI��ȡ��һ�ֽ�����
** �� �� : u8_writedata--SPIд���һ�ֽ�����
** �� �� : ������д���½��ض�
*/
uint8_t SPI_WriteAndRead_OneByte(uint8_t u8_writedata)
{
  uint8_t i;
  uint8_t u8_readdata = 0x00;
     
  for(i=0;i<8;i++)
  {
    u8_readdata <<= 1;      //��ȡMISO 8�������ֵ������u8_readdata��
       
    if(u8_writedata & 0x80) //�ж����λ������д���λ��������λ��
      SDI_UP;              //MOSI���1����������׼������1
    else
      SDI_DOWN;             //MOSI���0����������׼������0
    u8_writedata <<= 1;     //���������Ѿ���������λ
       
    	SCK_UP;                //����������(SCK��0-->1)�����������ϵ�����д������
    if(GET_SDO)                //��ȡ���λ����������ĩβ��ͨ������λ��ɶ������ֽ�
      u8_readdata |= 0x01;
    else
      u8_readdata &= ~0x01;
        
    SCK_DOWN;                //�½�������(SCK��1-->0)��MISO�Ͻ������µ����ݣ���ȡ����u8����readdata
   }
  return u8_readdata;
}
unsigned char RE_RSSI(void)
{
	  uint8_t RSSI;
	RSSI = spi_rw(0x26, 0x00);
	return RSSI;


}


unsigned char  save_rec_data(unsigned char *recbuf_pt)
{
			 uint8_t leg,i;
			 	if(!GET_NIRQ)
				{
				  i = RE_RSSI();
             clr_interruput_si4432();
		        leg =spi_rw(0x4b,0);              
				SCK_DOWN;
				nSEL_DOWN; 
				spi_byte(0x7f);		
				for(i=0;i<leg;i++)	
				{
					*(recbuf_pt+i) = spi_byte(0x00);
				}
				nSEL_UP;
				spi_rw(0x07|0x80, SI4432_PWRSTATE_READY);	  
			
			 rx_data();	
		      return leg; 
			}
		  	  return RX_NO_RECIVER;
}
 
void clr_interruput_si4432(void)
{

 spi_rw(0x03,0x00);	
 spi_rw(0x04,0x00);	

}

void initsi4432(void)
{
     unsigned char a;

      DrvSYS_Delay(10000);
	  SDN_DOWN;
 	  DrvSYS_Delay(200000);
	  SI4432_init();  
  	  TX0_RX0;


	a = spi_rw(0,0);

	a=a;
	a = spi_rw(1,0);

	a=a;
  
}
 
void rx_data(void)
{	

	spi_rw(0x07|0x80, SI4432_PWRSTATE_READY);	
//	if( spi_rw(0x07,0x00)!=SI4432_PWRSTATE_READY) {spi_rw(0x07|0x80, SI4432_PWRSTATE_READY);}
  if(spi_rw(0x07,0x00)!=SI4432_PWRSTATE_READY)
		 {
				spi_rw(0x07|0x80, SI4432_PWRSTATE_READY);
		 }

 	TX0_RX1;	
	
	spi_rw(0x08|0x80, 0x03);  
	spi_rw(0x08|0x80, 0x00);  
		
	spi_rw(0x07|0x80,SI4432_PWRSTATE_RX );
	
		if(spi_rw(0x07,0x00)!=SI4432_PWRSTATE_RX)
		 {
			spi_rw(0x07|0x80,SI4432_PWRSTATE_RX );
		 }

	
	spi_rw(0x05|0x80, SI4432_Rx_packet_received_interrupt);  
	
	  if(spi_rw(0x05,0x00)!=SI4432_Rx_packet_received_interrupt)
		 {
		spi_rw(0x05|0x80, SI4432_Rx_packet_received_interrupt);  
		 }

    spi_rw(0x03,0x00);	
	spi_rw(0x04,0x00);		
	
		
}		


unsigned char tx_data(unsigned char *pt,unsigned char leg)
{
	unsigned char i,ItStatus;
	
			 
	spi_rw(0x07|0x80, SI4432_PWRSTATE_READY);	// rf ģ�����Ready ģʽ
	if( spi_rw(0x07,0x00)!=SI4432_PWRSTATE_READY){spi_rw(0x07|0x80, SI4432_PWRSTATE_READY);} 	
	TX1_RX0;	

	   if((spi_rw(0x04,0x00))&&0x10 )
	   {
	spi_rw(0x08|0x80, 0x03);   
	spi_rw(0x08|0x80, 0x00);  
	
	

		
	spi_rw(0x34|0x80, 64); // 
	spi_rw(0x3e|0x80, leg); //
  	for (i = 0; i<leg; i++)
	{
		spi_rw(0x7f|0x80,*(pt+i));//
	}
  	 spi_rw(0x05|0x80, SI4432_PACKET_SENT_INTERRUPT);
     spi_rw(0x03,0x00);			
     spi_rw(0x04,0x00);
	 spi_rw(0x03,0x00);		
     spi_rw(0x04,0x00);
     spi_rw(0x07|0x80, SI4432_PWRSTATE_TX);
	   
if( spi_rw(0x07,0x00)!=SI4432_PWRSTATE_TX) {spi_rw(0x07|0x80, SI4432_PWRSTATE_TX);}
	
	while(GET_NIRQ)
	{
	  tx_timeout_en=1;
	  if(tx_timeout==6) 
	  {

	  	 tx_timeout_en=0;
			tx_timeout=0;
		   initsi4432();
			 rx_data();
		 return TX_TIMEOUT;
	  
	  
	  }
	
	}//�ȴ������ж�

		tx_timeout_en=0;//�س�ʱ
		tx_timeout=0;

		ItStatus =  spi_rw(0x03,0x00);  //���жϼĴ���
		if((ItStatus&0x04)==0x04)
		{
		clr_interruput_si4432();

		  rx_data();
		 return TX_SUCCESS;
		}
		else
		{
			rx_data();
		return TX_FAIL;
		}
		}

			  rx_data();
		return TX_RSSI_CHANNLE_OCCUPANCY;
	   //�������


	 
}

//REG����		
void SI4432_init(void)
{
	



	clr_interruput_si4432();

	spi_rw(0x06|0x80, 0x00);  //  �رղ���Ҫ���ж�
	
	spi_rw(0x07|0x80, 1);   // ���� Ready ģʽxton=1
	 


	spi_rw(0x09|0x80, 0x64);  //  ���ص���


	spi_rw(0x0a|0x80, 0x05);	// �رյ�Ƶ���
	spi_rw(0x0b|0x80, 0xea);  // GPIO 0 ������ͨ�����
	spi_rw(0x0c|0x80, 0xea);  //GPIO 1 ������ͨ�����
	spi_rw(0x0d|0x80, 0xf4);  // /GPIO 2 ����յ�������




	spi_rw(0x70|0x80, 0x2c);  	// ��������ø���Silabs ��Excel
	spi_rw(0x1d|0x80, 0x40);  // ʹ�� afc
		
	// 1.2K bps setting start
	spi_rw(0x1c|0x80, 0x16);	
	spi_rw(0x20|0x80, 0x83);   
	spi_rw(0x21|0x80, 0xc0); //
	spi_rw(0x22|0x80, 0x13);// 
	spi_rw(0x23|0x80, 0xa9); //
	spi_rw(0x24|0x80, 0x00); //
	spi_rw(0x25|0x80, 0x04); //
	spi_rw(0x2a|0x80, 0x14);
	spi_rw(0x6e|0x80, 0x09);
	spi_rw(0x6f|0x80, 0xd5);
	
		
	//1.2K bps setting end
	
			
		
	spi_rw(0x30|0x80, 0x8c);   // ʹ��PH+ FIFOģʽ����λ��ǰ�棬ʹ��CRCУ��	
			
	spi_rw(0x32|0x80, 0xff);  // byte0, 1,2,3 ��Ϊͷ��
	spi_rw(0x33|0x80, 0x42);//  byte 0,1,2,3 ��ͷ�룬ͬ����3,2 ��ͬ����
	
	spi_rw(0x35|0x80, 0x20);  // ��Ҫ���4��nibble��Preamble  �ֲ��ͼ18   42ҳ

	spi_rw(0x36|0x80, 0x2d);  // ͬ����Ϊ 0x2dd40000
	spi_rw(0x37|0x80, 0xd4);
	spi_rw(0x38|0x80, 0x00);
	spi_rw(0x39|0x80, 0x00);


	spi_rw(0x3a|0x80, 's');  // �����ͷ��Ϊ�� ��swwx"
	spi_rw(0x3b|0x80, 'w');
	spi_rw(0x3c|0x80, 'w');
	spi_rw(0x3d|0x80, 'x');
	spi_rw(0x3e|0x80, 10);  // �ܹ�����10���ֽڵ�����
	spi_rw(0x3f|0x80, 's'); // ��ҪУ���ͷ��Ϊ����swwx"
	spi_rw(0x40|0x80, 'w');
	spi_rw(0x41|0x80, 'w');
	spi_rw(0x42|0x80, 'x');

	spi_rw(0x43|0x80, 0xff);  // ͷ��1,2,3,4 ������λ����ҪУ��
	spi_rw(0x44|0x80, 0xff);  // 
	spi_rw(0x45|0x80, 0xff);  // 
	spi_rw(0x46|0x80, 0xff);  //
	 
	spi_rw(0x6d|0x80, 0x07);  // set power

	spi_rw(0x79|0x80, 0x0);  // ����Ҫ��Ƶ
	spi_rw(0x7a|0x80, 0x0);  // ����Ҫ��Ƶ
	
	
	spi_rw(0x71|0x80, 0x22); // ���䲻��Ҫ CLK��FiFo �� FSKģʽ
			
	spi_rw(0x72|0x80, 0x30);  // ƵƫΪ 30KHz
	

	spi_rw(0x73|0x80, 0x0);  // û��Ƶ��ƫ��
	spi_rw(0x74|0x80, 0x0);  // û��Ƶ��ƫ��
	
	
		
	spi_rw(0x75|0x80, 0x53);  // Ƶ������ 433.5
	spi_rw(0x76|0x80, 0x57);  // 
	spi_rw(0x77|0x80, 0x80);
		  
}
unsigned char spi_byte(unsigned char data)
{

	unsigned char i;
	
	for (i = 0; i < 8; i++)		// ����SCK �� SDI������һ���ֽڵ����ͬ�¶�ȡ1���ֽڵ�����
	{				// û�а���nSEL�Ŀ���
		if (data & 0x80)
			SDI_UP;
		else
			SDI_DOWN;
			
		data <<= 1;
		SCK_UP;
		
		if (GET_SDO)
			data |= 0x01;
		else
			data &= 0xfe;
			
		SCK_DOWN;
	}
	
	return (data);
}

//-------------------------------------------//
unsigned char spi_rw(unsigned char addr, unsigned char data)
{
	unsigned char i;
	
	SCK_DOWN;
	nSEL_DOWN;
	
	for (i = 0; i < 8; i++) 
	{
		if (addr & 0x80)
			SDI_UP;
		else
			SDI_DOWN;
		addr <<= 1;
		SCK_UP;
	DrvSYS_Delay(5);
	
		SCK_DOWN;
	}
	
	for (i = 0; i < 8; i++) 
	{
		if (data & 0x80)
			SDI_UP;
		else
			SDI_DOWN;
		data <<= 1;
		SCK_UP;
		if (GET_SDO)
			data |= 0x01;
		else
			data &= 0xfe;
		SCK_DOWN;
	}
	nSEL_UP;
	SCK_UP;
	return data;
}
 