#include "init.h"

void showData(int value);

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
    //BT5 A[1] PCR[1]
    //BT6 A[0] PCR[0]
    //SW1 C[12] PCR[44]

    // Configure button as input
    SIU.PCR[1].R=0X100;
    SIU.PCR[0].R=0X100;

    //SW1 as input
    //SIU.PCR[44].R=0X100;
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

    /* forever */
    for(;;)
    {
    	//Check button BT5 press
    	if(SIU.GPDI[1]==0)
    	{
    		//LED 5 ON and LED 6 OFF
    		SIU.GPDO[61].R=0;
    		SIU.GPDO[59].R=1;
    	}
    	//Check button BT6 press
    	if(SIU.GPDI[0]==0)
    	{
    		//LED 5 OFF and LED 6 ON
    		SIU.GPDO[61].R=1;
    		SIU.GPDO[59].R=0;
    	}
    	else 
    	{
    		//if no button is pressed LED 5 and LED 6 OFF 
    		SIU.GPDO[61].R=1;
    		SIU.GPDO[59].R=1;
    	}
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

/********************************************************************
 *                      Interrupt Functions                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                Interrupts can be handled below.                  *
 ********************************************************************/  
void PITCHANNEL0() {
    /* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1;
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
