
#define ST_ALARM 0
#define ST_VZAT  1
#define ST_VZATIE 2

#define EV_TC_ALARM 0
#define EV_TC_NORMA 1
#define EV_TM_OUT 2

extern void monitor_change_state(void);
//void monitor_rst(void);
unsigned int read_from_buf(unsigned char *p_buf,unsigned int *ptr_in,unsigned int *ptr_out,unsigned int *ptr_out_kv,unsigned int *crc,unsigned char *p_data,unsigned int max_buf);
unsigned char write_to_buf(unsigned char *p_buf,unsigned int *ptr_in,unsigned int *ptr_out,unsigned int *ptr_out_kv,unsigned int *crc,unsigned char *p_data,unsigned int l_data, unsigned int max_buf);




