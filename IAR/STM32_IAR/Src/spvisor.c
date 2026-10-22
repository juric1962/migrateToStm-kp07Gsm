
#include "def_log.h"
//#include <inavr.h>
//#include <iom2560.h>

#define DEF_SUPERVISOR 3600

extern void s_port(unsigned char ch);
void write_log_info(unsigned char sost, unsigned char mesto);
void lock_it(void);
void send_info(char size, char const *p, unsigned char fl_id,
               unsigned char id);

uint16_t cnt_supervosor;

const char evc_rld_sv[] = {
    'E', 'V', 'C', ':', 'r', 'l', 'd', ' ', 's', 'v',
};

void supervisorwdt(void) {
  if (cnt_supervosor >= DEF_SUPERVISOR) {
    send_info(sizeof(evc_rld_sv), evc_rld_sv, 0, 0);
    write_log_info(ST_ERROR, ERR7);
    // s_port(0x58);
    lock_it();
  }
}
