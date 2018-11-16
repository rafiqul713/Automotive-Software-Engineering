#include "init.h"

/********************************************************************
 *                              main                                *
 ********************************************************************/
int speed=0, flag=0;
void main(void)
{
    /* board initialization */
	Init();

    //Configure Time Channel 0
	PIT_ConfigureTimer(0, 1000);
    //Start timer channel 0
	PIT_StartTimer(0);

	//Sub Task 3
	//Configure Time Channel 0
	PIT_ConfigureTimer(0,200);
    //Start timer channel 0
	PIT_StartTimer(0);

    /* forever */
	for(;;)
	{
/********************************************************************
 *                           Main Loop                              *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                  Write down your logic here.                     *
 ********************************************************************/  
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

//Task 1 LED0 will be blinked
void PITCHANNEL0() {
	/* clear flag */
	PIT.CH[0].TFLG.B.TIF = 1;
	//Blinking LED0
	LED0 = ~LED0;
}

// Task 2 Subtask 2
//Configure Interrupt Service Routine for buffer 0 to 15
//Clear Flag 
void FLEXCAN_BUF_00_03(){
	CAN_0.IFRL.B.BUF00I =1;
	CAN_0.IFRL.B.BUF01I =1;
	CAN_0.IFRL.B.BUF02I =1;
	CAN_0.IFRL.B.BUF03I =1;    
}


// Task 3 Sub Task 1
void FLEXCAN_BUF_04_07(){
	if(CAN_0.IFRL.B.BUF05I==1){
		
			if(CAN_0.RXFIFO.ID.B.STD_ID == 0xFF)
			{
				LED1 = ~LED1;
                //LED2 = 0;

			}
			if(CAN_0.RXFIFO.ID.B.STD_ID == 0x01)
			{
				LED2 = ~LED2;


			}

	}

	CAN_0.IFRL.B.BUF04I=1;
	CAN_0.IFRL.B.BUF05I=1;
	CAN_0.IFRL.B.BUF06I=1;
	CAN_0.IFRL.B.BUF07I=1;
}

void FLEXCAN_BUF_08_11(){
	CAN_0.IFRL.B.BUF08I =1;
	CAN_0.IFRL.B.BUF09I =1;
	CAN_0.IFRL.B.BUF10I =1;
	CAN_0.IFRL.B.BUF11I =1;
}
void FLEXCAN_BUF_12_15(){
	CAN_0.IFRL.B.BUF12I =1;
	CAN_0.IFRL.B.BUF13I =1;
	CAN_0.IFRL.B.BUF14I =1;

	CAN_0.IFRL.B.BUF15I =1;
}

/* ... ISRs ... */



// Task 2 Subtask 3
void PIT_Timer0(){
	PIT.CH[0].TFLG.B.TIF=1; /* Clear Flag*/

	//LED0 = ~LED0;
	CAN_0.BUF[0].CS.B.CODE = 0b1100; // Transmit remote frame
	CAN_0.BUF[0].CS.B.IDE =0;
	CAN_0.BUF[0].CS.B.RTR = 0;
	/* STD_ID */
    //CAN_0.BUF[0].ID.B.STD_ID = ; // IP
    /* Data Length */

    CAN_0.BUF[0].CS.B.LENGTH =0x04 ; //sending 4 byte
	CAN_0.BUF[0].DATA.B[0] = speed & 0xFF; 
	CAN_0.BUF[0].DATA.B[1] = (speed >> 8) & 0b00000001;

    


	else if(speed>=300)
	{
		flag=1;
	}
	else if(speed<=0)

	{
		flag=0;
	}

	if(flag==1)
	{
		speed=speed-5;
	}
	else if(flag==0)
	{
		speed=speed+5;
	}

}


/********************************************************************
 *                   Interrupt Vector Table                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
	switch(INTC.IACKR.B.INTVEC)
	{
    /* ... ISR calls should be here ... */
	//59 IRQ is used for PIT Channel 0
		case 59:
		{
		/* Timer Interrupt */
			PITCHANNEL0();
			break;
		}

		case 59:
		{
			PIT_Timer0();
			break;
		}

		case 68:
		{
			FLEXCAN_BUF_00_03();
			break;
		}
		case 69:
		{
			FLEXCAN_BUF_04_07();
			break;
		}
		case 70:
		{
			FLEXCAN_BUF_08_11();
			break;
		}
		case 71: 
		{
			FLEXCAN_BUF_12_15();
			break;
		}
		default:
		break;
	}
    /* End of Interrupt Request */
	INTC.EOIR.R = 0x00000000;
}
