#include "def_prot.h"
#include "sec.h"
#include <iom2560.h>

extern unsigned int crc_m1(unsigned char *ka, unsigned int num,
                           unsigned int crc);

#define APPLICATION_ADDRESS 0
typedef void (*pFunction)(void);
pFunction Jump_To_Application;
unsigned int JumpAddress;

void startNewProgram(void) {
  //	JumpAddress = *( unsigned  int*) (APPLICATION_ADDRESS + 4);
  JumpAddress = (APPLICATION_ADDRESS + 4);
  Jump_To_Application = (pFunction)JumpAddress;
  //   __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
  Jump_To_Application();
}

unsigned char write_to_buf(unsigned char *p_buf, unsigned int *ptr_in,
                           unsigned int *ptr_out, unsigned int *ptr_out_kv,
                           unsigned int *crc, unsigned char *p_data,
                           unsigned int l_data, unsigned int max_buf) {
  unsigned int temp, i, j = 0;

  if (*crc != crc_m1(p_buf, max_buf, 0xffff)) {
    *ptr_out = 0;
    *ptr_in = 0;
    *ptr_out_kv = 0;
  }

  if (*ptr_in >= *ptr_out) {
    if ((*ptr_out == 0) && (*ptr_in == (max_buf - 1)))
      return (1); // выход буфер переполнен
    temp = (max_buf - 1) - (*ptr_in);
    temp = temp + (*ptr_out);
    if (l_data > temp)
      return (1); // нельзя записать - переполнение
    temp = (*ptr_in) + l_data;
    if (temp > max_buf) {
      for (i = (*ptr_in); i < max_buf; i++) {
        *(p_buf + i) = *(p_data + j);
        j++;
      }
      (*ptr_in) = temp - max_buf;
      for (i = 0; i < (*ptr_in); i++) {
        *(p_buf + i) = *(p_data + j);
        j++;
      };
    } else {
      for (i = (*ptr_in); i < temp; i++) {
        *(p_buf + i) = *(p_data + j);
        j++;
      }
      if (temp == max_buf)
        (*ptr_in) = 0;
      else
        (*ptr_in) = temp;
    }
    *crc = crc_m1(p_buf, max_buf, 0xffff); // запись CRC
  }

  else {
    if ((*ptr_out - *ptr_in) == 1)
      return (1); // выход буфер переполнен
    temp = (*ptr_out) - (*ptr_in) - 1;
    if (l_data > temp)
      return (1); // нельзя записать - переполнение
    temp = (*ptr_in) + l_data;
    for (i = (*ptr_in); i < temp; i++) {
      *(p_buf + i) = *(p_data + j);
      j++;
    }
    (*ptr_in) = temp;
    *crc = crc_m1(p_buf, max_buf, 0xffff); // запись CRC
  }
  return (0);
}

unsigned int read_from_buf(unsigned char *p_buf, unsigned int *ptr_in,
                           unsigned int *ptr_out, unsigned int *ptr_out_kv,
                           unsigned int *crc, unsigned char *p_data,
                           unsigned int max_buf) {
  unsigned int i, j = 0;

  if (*ptr_in == *ptr_out)
    return (0);

  if (*crc != crc_m1(p_buf, max_buf, 0xffff)) {
    *ptr_out = 0;
    *ptr_in = 0;
    *ptr_out_kv = 0;
    return (0);
  }

  if (*ptr_in > *ptr_out) {
    for (i = (*ptr_out); i < *ptr_in; i++) {
      *(p_data + j) = *(p_buf + i);
      j++;
    }
    *ptr_out_kv = *ptr_in; // это нужно при приеме квитка, при преме квитка
                           // нужно p_out=p_out_kv
    return (j);
  } else {
    for (i = (*ptr_out); i < max_buf; i++) {
      *(p_data + j) = *(p_buf + i);
      j++;
    }
    for (i = 0; i < (*ptr_in); i++) {
      *(p_data + j) = *(p_buf + i);
      j++;
    }
    *ptr_out_kv = *ptr_in;
    return (j);
  }
}
