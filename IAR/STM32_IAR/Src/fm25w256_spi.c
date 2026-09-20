#include "fm25w256_spi.h"
#include <stdlib.h>

#define FM25W256_SPI_INSTANCE SPI1
#define FM25W256_CS_PORT      GPIOA
#define FM25W256_CS_PIN       GPIO_PIN_15
#define FM25W256_TIMEOUT      1000u

#define FM25W256_CMD_WRITE_ENABLE  0x06u
#define FM25W256_CMD_WRITE_DISABLE 0x04u
#define FM25W256_CMD_READ_STATUS   0x05u
#define FM25W256_CMD_WRITE_STATUS  0x01u
#define FM25W256_CMD_READ          0x03u
#define FM25W256_CMD_FAST_READ     0x0Bu
#define FM25W256_CMD_PAGE_PROGRAM  0x02u

SPI_HandleTypeDef hspi1;

static void fm25w256_cs_low(void) {
  HAL_GPIO_WritePin(FM25W256_CS_PORT, FM25W256_CS_PIN, GPIO_PIN_RESET);
}

static void fm25w256_cs_high(void) {
  HAL_GPIO_WritePin(FM25W256_CS_PORT, FM25W256_CS_PIN, GPIO_PIN_SET);
}

static void fm25w256_write_enable(void) {
  uint8_t cmd = FM25W256_CMD_WRITE_ENABLE;
  fm25w256_cs_low();
  HAL_SPI_Transmit(&hspi1, &cmd, 1, FM25W256_TIMEOUT);
  fm25w256_cs_high();
}

static uint8_t fm25w256_read_status(void) {
  uint8_t tx = FM25W256_CMD_READ_STATUS;
  uint8_t rx = 0;

  fm25w256_cs_low();
  HAL_SPI_Transmit(&hspi1, &tx, 1, FM25W256_TIMEOUT);
  HAL_SPI_Receive(&hspi1, &rx, 1, FM25W256_TIMEOUT);
  fm25w256_cs_high();

  return rx;
}

static void fm25w256_wait_busy(void) {
  while ((fm25w256_read_status() & 0x01u) != 0u) {
  }
}

static void fm25w256_read_block(uint32_t address, uint8_t *buffer, uint16_t length) {
  uint8_t tx[4];
  tx[0] = FM25W256_CMD_READ;
  tx[1] = (uint8_t)((address >> 16) & 0xFFu);
  tx[2] = (uint8_t)((address >> 8) & 0xFFu);
  tx[3] = (uint8_t)(address & 0xFFu);

  fm25w256_cs_low();
  HAL_SPI_Transmit(&hspi1, tx, 4, FM25W256_TIMEOUT);
  HAL_SPI_Receive(&hspi1, buffer, length, FM25W256_TIMEOUT);
  fm25w256_cs_high();
}

static void fm25w256_write_page(uint32_t address, const uint8_t *buffer,
                                uint16_t length) {
  uint8_t tx[4];

  fm25w256_write_enable();
  fm25w256_wait_busy();

  tx[0] = FM25W256_CMD_PAGE_PROGRAM;
  tx[1] = (uint8_t)((address >> 16) & 0xFFu);
  tx[2] = (uint8_t)((address >> 8) & 0xFFu);
  tx[3] = (uint8_t)(address & 0xFFu);

  fm25w256_cs_low();
  HAL_SPI_Transmit(&hspi1, tx, 4, FM25W256_TIMEOUT);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, length, FM25W256_TIMEOUT);
  fm25w256_cs_high();

  fm25w256_wait_busy();
}

void FM25W256_SPI_Init(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_SPI1_CLK_ENABLE();

  GPIO_InitTypeDef gpio = {0};

  gpio.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOB, &gpio);

  gpio.Pin = FM25W256_CS_PIN;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  gpio.Alternate = 0;
  HAL_GPIO_Init(FM25W256_CS_PORT, &gpio);

  fm25w256_cs_high();

  hspi1.Instance = FM25W256_SPI_INSTANCE;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  HAL_SPI_Init(&hspi1);
}

void RdFromFleshToArr(unsigned int adres_flesh, unsigned char *adres_ozu,
                      unsigned int num) {
  unsigned int i;
  

  if (num == 0u) {
    return;
  }

  fm25w256_read_block((uint32_t)adres_flesh, adres_ozu, (uint16_t)num);

}

void RdFromFleshToArrInt(unsigned int adres_flesh, unsigned int *adres_ozu,
                         unsigned int num) {
  unsigned int i;
  uint8_t *dst = (uint8_t *)adres_ozu;
  uint16_t words = (uint16_t)num;

  if (words == 0u) {
    return;
  }

  fm25w256_read_block((uint32_t)adres_flesh, dst, (uint16_t)(words * 2u));

  for (i = 0u; i < num; ++i) {
    uint16_t value = (uint16_t)(dst[i * 2u] | ((uint16_t)dst[i * 2u + 1u] << 8));
    ((uint16_t *)adres_ozu)[i] = value;
  }
}

void WrArrayToFlesh(unsigned int adres_flesh, unsigned char *adres_ozu,
                    unsigned int num, unsigned char flag,
                    unsigned char znach) {
  unsigned int i;
  uint32_t address = (uint32_t)adres_flesh;

  if (num == 0u) {
    return;
  }

  if (flag == 0u) {
    uint16_t offset = 0u;
    while (offset < num) {
      uint16_t chunk = (uint16_t)((num - offset) > 64u ? 64u : (num - offset));
      fm25w256_write_page(address + offset, adres_ozu + offset, chunk);
      offset += chunk;
    }
  } else {
    
    fm25w256_write_page(address, &znach, (uint16_t)num);
    
  }
}

void WrArrayToFleshInt(unsigned int adres_flesh, unsigned int *adres_ozu,
                       unsigned int num, unsigned char flag,
                       unsigned int znach) {
  unsigned int i;
  uint32_t address = (uint32_t)adres_flesh;

  if (num == 0u) {
    return;
  }

  if (flag == 0u) {
    uint8_t *src = (uint8_t *)adres_ozu;
    uint16_t offset = 0u;
    while (offset < (uint16_t)(num * 2u)) {
      uint16_t chunk = (uint16_t)(((num * 2u) - offset) > 64u ? 64u : ((num * 2u) - offset));
      fm25w256_write_page(address + offset, src + offset, chunk);
      offset += chunk;
    }
  } else {
    fm25w256_write_page(address, &znach, (uint16_t)(num * 2u));
    
  }
}
