
#include "LCD12864.h"
#include "charcode.h"
#include "interface.h"
#include <stdio.h>

//��ʱn us
void delay_nus(unsigned long n)
{
	unsigned long j;
	while(n--)
	{ j=8;
		while(j--);
	}
}


//GPIO���ú���
void LCDGPIO_Configuration(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LCDCS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDCS_GPIO, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = LCDRST_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDRST_GPIO, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin = LCDRS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDRS_GPIO, &GPIO_InitStructure);   

	GPIO_InitStructure.GPIO_Pin = LCDSCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDSCL_GPIO, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = LCDSID_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDSID_GPIO, &GPIO_InitStructure);  
}

/*******************************************************************************
* �� �� ��         : LCD12864_WriteCmd
* ��������		   : д��һ�����12864
* ��    ��         : cmd
* ��    ��         : ��
*******************************************************************************/

void LcdWriteCmd(u8 cmd)
{
	char i; 
	LCDCS_RESET;
	LCDRS_RESET;
	for(i=0;i<8;i++){ 
		LCDSCL_RESET; 
		if(cmd&0x80) LCDSID_SET; //��������
		else       LCDSID_RESET; 
		LCDSCL_SET; //����ʱ��������
		cmd <<= 1; 
	} 
	LCDCS_SET; 
}

/*******************************************************************************
* �� �� ��         : LcdWriteData
* ��������		   : д��һ�����ݵ�12864
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/

void LcdWriteData(u8 dat)
{	
	char i; 
	LCDCS_RESET;
	LCDRS_SET;
	for(i=0;i<8;i++){ 
		LCDSCL_RESET; 
		if(dat&0x80) LCDSID_SET; //��������
		else       LCDSID_RESET; 
		
		LCDSCL_SET; //����ʱ��������
		dat <<= 1; 
	} 
	LCDCS_SET; 
}

/*******************************************************************************
* �� �� �� ��LCD12864Init
* �������� ��LCD1602��ʼ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void LCD12864Init(void)
{
	const unsigned char con_d2 = 0x16; //�Աȶ�ϸ��Ĭ��ֵ,�����÷�Χ 0x00��0x3f
	
	LCDGPIO_Configuration();
	
	LCDRST_RESET;     Delay_us(3000);//�͵�ƽ��λ
	LCDRST_SET;     Delay_us(50);  //��λ���
	
	LcdWriteCmd(0xe2); Delay_us(15);   //����λ 

	LcdWriteCmd(0x2c); Delay_us(15);  //��ѹ���� Boost ON
	LcdWriteCmd(0x2e); Delay_us(15);  //��ѹ���� Voltage Regular ON
	LcdWriteCmd(0x2f); Delay_us(15);  //��ѹ���� Voltage Follower ON

	LcdWriteCmd(0x24);  //�ֵ��Աȶȣ������÷�Χ 0x20��0x27
	LcdWriteCmd(0x81);  //΢���Աȶ�,���м����������
	LcdWriteCmd(con_d2);//0x1a,΢���Աȶȵ�ֵ�������÷�Χ 0x00��0x3f

	LcdWriteCmd(0xa2);  //1/9 ƫѹ��(bais set)
	LcdWriteCmd(0xc8);  //��ɨ��˳��0xc8���ϵ���,0xc0���µ���
	LcdWriteCmd(0xa0);  //��ɨ��˳��0xa0������,0xa1���ҵ���

	LcdWriteCmd(0x40);  //��ʼ�У���һ�п�ʼ
	LcdWriteCmd(0xaf);  //����ʾ	

	LcdClearScreen();
	//LcdWriteString(0, 0, "��̫�������޹�˾");
	ShowEnString(0,0,"   V:   cm/s    ");
	ShowEnString(0,1,"   D:   cm      ");
	ShowEnString(0,2,"                ");
	ShowEnString(0,3,"  k:     b:     ");
	//ShowEnString(0,2,"   Warning!!!   ");
//	ShowEnString(0,4,"0123456789abcdef");
//	ShowEnString(0,5,"0123456789abcdef");
//	ShowEnString(0,6,"0123456789abcdef");
//	ShowEnString(0,7,"0123456789abcdef");
//	LCD12864WriteCommand(' ');
}

/*******************************************************************************
* �� �� ��         : LcdClearScreen
* ��������		   : ����12864
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void LcdClearScreen(void)
{
	unsigned char i, j;

	for(i=0; i<8; i++)
	{
		LcdWriteCmd(0xB0+i); //y���ַ

		//--��д������֮��X����������ǻ��Զ���1�ģ����ǳ�ʼ��ʹ��0xA0
		//x�������0x10,0x00��ʼ
		LcdWriteCmd(0x10); 
		LcdWriteCmd(0x00);							   
		
		//X����128λ����һ��ˢ128�Σ�X������Զ���1���������ǲ�������������
		for(j=0; j<128; j++)
		{
			LcdWriteData(0x00);  
		}
	}
}

/*******************************************************************************
* �� �� �� ��LcdWriteString
* �������� ���ú�������ֱ����д�ַ��� 16*16
* ��    �� ��x x����(0,127)   ,   y y����(0,3)  ��  str Ҫд����ַ���
* ��    �� ����
*******************************************************************************/
unsigned char LcdWriteString(unsigned char x, unsigned char y, unsigned char *cn)
{
	unsigned char j, x1, x2, wordNum;
	y = y*2;
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y > 7)
	{
		return 0;
	}

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
	if(x > 128)
	{
		return 0;
	}
	y += 0xB0;	   //��ȡY�����ֵ
	//--����Y����--//
	LcdWriteCmd(y);
	while ( *cn != '\0')	 //��C�������ַ��������ԡ�\0����β
	{	
		
		//--����Y����--//
		LcdWriteCmd(y);

		x1 = (x >> 4) & 0x0F;   //����X����Ҫ��������ָߵ�4λ������������ȡ����4λ
		x2 = x & 0x0F;          //ȥ����λ
		//--����X����--//
		LcdWriteCmd(0x10 + x1);   //��4λ
		LcdWriteCmd(x2);	//��4λ

		for (wordNum=0; wordNum<50; wordNum++)
		{
			//--��ѯҪд�������ֿ��е�λ��--//
			if ((CN16CHAR[wordNum].Index[0] == *cn)
					&&(CN16CHAR[wordNum].Index[1] == *(cn+1)))
			{
				for (j=0; j<32; j++) //дһ����
				{		
					if (j == 16)	 //����16X16�õ�����Y���꣬�����ڵ���16ʱ���л�����
					{
						//--����Y����--//
						LcdWriteCmd(y + 1);
						
						//--����X����--//
						LcdWriteCmd(0x10 + x1);   //��4λ
						LcdWriteCmd(x2);	//��4λ
					}
					LcdWriteData(CN16CHAR[wordNum].Msk[j]);
				}
				x += 16;
			}//if�鵽�ֽ���		
		} //for���ֽ���	
		cn += 2;
	}	//while����
	return 1;
}

