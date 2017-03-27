//  name: customanos_pwm.h
/*
//  parameters: 1) desired_freq[Hz] --> epi8umiti frequency tou pwm pou paragetai
                2) desDuty[%]       --> epi8umito duty cycle
    outputs:    8a gurisw 3 pointers: a) pTBPRD: i timi tou TBPRD se Hz
                                      b) pTBCLK: i timi tou TBCLK se Hz
                                      c) pDUTY:  i timi tou COMPARE reg
*/
#include "soc_AM335x.h"                 //SOC_PWMSSx_REGS,SOC_GPIO_x_REGS
#include "pin_mux.h"                    //GPIO_#_## (GPIO_1_29)
#include "ehrpwm.h"
#include "hw_types.h"                   //auto kai to apo panw kaluptoun to bool.MIPWS PREPEI NA MPOUN STO customanos_pwm.h
#include "soc_AM335x.h"                 //auto exei tis d/nseis twn modules (blepe:Base addresses of PWMSS memory mapped registers.)
#include "hw_ehrpwm.h"
#include "gpio_v2.h"
#include "uartStdio.h"


//PROTOTYPING
//void custom_pwm(unsigned int freqnumber, unsigned short int desDuty, int table[4]);
//DEFINES
#define EHRPWM_INP_CLK_FREQ		        (1000000000)	          //input clock freq for the ehrPWM module. i timi den kserw giati einia tosi...
#define SYSCLKOUT                       (EHRPWM_INP_CLK_FREQ)
#define HSP                             (1)
#define CLKDIV                          (14)
//GLOBAL VARIABLES
double* pTBPRD;
double* pTBCLK;
double* pDUTY;
//DECLARING
double custom_pwm(int entoles[])
{
  double tpwm=0;                //einai sec
  double desired_freq = entoles[0];      //entoles[0] = commandade2get[0], einai Hz
  double desDuty = 0;
  //UARTprintf("DutyCycle=%d \n",(int) desDuty);
  double ftbclk = 0;            //einai Hz
  double tbclk_reg=0;           //einai sec
  double tbprd_reg=0;           //einai ka8aros #
  double reg2comp=0;            //i timi pou 8a parei to cmpa, ka8aros #
  pTBPRD = &tbprd_reg;
  pTBCLK = &ftbclk;
  pDUTY  = &reg2comp;                 //8ewrw pws auto einai to value tou CMPx me to opoio 8a ginei i sugkrisi
  //
  //UARTprintf("desired_freq=%d \n",(int) desired_freq);
  //UARTprintf("DutyCycle=%d \n",(int) desDuty);
  desDuty = (double) (entoles[1])/100;
  // !!!!!!!!!!!!!!!! EDW PREPEI ISWS NA KANW KATI PIO GENIKO GIA TO PWS EPILEGW hsp k clkdiv.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //twra to pairnw san sta8eres times. POU ORIZW TA hsp k clkdiv FYSIKA(dld ta pragmatika regs)--->TO KANEI I TimeBaseClkConfig
  ftbclk =(double) SYSCLKOUT/(HSP*CLKDIV); // einai se Hz, bres struct me tbclk
  UARTprintf("ftbclk=%d \n",(int) ftbclk);
  tbclk_reg =(double) 1/ftbclk;
  tpwm = 1/desired_freq;              // einai se sec.
  //
  if ((entoles[2] == 1) || (entoles[2] == 2))     //entoles[2]=countMode=up or down
  {
    tbprd_reg = (tpwm*ftbclk)-1;
    //UARTprintf("tpwm*tbclk-1\n");
  }
  else if (entoles[2] == 3)     //if updown
  {
    tbprd_reg = ftbclk/(2*desired_freq);
  }
  else
  {
    UARTprintf("WRONG VALUE \n");
  }
  // DUTY CYCLE
  if (entoles[3] == 1)
  {
    reg2comp = desDuty*tbprd_reg;
    // UARTprintf("1111reg2comp=%d \n",(int) reg2comp);
  }
  else if (entoles[3] == 0)
  {
    reg2comp = tbprd_reg*(1-desDuty);
  }
  else
  {
    UARTprintf("NOT A POSSIBILITY \n");
  }
  //DEBUGGING
  //UARTprintf("tbclk=%d \n",(int) tbclk_reg); einai 0,kati
  //UARTprintf("tbclk=%d \n", (int) *pTBCLK); einai 0,kati
  UARTprintf("tbprd=%d \n",(int) *pTBPRD);
  UARTprintf("tbprd=%d \n",(int) tbprd_reg);
  UARTprintf("reg2comp=%d \n", (int) reg2comp);
  return reg2comp;
}
