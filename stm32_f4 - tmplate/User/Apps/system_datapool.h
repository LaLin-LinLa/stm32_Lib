#ifndef _SYSTEM_DATAPOOL_H_
#define _SYSTEM_DATAPOOL_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define USART1_RX_BUFFER_SIZE 32
#define USART6_RX_BUFFER_SIZE 32

extern uint8_t Usart1_Rx_Buff[2][USART1_RX_BUFFER_SIZE];
extern uint8_t Usart6_Rx_Buff[USART6_RX_BUFFER_SIZE];

typedef struct
{
  uint8_t  port_num;
  int16_t  len;
  void*    address;
}USART_COB;

typedef struct{
  uint16_t  ID;
  uint8_t   DLC;
  void*    address;
}CAN_COB;


#ifdef __cplusplus
}
#endif
#endif

