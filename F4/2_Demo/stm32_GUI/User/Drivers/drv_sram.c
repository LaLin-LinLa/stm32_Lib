#include "drv_sram.h"
#include "drv_LCD.h"

//ʹ��NOR/SRAM�� Bank1.sector3,��ַλHADDR[27,26]=10 
//��IS61LV25616/IS62WV25616,��ַ�߷�ΧΪA0~A17 
//��IS61LV51216/IS62WV51216,��ַ�߷�ΧΪA0~A18
#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	


/**
	* @brief  ��ָ����ַ(WriteAddr+Bank1_SRAM3_ADDR)��ʼ,����д��n���ֽ�.
	* @param	pBuffer:�ֽ�ָ��
	* @param	WriteAddr:Ҫд��ĵ�ַ
	* @param	n:Ҫд����ֽ���
	* @retval None
	*/
void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*(vu8*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

/**
	* @brief  ��ָ����ַ((WriteAddr+Bank1_SRAM3_ADDR))��ʼ,��������n���ֽ�.
	* @param	pBuffer:�ֽ�ָ��
	* @param	ReadAddr:Ҫ��������ʼ��ַ
	* @param	n:Ҫ�������ֽ���
	* @retval None
	*/
void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*pBuffer++=*(vu8*)(Bank1_SRAM3_ADDR+ReadAddr);
		ReadAddr++;
	}
} 


void FSMC_SRAM_Test(uint16_t x,uint16_t y)
{
    uint8_t writeData = 0xf0, readData;
    uint16_t cap=0;
    uint32_t addr;  
    addr = 1024;    //��1KBλ�ÿ�ʼ����    
    LCD_ShowString(x,y,239,y+16,16,"ExSRAM Cap:   0KB"); 

    while(1){
        FSMC_SRAM_WriteBuffer(&writeData, addr, 1);
        FSMC_SRAM_ReadBuffer(&readData,addr,1);

        if(readData == writeData){  //�������������Ƿ���д�������һ��
            cap++;      //�����ͬ��ʾд��/�����ɹ���������1����λKB��
            addr += 1024;   //��ַ��1024
            readData = 0;
            if(addr > 256 * 1024)   //IS62WV12816������Ϊ256KB
                break;   
        }
        else
            break;  
    }
    LCD_ShowxNum(x+11*8,y,cap,4,16,0);  //LCD����ʾ�ڴ�����
//    printf("SRAM Menmory Size:%dKB\r\n",cap);
}
