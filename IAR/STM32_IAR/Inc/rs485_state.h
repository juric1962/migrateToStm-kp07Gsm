#ifndef RS485_STATE_H
#define RS485_STATE_H

#include "ozu_map.h"

typedef struct {
  unsigned int cnt_bt_rx_tx;
  unsigned int cnt_tm_tx_out;
  unsigned int cnt_tm_pre_tx;
  unsigned int vol_tm_tx_out;
  unsigned int cnt_tm_rx_out;
  unsigned int vol_tm_rx_out;
  unsigned int cnt_tm_out;
  unsigned int vol_tm_out;
  unsigned char *p_data485;
} rs485_port_state_t;

typedef struct {
  unsigned char busy : 1;
  unsigned char rec : 1;
  unsigned char tm_out : 1;
  unsigned char tx : 1;
  unsigned char over : 1;
  unsigned char buffed : 1;
} rs485_port_flags_t;

extern rs485_port_state_t Rs485_2;
extern rs485_port_flags_t fl_485_2;
extern unsigned char Rs485_2_buf_rx_tx[MAX_BUF_RS485_2];

#endif /* RS485_STATE_H */
