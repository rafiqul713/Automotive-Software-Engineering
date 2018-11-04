#include "init.h"

void showData(int value);
 int counter=0;
/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
    //Configure Timer
    PIT_ConfigureTimer(0,1000);
    //Configure channel for timer
    PIT_StartTimer(0);
    /* forever */
    for(;;)
    {
    	if(counter == 0)
    	{
            SIU.GPDO[9].R = 0;
            SIU.GPDO[42].R = 1;
            SIU.GPDO[13].R = 1;
            SIU.GPDO[12].R = 1;
            SIU.GPDO[62].R = 1;
            SIU.GPDO[61].R = 1;
            SIU.GPDO[59].R = 1;
            SIU.GPDO[11].R = 1;
        }

        else if(counter == 1)
        {
            SIU.GPDO[9].R = 0;
            SIU.GPDO[42].R = 0;
            SIU.GPDO[13].R = 1;
            SIU.GPDO[12].R = 1;
            SIU.GPDO[62].R = 1;
            SIU.GPDO[61].R = 1;
            SIU.GPDO[59].R = 1;
            SIU.GPDO[11].R = 1;
        }
        else if(counter == 2)
        {
            SIU.GPDO[9].R = 0;
            SIU.GPDO[42].R = 0;
            SIU.GPDO[13].R = 0;
            SIU.GPDO[12].R = 1;
            SIU.GPDO[62].R = 1;
            SIU.GPDO[61].R = 1;
            SIU.GPDO[59].R = 1;
            SIU.GPDO[11].R = 1;
        }
        else if(counter == 3)
        {
            SIU.GPDO[9].R = 0;
            SIU.GPDO[42].R = 0;
            SIU.GPDO[13].R = 0;
            SIU.GPDO[12].R = 0;
            SIU.GPDO[62].R = 1;
            SIU.GPDO[61].R = 1;
            SIU.GPDO[59].R = 1;
            SIU.GPDO[11].R = 1;
        }
        else if(counter == 4)
        {
            SIU.GPDO[9].R = 0;
            SIU.GPDO[42].R = 0;
            SIU.GPDO[13].R = 0;
            SIU.GPDO[12].R = 0;
            SIU.GPDO[62].R = 0;
            SIU.GPDO[61].R = 1;
            SIU.GPDO[59].R = 1;
            SIU.GPDO[11].R = 1;
        }
        else if(counter == 5)
        {
            SIU.GPDO[9].R = 0;
            SIU.GPDO[42].R = 0;
            SIU.GPDO[13].R = 0;
            SIU.GPDO[12].R = 0;
            SIU.GPDO[62].R = 0;
            SIU.GPDO[61].R = 0;
            SIU.GPDO[59].R = 1;
            SIU.GPDO[11].R = 1;
        }
        else if(counter == 6)
        {
            SIU.GPDO[9].R = 0;
            SIU.GPDO[42].R = 0;
            SIU.GPDO[13].R = 0;
            SIU.GPDO[12].R = 0;
            SIU.GPDO[62].R = 0;
            SIU.GPDO[61].R = 0;
            SIU.GPDO[59].R = 0;
            SIU.GPDO[11].R = 1;
        }
        else if(counter == 7)
        {
            SIU.GPDO[9].R = 0;
            SIU.GPDO[42].R = 0;
            SIU.GPDO[13].R = 0;
            SIU.GPDO[12].R = 0;
            SIU.GPDO[62].R = 0;
            SIU.GPDO[61].R = 0;
            SIU.GPDO[59].R = 0;
            SIU.GPDO[11].R = 0;
        }
        else
        {
            SIU.GPDO[9].R = 1;
            SIU.GPDO[42].R = 1;
            SIU.GPDO[13].R = 1;
            SIU.GPDO[12].R = 1;
            SIU.GPDO[62].R = 1;
            SIU.GPDO[61].R = 1;
            SIU.GPDO[59].R = 1;
            SIU.GPDO[11].R = 1;
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


void PITCHANNEL0() {
    /* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1;
    if(counter<=7) {
        counter++;
    }
    else {
        counter = 0;
    }
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