//�����ַ���������λ��,δ�ҵ��ͷ��ؿո�
int SearchEnPosition(unsigned char value)
{
	int i=0;
	while(EN16CHAR[i].Index != ' ')
	{
		if(EN16CHAR[i].Index == value) return i;
		i++;
	}
	return i;
}

//��ʾһ��Ӣ���ַ�
void ShowEnChar(unsigned char x, unsigned char y, unsigned char en)	
{  
	unsigned char j, x1, x2, wordNum,y1;
	
	y1 = y;
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y1 > 7)
	{
		return;
	}

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
	if(x > 128)
	{
		return;
	}
	y1 += 0xB0;	   //��ȡY�����ֵ
	//--����Y����--//
	LcdWriteCmd(y1);

	x1 = (x >> 4) & 0x0F;   //����X����Ҫ��������ָߵ�4λ������������ȡ����4λ
	x2 = x & 0x0F;          //ȥ����λ
	//--����X����--//
	LcdWriteCmd(0x10 + x1);   //��4λ
	LcdWriteCmd(x2);	//��4λ
	//--��ѯҪд�������ֿ��е�λ��--//
	wordNum = SearchEnPosition(en);		    
	for (j=0; j<16; j++) //дһ����
	{		
		if (j == 8)	 //����16X16�õ�����Y���꣬�����ڵ���16ʱ���л�����
		{
			//--����Y����--//
			LcdWriteCmd(y1 + 1);
			
			//--����X����--//
			LcdWriteCmd(0x10 + x1);   //��4λ
			LcdWriteCmd(x2);	//��4λ
		}
		LcdWriteData(EN16CHAR[wordNum].Msk[j]);
	}
}

/*******************************************************************************
* �� �� �� ��ShowEnString
* �������� ����ָ��λ��д��Ӣ���ֵ��ַ��� 8*16
* ��    �� ��x x����(0,127)   ,   y y����(0,3)  ��  str Ҫд����ַ���
* ��    �� ����
*******************************************************************************/
void ShowEnString(unsigned int x,unsigned int y,unsigned char *str)
{  
	unsigned int x1,y1;
	x1=x;
	y1=y;
	while(*str!='\0')
	{	
		ShowEnChar(x1,y1*2,*str);
		x1+=8;
		if(x1+8 > 128)//����
		{
			x1 = x;
			y1 = y1+1;
		}
		str++;
	}	
}

/*******************************************************************************
* �� �� �� ��LCD12864WriteCommand
* �������� ����ʾָ���Ļ U D L R S 
* ��    �� ��comm �ַ���ʽ
* ��    �� ����
*******************************************************************************/
void LCD12864WriteCommand(char comm)
{
	ShowEnString(9*8,3,(unsigned char *)&comm);
}

void LCD12864WriteDistance(unsigned int distance)
{
	char data_buf[4];
	if(distance >150)
	{
		ShowEnString(5*8,1,"---");
		return;
	}	
	sprintf(data_buf,"%03d",(int)distance);
	data_buf[3] = 0;
	ShowEnString(5*8,1,(unsigned char *)data_buf);
}

void LCD12864WriteDistanceSafe(unsigned int distance)
{
	distance = distance;
//	char data_buf[12];
//	
//	sprintf(data_buf,"Ds:%03dm   ",(int)distance);
//	data_buf[11] = 0;
//	ShowEnString(4*8,2,(unsigned char *)data_buf);
	ShowEnString(3*8,2,"          ");
}

void LCD12864WriteSpeed(unsigned int speed)
{
	char data_buf[4];
	sprintf(data_buf,"%03d",(int)speed);
	data_buf[3] = 0;
	ShowEnString(5*8,0,(unsigned char *)data_buf);
}

void LCD12864WriteParam(unsigned char k_rate , unsigned char b_rate)
{
	char data_buf[4];
//	char index;
	sprintf(data_buf,"%03d",(int)k_rate);
	data_buf[3] = 0;
	ShowEnString(4*8,3,(unsigned char *)data_buf);	
	
//	for(index=0;index<4;index++)
//	data_buf[index] = 0;
	sprintf(data_buf,"%03d",(int)b_rate);
	data_buf[3] = 0;
	ShowEnString(11*8,3,(unsigned char *)data_buf);	
}

void LCD12864WriteWarn(void)
{
	ShowEnString(3*8,2,"Warning!!!");
}
