#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

uint8_t SPI_ReadWrite_8Byte(SPI_TypeDef* SPIx, uint8_t TxData);
uint16_t SPI_ReadWrite_16Byte(SPI_TypeDef* SPIx, uint16_t TxData);

#ifdef __cplusplus
}
#endif
#endif

