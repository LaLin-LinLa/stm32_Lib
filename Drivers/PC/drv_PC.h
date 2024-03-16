#ifndef __DRV_PC_H
#define __DRV_PC_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

#define PC_LEN_rx								12u				//接收上位机数据长度
#define PC_HEAD_rx							0x55			//接收上位机数据包帧头
#define PC_TAIL_rx							0xAA			//接收上位机数据包帧尾

/*接收PC数据包*/
typedef __packed struct
{
	/*数据接收包，用户自定义*/
	uint8_t head;
	float pitch[4];
	float yaw[4];
	float distance;
	uint8_t shoot_flag;
	uint8_t end;

} PC_rx_Pack_t;
/*发送上位机数据包*/
typedef __packed struct
{
	/*数据发送包，用户自定义*/
	float data[8];			//整型数据
	uint8_t	tail[4];		//包尾数据
	
} PC_tx_Pack_t;

/*发送上位机共用体*/
typedef __packed union
{
	PC_tx_Pack_t	txpack;
	unsigned char PcDataArray[sizeof(PC_tx_Pack_t)];
} PC_tx_Union_t;
/*接收上位机共用体*/
typedef __packed union
{
	PC_rx_Pack_t	rxpack;
	unsigned char PcDataArray[sizeof(PC_rx_Pack_t)];
} PC_rx_Union_t;

void PC_DatePrcess(volatile const uint8_t * buf);
void PC_DateSend(PC_tx_Union_t *PC_tx);
extern const PC_rx_Pack_t *get_PC_point(void);

#ifdef __cplusplus
}
#endif
#endif
