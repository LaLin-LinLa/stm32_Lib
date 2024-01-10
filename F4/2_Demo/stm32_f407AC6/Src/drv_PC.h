#ifndef __DRV_PC_H
#define __DRV_PC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "system.h"

/*����PC���ݰ�*/
typedef __PACKED_STRUCT
{
	/*���ݽ��հ����û��Զ���*/
	uint8_t head;
	float pitch[4];
	float yaw[4];
	float distance;
	uint8_t shoot_flag;
	uint8_t end;

} PC_rx_Pack_t;
/*������λ�����ݰ�*/
typedef __PACKED_STRUCT
{
	/*���ݷ��Ͱ����û��Զ���*/
	float data[8];			//��������
	uint8_t	tail[4];		//��β����
	
} PC_tx_Pack_t;

/*������λ��������*/
typedef __PACKED_UNION
{
	PC_tx_Pack_t	txpack;
	unsigned char PcDataArray[sizeof(PC_tx_Pack_t)];
} PC_tx_Union_t;
/*������λ��������*/
typedef __PACKED_UNION
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
