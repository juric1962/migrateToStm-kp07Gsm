#ifndef FM25W256_SPI_H
#define FM25W256_SPI_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void FM25W256_SPI_Init(void);

void RdFromFleshToArr(uint16_t adres_flesh, unsigned char *adres_ozu,
                      uint16_t num);

void RdFromFleshToArrInt(uint16_t adres_flesh, uint16_t *adres_ozu,
                         uint16_t num);

void WrArrayToFlesh(uint16_t adres_flesh, unsigned char *adres_ozu,
                    uint16_t num, unsigned char flag,
                    unsigned char znach);

void WrArrayToFleshInt(uint16_t adres_flesh, uint16_t *adres_ozu,
                       uint16_t num, unsigned char flag,
                       uint16_t znach);

#ifdef __cplusplus
}
#endif

#endif /* FM25W256_SPI_H */
