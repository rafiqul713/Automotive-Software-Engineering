#include "init.h"
/********************************************************************
*                          global variables                         *
********************************************************************/

/********************************************************************
*                    _____  ___  ___   ___                          *
*                   |_   _|/ _ \|   \ / _ \                         *
*                     | | | (_) | |) | (_) |                        *
*                     |_|  \___/|___/ \___/                         *
*                                                                   *
* If you need global variables define them here!                    *
********************************************************************/  


 
#define DOOR_ID 0x102 // Receive from ECU 0
#define SPEED_ID 0x502 // Receive from ECU 4
#define ECU4_ALIVE 0x504 //Receive from ECU 4

int Left_Door=1;
int Right_Door=1;
int Door_Status=1;


int every_200_ms_interval = 0;
int every_400_ms_interval = 0; 
unsigned char indicatorSwitches = 7; //00000111
unsigned char doors_status = 3; //00000011
int speed = 0;
int engineControlIsAlive = 1;

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();

    /* turn off leds */
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1; 

    /********************************************************************
    *                    _____  ___  ___   ___                          *
    *                   |_   _|/ _ \|   \ / _ \                         *
    *                     | | | (_) | |) | (_) |                        *
    *                     |_|  \___/|___/ \___/                         *
    *                                                                   *
    * Some configuration/initialisation must                            *
    * be done outside of the for-loop....(!)                            *
    ********************************************************************/  
    //configure for 200 and 400 ms
    PIT_ConfigureTimer(0, 200);
	PIT_ConfigureTimer(1, 400);

	//start PIT timer for channel 0 and 1
	PIT_StartTimer(0);
	PIT_StartTimer(1);  

    /* forever */
    for(;;)
    {
        /********************************************************************
        *                    _____  ___  ___   ___                          *
        *                   |_   _|/ _ \|   \ / _ \                         *
        *                     | | | (_) | |) | (_) |                        *
        *                     |_|  \___/|___/ \___/                         *
        *                                                                   *
        * Write down your logic here.                                       *
        ********************************************************************/
    	if(every_200_ms_interval) {

			//Send fuel level information

			//get potentiometer value which is 10 bit
			CAN_0.BUF[0].DATA.B[0] = ADC_0.CDR[4].B.CDATA; // first 8 bit
			CAN_0.BUF[0].DATA.B[1] =  ADC_0.CDR[4].B.CDATA >> 8; // last 2 bit		
			CAN_0.BUF[0].CS.B.CODE = 0x0C;
			
			/*
				PCR[44] SW1
				PCR[4] SW2
				PCR[3] SW3
				PCR[2] SW4

				if(SIU.GPDI[44].R==0) or if(SW1==0)
			*/

			//11111110
			//00000111

			/*
				bit 0 -> left
				bit 1 -> right
				bit 2 -> hazard
			*/

			//if switch 1 is pressed
			if(SW1==0){
				indicatorSwitches &= 254; //(11111110& 00000111 = 110 or 6) 
			}
			else {
				indicatorSwitches |= 1; //00000001
			}
			if(SW3==0){
				indicatorSwitches &= 253; //11111101
			}
			else {
				indicatorSwitches |= 2; //00000010
			}
			if(SW4==0){
				indicatorSwitches &= 251; //11111011
			}
			else {
				indicatorSwitches |= 4; //00000100
			}
			
			// send indicator switches information
			CAN_0.BUF[1].DATA.B[0] = indicatorSwitches;
			CAN_0.BUF[1].CS.B.CODE = 0xC;
			
			// send alive signal
			CAN_0.BUF[3].CS.B.CODE = 0xC;
			
			// send error message
			//not alive
			if(engineControlIsAlive == 0) {
				CAN_0.BUF[4].CS.B.CODE = 0xC;
				LED7 = ~LED7;
			} 
			//alive
			else {
				engineControlIsAlive = 0;
				LED7 = 1;
			}
			
			every_200_ms_interval = 0;
		} 
		
		//0 position bit-> warning
		//1 position bit -> left
		//2 position bit-> right
		//0 door is OPEN
		//1 door is CLOSED
		if(every_400_ms_interval) {
			// sending doors status message
			//speed > 5 and door is open
			// if( (speed > 5) && (doors_status != 3) ){
			// 	CAN_0.BUF[2].DATA.B[0] = doors_status << 1;
			// }
			// else{
			// 	CAN_0.BUF[2].DATA.B[0] = (doors_status << 1) | 1;
			// }


		 	if(speed > 5 && (Left_Door==0||Right_Door==0)){
				Door_Status= 0; // Warning ON 
			}

			else {
				Door_Status=1; //Warning OFF
			}


			CAN_0.BUF[2].DATA.B[0]=(Door_Status|(Left_Door<<1)|(Right_Door<<2)); 
			CAN_0.BUF[2].CS.B.CODE = 0xC;
			every_400_ms_interval = 0;
		}


    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 ********************************************************************/  

//execute every 200 ms
void PITCHANNEL0(void)
{
	every_200_ms_interval = 1;
    /* your own code above! */
    PIT.CH[0].TFLG.B.TIF = 1;
}

//execute every 400
void PITCHANNEL1(void)
{
	every_400_ms_interval = 1;
    /* your own code above! */
    PIT.CH[1].TFLG.B.TIF = 1;
}

void CANMB0003(void)
{
/* No modifications needed here */
/* Receive interrupts are being cleared here */
    CAN_0.IFRL.B.BUF00I = 1;
    CAN_0.IFRL.B.BUF01I = 1;
    CAN_0.IFRL.B.BUF02I = 1;
    CAN_0.IFRL.B.BUF03I = 1;
}

void CANMB0407(void)
{

    /********************************************************************
    *                    _____  ___  ___   ___                          *
    *                   |_   _|/ _ \|   \ / _ \                         *
    *                     | | | (_) | |) | (_) |                        *
    *                     |_|  \___/|___/ \___/                         *
    *                                                                   *
    * CAN reception is handled here                                     *
    * The following buffers are important:                              *
    * CAN_0.RXFIFO.ID.B.STD_ID: ID of received message                  *
    * CAN_0.RXFIFO.DATA.B[i]: value of data byte 'i'                    *
    * IMPORTANT: check for the flag in CAN_0.IFRL.B.BUF05I first!       *
    ********************************************************************/  
   	if(CAN_0.IFRL.B.BUF05I) {
		switch(CAN_0.RXFIFO.ID.B.STD_ID) {
			case DOOR_ID:
				//doors_status = CAN_0.RXFIFO.DATA.B[0]; //or CAN_0.BUF[0].DATA.B[0]
				//LED0 = ~LED0;

				//status of left and right door
				Left_Door = (CAN_0.RXFIFO.DATA.B[0]& 0b00000001);
				Right_Door= (CAN_0.RXFIFO.DATA.B[0]& 0b00000010);	
				break;
			case SPEED_ID:
				speed = (CAN_0.RXFIFO.DATA.B[1] << 8) | CAN_0.RXFIFO.DATA.B[0];
				//LED1 = ~LED1;
				break;
			case ECU4_ALIVE:
				engine_control_is_alive = 1;
				//LED2 = ~LED2;
				break;
		}
	}  

    /* end of own code! */
    /* clear flags as last step here! */
    /* don't change anything below! */
    CAN_0.IFRL.B.BUF04I = 1;
    CAN_0.IFRL.B.BUF05I = 1;
    CAN_0.IFRL.B.BUF06I = 1;
    CAN_0.IFRL.B.BUF07I = 1;
}

void CANMB0811(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */

    CAN_0.IFRL.B.BUF08I = 1;
    CAN_0.IFRL.B.BUF09I = 1;
    CAN_0.IFRL.B.BUF10I = 1;
    CAN_0.IFRL.B.BUF11I = 1;
}

void CANMB1215(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */
    CAN_0.IFRL.B.BUF12I = 1;
    CAN_0.IFRL.B.BUF13I = 1;
    CAN_0.IFRL.B.BUF14I = 1;
    CAN_0.IFRL.B.BUF15I = 1;
}



/********************************************************************
 *                   Interrupt Vector Table                         *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            PITCHANNEL0();
            break;
        case 60:
            PITCHANNEL1();
        case 68:
            CANMB0003();
            break;
        case 69:
            CANMB0407();
            break;
        case 70:
            CANMB0811();
            break;
        case 71:
            CANMB1215();
            break;        
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
