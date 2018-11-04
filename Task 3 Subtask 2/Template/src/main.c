#include "init.h"

void showData(int value);

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{

	/*
	LED related PIN no

	LED 0 A[9] PCR[9]
	LED 1 C[10] PCR[42]
	LED 2 A[13] PCR[13]
	LED 3 A[12] PCR[12]
	LED 4 D[14] PCR[62]
	LED 5 D[13] PCR[61]
	LED 6 D[11] PCR[59]
	LED 7 A[11] PCR[11]
  */	 

    Init();
    /* board initialization */
    //PIT_ConfigureTimer( int timerChannel, unsigned int time )
	//Timer is configured here
    PIT_ConfigureTimer(0,1000);
    //LED 7 off
    SIU.GPDO[11].R=1;
    //SW1 C[12] PCR[44]
    //SW1 as input
    SIU.PCR[44].R=0X100;
    /* forever */
    for(;;)
    {
       //Switch is pressed set timer	
       if(SIU.GPDI[44].R == 0)
       {
       		PIT_StartTimer(0);
       }
       //Switch is off stop timer
       else if(SIU.GPDI[44].R==1)
       {
        	PIT_StopTimer(0);
        	//LED 7 OFF
        	SIU.GPDO[11].R = 1;
       }
}

void showData(int value)
{
/********************************************************************
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/  
  if(value <= 256)
  {   
    /* LED0 - on, LED1 - off, LED2 - off, LED3 - off */
  }
  else if(value > 256 && value <= 512)
  {  
    /* LED0 - on, LED1 - on, LED2 - off, LED3 - off */
  }
  else if(value > 512 && value <= 768)
  {  
    /* LED0 - on, LED1 - on, LED2 - on, LED3 - off */
  }    
  else
  {  
    /* LED0 - on, LED1 - on, LED2 - on, LED3 - on */
  }     
}


void PITCHANNEL0() {
    /* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1;
    //LED 7 is toggled 
    SIU.GPDO[11].R = ~SIU.GPDO[11].R;

}

void PITCHANNEL1() {
    /* clear flag */
    PIT.CH[1].TFLG.B.TIF = 1;
}

/********************************************************************
 *                   Interrupt Vector Table                         *
 *                                                                  *
 *                 Don't touch anything below!                      *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            /* Timer Interrupt */
            PITCHANNEL0();
            break;
        case 60:
            /* Timer Interrupt */
            PITCHANNEL1();
            break;            
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
