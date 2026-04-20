;sfrb SREG = 0x3F; 

#define  USB_INTR_PENDING = 0x3F
#define  USB_LOAD_PENDING(reg)   in reg, USB_INTR_PENDING