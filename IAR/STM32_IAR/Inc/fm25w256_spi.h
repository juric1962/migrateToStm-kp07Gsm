#ifndef FM25W256_SPI_H
#define FM25W256_SPI_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void FM25W256_SPI_Init(void);

void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                      unsigned int num);

void RdFromFleshToArrInt(unsigned int adres_flesh, unsigned int *adres_ozu,
                         unsigned int num);

void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                    unsigned int num, unsigned char flag,
                    unsigned char znach);

void WrArrayToFleshInt(unsigned int adres_flesh, unsigned int *adres_ozu,
                       unsigned int num, unsigned char flag,
                       unsigned int znach);

#ifdef __cplusplus
}
#endif

#endif /* FM25W256_SPI_H */
