
#include <iom2560.h> 
#include "dfpin.h"
#include "dfcnst.h"


unsigned char state_led_md; 
unsigned int count_led;

extern unsigned char simka; //dobavka

void fun_state_md(void)
{
  count_led++;
  switch(state_led_md)
  {
  case LED_MD_OFF: S1_OFF; S5_OFF; break;
  case LED_MD_NO_CREG:  if(count_led<500){if(simka==SIM_BASE){S1_GR;}else {S1_RD;}}else {S1_YL;}break; //dobavka
  case LED_MD_YES_CREG: if(count_led<100){if(simka==SIM_BASE){S1_GR;}else {S1_RD;}}else {S1_OFF;}break;//dobavka
  case LED_MD_GPRS:if(count_led<500){if(simka==SIM_BASE){S1_GR;}else {S1_RD;}}else {S1_OFF;}break;//dobavka
  case LED_MD_PPP: if(simka==SIM_BASE){S1_GR;}else {S1_RD;};break;                                //dobavka
     
  }
  if(count_led>=1000)count_led=0;
}


