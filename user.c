/******************************************************************************/
/* Files to Include                                                           */
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

#include "user.h"
#include "nixieFunctions.h"
#include "kho_can.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
/*
void InitializeCAN(void)
{
    TRISB |= 0b00001000;    //set RB2 as TX pin (OUT)
    TRISB &= ~(0b00000100); //set RB3 as RX pin  (IN)

    CANCON = 0x80;          //REQOP<2:0>=100    Request configuration mode
    while(!((CANSTAT & 0b11100000 ) == 0b10000000));    // Wait for configuration mode

    ECANCON = 0x00;         // Select legacy mode

    // Initialize CAN Timing
    BRGCON1 = 0x01; //0000 0001     //SJW=1TQ     BRP  1
    BRGCON2 = 0x91; //0001 0001     //SEG2PHTS 1    sampled once  PS1=8TQ  PropagationT 1TQ
    BRGCON3 = 0x01; //0000 0010     //PS2  6TQ

    //
    // Filters initialization
    //

    RXB0CON = 0b00000000; //RXB0CON = 0b00100000; //RXF0 0b001... - only 11bit addressing allowed
    RXB1CON = 0b00100000; //RXF1 0b001... - only 11bit addressing allowed

    //RXF0 - asociated with RXB0
    RXF0SIDH = 0b00100110;
    RXF0SIDL = 0b00000000; //130
    RXF0EIDH = 0x00;
    RXF0EIDL = 0x00;

    //RXF1 - asociated with RXB0
    RXF1SIDH = 0b01000110;
    RXF1SIDL = 0b00000000; //230
    RXF1EIDH = 0x00;
    RXF1EIDL = 0x00;

    //RXF0 and RXF1 filter mask
    RXM0SIDH = 0xFF; //0xFF - full mask
    RXM0SIDL = 0xE0; //0xE0 - full mask
    RXM0EIDH = 0x00;
    RXM0EIDL = 0x00;

    //RXF2 - associatet with RXB1
    RXF2SIDH = 0b01100110; //330
    RXF2SIDL = 0b00000000;
    RXF2EIDH = 0x00;
    RXF2EIDL = 0x00;

    //RXF3 - associatet with RXB1
    RXF3SIDH = 0b10000110; //430
    RXF3SIDL = 0b00000000;
    RXF3EIDH = 0x00;
    RXF3EIDL = 0x00;

    //RXF2 - RXF5 filter mask
    RXM1SIDH = 0xFF; //0xFF - full mask
    RXM1SIDL = 0xE0; //0xE0 - full mask
    RXM1EIDH = 0x00;
    RXM1EIDL = 0x00;

    //Enable filters
    RXFCON0 = 0b00001111; //Enabled: RXF0, RXF1, RXF2
    RXFCON1 = 0b00000000; //other filters disabled

    IPR3 = 0x00; //set all interupts on low

    CANCON = 0x00;                      // Select normal mode
    while(!((CANSTAT & 0b11100000 ) == 0b00000000));    // Wait for normal mode

    //CANCON = 0x40;                      // Select loopback mode
    //while(!((CANSTAT & 0b11100000 ) == 0b01000000));    // Wait for loopback mode

    //Set transmit buffer 0
    TXB0EIDH = 0x00;
    TXB0EIDL = 0x00;

    TXB0SIDH = 0b11000110;
    TXB0SIDL = 0b00000000;

    TXB0DLC = 8;

    //Set transmit buffer 1
    TXB1EIDH = 0x00;
    TXB1EIDL = 0x00;

    TXB1SIDH = 0b11100110;
    TXB1SIDL = 0b00000000;

    TXB1DLC = 8;

    //ENABLE
    PIE3 = 0xFF; //enable all interupts

}
*/
void InitializeTimer0 (void) {
    //lampsRefreshRate = DEFALUT_REFRESH_RATE;
    /*T0CON = 0b01000111;
    TMR0L = lampsRefreshRate;
    TMR0H = 0x00;
    INTCON  = 0b01100000;
    INTCON2 = 0b00000100;
    INTCON3 = 0x00;
    T0CON = 0b11000111;*/
    //TMR2 = 0x00;
    //PR2 = 0xFF-lampsRefreshRate;
    //set interrupts
    INTCON  &= ~(0b00100000); //disable TMR0IE overflow interrupt
    INTCON2 |= 0b00000100; //set TMR0IP overflow interrupt priority to high
    //set pins
    //
    //set internal registers
    T0CON = 0b00001111;
    TMR0H = 0xCF;
    TMR0L = 0x5F; //rate
    //enable periphelia
    T0CON = 0b10001111;
    systemTick = 0;
    old_systemTick = 0;
    //enable interrupts
    //INTCON  |= 0b00100000; //enable TMR0IE overflow interrupt
}

void InitializeTimer1 (void) {
    lampsRefreshRate = EEPROM_READ(DEFALUT_REFRESH_RATE);
    T1CON = 0b00000000;
    TMR1H = 0xFF;
    TMR1L = lampsRefreshRate;
    IPR1 |= 0x01;
    PIR1 &= ~0x01;
    PIE1 |= 0x01;
    T1CON = 0b00000001;
}

void InitializePWM (void)
{

  //lampsPWMFrequency = 0x70;
  lampsPWMFrequency = EEPROM_READ(DEFAULT_PWM_FREQUENCY); //0x70;
  PR2 = lampsPWMFrequency;                /* initialize the PWM period */
  TMR2 = 0x00;
  //lampsPWMDutyCycle = 0x40;
  lampsPWMDutyCycle = EEPROM_READ(DEFAULT_PWM_DUTYCYCLE); //0x40;
  CCPR1L = lampsPWMDutyCycle;             /* initialize the PWM duty cycle */
  TRISCbits.TRISC2 = 0;      /* configure the CCP1 modul */
  T2CON = 0x04;              /* postscale 1:1, Timer0 ON, prescaler 1 */
  CCP1CON = 0x0F;            /* turn the PWM on */
}

void InitializeOutputs (void)
{
    ADCON1 = 0x0F;

    TRISC = 0x00;
    TRISB = 0x00;
    TRISA = 0x00;

    PORTC = 0x00;
    PORTB = 0x00;
    PORTA = 0x00;

    LATC = 0x00;
    LATB = 0x00;
    LATA = 0x00;
    /*
    //d6
    TRISCbits.TRISC0    = 0x00;
    TRISCbits.TRISC1    = 0x00;
    TRISCbits.TRISC3    = 0x00;
    TRISCbits.TRISC4    = 0x00;
    TRISCbits.TRISC5    = 0x00;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    PORTCbits.RC0 = 0x00;
    PORTCbits.RC1 = 0;
    PORTCbits.RC3 = 0;
    PORTCbits.RC4 = 0;
    PORTCbits.RC5 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;

    LATCbits.LATC0 = 0x00;
    LATCbits.LATC1 = 0;
    LATCbits.LATC3 = 0;
    LATCbits.LATC4 = 0;
    LATCbits.LATC5 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC7 = 0;*/
}


void InitApp(void)
{
    RCON    |= 0b10000000;
    INTCON2  = 0b00000000;
    INTCON3  = 0b00000000;
    INTCON   = 0b00000000;//enable high and low interrupts
    PIE1     = 0b00000000;
    PIE2     = 0b00000000;
    IPR1     = 0x00;
    IPR2     = 0x00;
    
    InitializeOutputs();
    initCAN();
    InitializeTimer0();
    InitializeTimer1();
    InitializePWM();
    initNixie();
    //initialize interrupts
    INTCON  |= 0b11000000;
}

