#define SCENARIO_TEST_MTC_ENABLE 
typedef struct
   {     
     char step;
     char enable;
     int delay;
   
   } TEST_MTC;
extern TEST_MTC test_mtc;
void scenario_test_mtc(void);
