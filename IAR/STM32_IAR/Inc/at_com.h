
typedef struct
{
unsigned char ok :1;//требуемый ответ
unsigned char err :1;//ошибочный, нетребуемый ответ
unsigned char tm_out :1;//отсутствие ответа
unsigned char tx_en :1;//послать комманду
unsigned char rx_en :1;//принимать ответы
unsigned char rx_rec :1;//принят ответ
unsigned char greg_ereg :1;//пакетник GPRS or LTE
//unsigned char tm_out_en:1;//разрешение анализа по превышению времени ожидания ответа
}
FL_AT_COM;
extern FL_AT_COM fl_at_com;
