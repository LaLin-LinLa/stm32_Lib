#ifndef __DRV_PC_H
#define __DRV_PC_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

#define PC_LEN_rx								12u				//������λ�����ݳ���
#define PC_HEAD_rx							0x55			//������λ�����ݰ�֡ͷ
#define PC_TAIL_rx							0xAA			//������λ�����ݰ�֡β

/*����PC���ݰ�*/
typedef __packed struct
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
typedef __packed struct
{
	/*���ݷ��Ͱ����û��Զ���*/
	float data[8];			//��������
	uint8_t	tail[4];		//��β����
	
} PC_tx_Pack_t;

/*������λ��������*/
typedef __packed union
{
	PC_tx_Pack_t	txpack;
	unsigned char PcDataArray[sizeof(PC_tx_Pack_t)];
} PC_tx_Union_t;
/*������λ��������*/
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
