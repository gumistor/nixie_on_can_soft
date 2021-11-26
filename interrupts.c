/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "nixieFunctions.h"
#include "kho_can.h"
#include "user.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{
     static uint8_t currentLamp = 0;
    //disable interrupts
    /*INTCON &= ~(0b0010000);
    //check TIMER0 interrupt
    if(INTCON & 0b00000100)
    {
        //clear TIMER0 flag
        INTCON &= ~(0b0000100);
        updateEnable |= UPDATE_LAMPS;
        setNumber(0xFF);
        setDot(0x00);
        setLamp(currentLamp);
        setNumber(lampsDigits[currentLamp]);
        setDot((lampsDots >> currentLamp) & 0x01);
        if(currentLamp > 4) currentLamp=0;
        else currentLamp++;
    } 
    else
    {

    }*/
    //enable interrupts
    //TMR0L = lampsRefreshRate;
    //INTCON |= 0b0010000;
    INTCON &= ~(0b00100000);        //disable interrupts from timer
    if(INTCON & 0b00000100)         //check TIMER0 interrupt
    {
        INTCON &= ~(0b00000100);    //clear TIMER0 flag
        systemTick++;                //update systems tick counter
        TMR0H = 0x00;               //reset timer register
        TMR0L = 0x5F;        //to rate 125ms
    }
    //enable interrupts
    INTCON |= 0b00100000;

    PIE1 &= ~0x01;
    if(PIR1 & 0x01)
    {
        T1CON = 0b00000000;
        updateEnable |= UPDATE_LAMPS;
        setNumber(0xFF);
        setDot(0x00);
        setLamp(currentLamp);
        setNumber(lampsDigits[currentLamp]);
        setDot((lampsDots >> currentLamp) & 0x01);
        if(currentLamp > 4) currentLamp=0;
        else currentLamp++;
        TMR1H = 0xEF;
        TMR1L = 0xD8;//lampsRefreshRate;
        PIR1 &= ~0x01;
    }
    PIE1 |= 0x01;
    T1CON = 0b00000001;
}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{
    PIE3 = 0x00;                                //dissable all interrupts
    if(PIR3 & 0b00000001) {                     //RX0 received
        doCANbuffer((_CANbuffer *)&RXB0CON);   //give pointer to function
        RXB0CON &= ~(0b10000000);               //erase receiver bit
        PIR3 &= ~(0b00000001);                  //clear interrupt
    }
    if(PIR3 & 0b00000010) {                     //RX1 received
        doCANbuffer((_CANbuffer *)&RXB1CON);   //give pointer to function
        RXB1CON &= ~(0b10000000);               //erase receiver bit
        PIR3 &= ~(0b00000010);                  //clear interrupt
    }
    if(PIR3 & 0b00000100) {                     //TX0 transmited
        PIR3 &= ~(0b00000100);                  //clear interrupt
    }
    if(PIR3 & 0b00001000) {                     //TX1 transmited
        PIR3 &= ~(0b00001000);                  //clear interrupt
    }
    if(PIR3 & 0b00010000) {                     //TX2 transmited
        PIR3 &= ~(0b00010000);                  //clear interrupt
    }
    if(PIR3 &       0b00100000) {               //ERROR on bus
        CANdriverErrors = COMSTAT;
        COMSTAT &= ~0b10000000;
        RXB0CON &= ~0b10000000;                 //set error values to register
        RXB1CON &= ~0b10000000;                 //set error values to register
        PIR3 &= ~(  0b00100000);                //clear interrupt
    }
    if(PIR3 & 0b01000000) {                     //Bus activity wakeup
        PIR3 &= ~(0b01000000);                  //clear interrupt
    }
    if(PIR3 & 0b10000000) {                     //Invalid message on the bus
        PIR3 &= ~(0b10000000);                  //clear interrupt
    }
    PIE3 = 0xFF;
   /* uint8_t data_size;
    //////////////////////////////////////////////////////////////////////////
    PIE3 = 0x00; //dissable interrupts
    if(PIR3 & 0b00000001) { //RX0 received
        RXB0CON &= ~(0b10000000);
        if((RXB0CON & 0x03) == 0x00) {
            updateEnable |= UPDATE_DIGITS;
            data_size = RXB0DLC;
            if(data_size > 0) lampsDigits[5] = RXB0D0;
            if(data_size > 1) lampsDigits[4] = RXB0D1;
            if(data_size > 2) lampsDigits[3] = RXB0D2;
            if(data_size > 3) lampsDigits[2] = RXB0D3;
            if(data_size > 4) lampsDigits[1] = RXB0D4;
            if(data_size > 5) lampsDigits[0] = RXB0D5;
            if(data_size > 6) lampsDots = RXB0D6;
        } else if((RXB0CON & 0x03) == 0x01) {
            updateEnable |= UPDATE_RESERVED;
        } else {
            //do nothing
        }
        PIR3 &= ~(0b00000001);
    }
    if(PIR3 & 0b00000010) { //RX1 received
       // RXB1CON &= ~(0b10000000);
        //if((RXB1CON & 0x07) == 0x02) {
            //data_size = RXB1DLC;
            //if(8 == data_size) {
                //newlampsNixieOnStatus =    RXB1D0;
                //newlampsRefreshRate =      RXB1D1;
                //newlampsPWMFrequency =     RXB1D2;
                //newlampsPWMDutyCycle =     RXB1D3;
                //newDetox =                 RXB1D4;
                //updateEnable |= UPDATE_CONFIG;
          //  }
        //} else if((RXB1CON & 0x07) == 0x03) {
        //    sendStatusFrame();
        //} else {

        //}
        PIR3 &= ~(0b00000010);
    }
    if(PIR3 & 0b00000100) { //TX0 transmited
        PIR3 &= ~(0b00000100);
    }
    if(PIR3 & 0b00001000) { //TX1 transmited
        PIR3 &= ~(0b00001000);
    }
    if(PIR3 & 0b00010000) { //TX2 transmited
        PIR3 &= ~(0b00010000);
    }
    if(PIR3 & 0b00100000) { //ERROR on bus
        PIR3 &= ~(0b00100000);
    }
    if(PIR3 & 0b01000000) { //Bus activity wakeup
        PIR3 &= ~(0b01000000);
    }
    if(PIR3 & 0b10000000) { //Invalid message on the bus
        PIR3 &= ~(0b10000000);
    }
    PIE3 = 0xFF; //enable interrupts
     */
}
