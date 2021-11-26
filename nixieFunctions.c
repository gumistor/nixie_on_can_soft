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

//sets '1' of the pin 'piNo' in port C
void setPortCPinOn(uint8_t pinNo) {
    uint8_t tempOut;
    tempOut = LATC;
    tempOut |= 0x01 << pinNo;
    LATC = tempOut;
}

//sets '1' of the pin 'piNo' in port B
void setPortBPinOn(uint8_t pinNo) {
    uint8_t tempOut;
    tempOut = LATB;
    tempOut |= 0x01 << pinNo;
    LATB = tempOut;
}

//sets '1' of the pin 'piNo' in port A
void setPortAPinOn(uint8_t pinNo) {
    uint8_t tempOut;
    tempOut = LATA;
    tempOut |= 0x01 << pinNo;
    LATA = tempOut;
}

//sets '0' of the pin 'piNo' in port C
void setPortCPinOff(uint8_t pinNo) {
    uint8_t tempOut;
    tempOut = LATC;
    tempOut &= ~(0x01 << pinNo);
    LATC = tempOut;
}

//sets '0' of the pin 'piNo' in port B
void setPortBPinOff(uint8_t pinNo) {
    uint8_t tempOut;
    tempOut = LATB;
    tempOut &= ~(0x01 << pinNo);
    LATB = tempOut;
}

//sets '0' of the pin 'piNo' in port A
void setPortAPinOff(uint8_t pinNo) {
    uint8_t tempOut;
    tempOut = LATA;
    tempOut &= ~(0x01 << pinNo);
    LATA = tempOut;
}

//set a 'noOfLamp' anode
void setLamp(uint8_t noOfLamp) { //h1 h0 : m1 m0 : s1 s0

    //turn off all anodes
    setPortBPinOff(LAMP_H1_PIN); //lamp h1
    setPortBPinOff(LAMP_H0_PIN); //lamp h0
    setPortBPinOff(LAMP_M1_PIN); //lamp m1
    setPortBPinOff(LAMP_M0_PIN); //lamp m0
    setPortCPinOff(LAMP_S1_PIN); //lamp s1
    setPortCPinOff(LAMP_S0_PIN); //lamp s0

    //turn on one anode, acording to 'noOfLamp' value
    switch(noOfLamp) {
        case LAMP_S0:
            setPortCPinOn(LAMP_S0_PIN);
        break;
        case LAMP_S1:
            setPortCPinOn(LAMP_S1_PIN);
        break;
        case LAMP_M0:
            setPortBPinOn(LAMP_M0_PIN);
        break;
        case LAMP_M1:
            setPortBPinOn(LAMP_M1_PIN);
        break;
        case LAMP_H0:
            setPortBPinOn(LAMP_H0_PIN);
        break;
        case LAMP_H1:
            setPortBPinOn(LAMP_H1_PIN);
        break;
        default:
        break;
    }
}

//sets number to be shown according to argument 'noToSet'
void setNumber_old(uint8_t noToSet) {

    //turn off all digits, it prevents flashing of previous digit before
    //update to new value
    setPortAPinOff(0); //0
    setPortAPinOff(1); //1
    setPortAPinOff(2); //2
    setPortAPinOff(3); //3
    setPortAPinOff(4); //4
    setPortAPinOff(5); //5
    setPortCPinOff(0); //6
    setPortCPinOff(1); //7
    setPortCPinOff(3); //8
    setPortCPinOff(4); //9



    //switch on proper value
    switch(noToSet) {
        case 0x00:
            setPortAPinOn(0);
        break;
        case 0x09:
            setPortAPinOn(1);
        break;
        case 0x02:
            setPortCPinOn(3);
        break;
        case 0x03:
            setPortCPinOn(1);
        break;
        case 0x04:
            setPortCPinOn(0);
        break;
        case 0x05:
            setPortAPinOn(5);
        break;
        case 0x06:
            setPortAPinOn(4);
        break;
        case 0x07:
            setPortAPinOn(3);
        break;
        case 0x08:
            setPortAPinOn(2);
        break;
        case 0x01:
            setPortCPinOn(4); 
        break;
        case 0x0D: //dot
           // setPortCPinOn(5);
        break;
        default:
            //off
        break;
    }
}

void setNumber(uint8_t noToSet) {

    //turn off all digits, it prevents flashing of previous digit before
    //update to new value

    setPortAPinOff(4); //0//
    setPortAPinOff(3); //1//
    setPortAPinOff(2); //2//
    setPortAPinOff(1); //3//
    setPortAPinOff(0); //4//
    //setPortAPinOff(5); //dot//
    setPortCPinOff(0); //5//
    setPortCPinOff(1); //6//
    setPortCPinOff(3); //7//
    setPortCPinOff(4); //8//
    setPortCPinOff(5); //9//



    //switch on proper value
    switch(noToSet) {
        case 0x00:
            setPortAPinOn(4);
        break;
        case 0x01:
            setPortCPinOn(5);
        break;
        case 0x08:
            setPortAPinOn(2);
        break;
        case 0x07:
            setPortAPinOn(1);
        break;
        case 0x06:
            setPortAPinOn(0);
        break;
        case 0x05:
            setPortCPinOn(0); //
        break;
        case 0x04:
            setPortCPinOn(1); //
        break;
        case 0x03:
            setPortCPinOn(3);
        break;
        case 0x02:
            setPortCPinOn(4);
        break;
        case 0x09:
            setPortAPinOn(3);
        break;
        case 0x0D: //dot
           // setPortCPinOn(5);
        break;
        default:
            //off
        break;
    }
}

//sets dot to be shown according to argument 'currentLamp'
void setDot_old(uint8_t currentLamp) {
        setPortCPinOff(5); //dot
        if(currentLamp) {
            setPortCPinOn(5);
        }
}

void setDot(uint8_t currentLamp) {
        setPortAPinOff(5); //dot
        if(currentLamp) {
            setPortAPinOn(5);
        }
}

//creates status frame to be send on the bus
//values are entered directly to the CAN buffer, becase
//it is alway set to provide these informations
//TXB1 (transmit buffer 1 - has it's own id and will contain
//      same data all the time)
//DLC is set to 8 thus all data bytes are filled with data
/*
void sendStatusFrame(void) {
    TXB1D0 = lampsNixieOnStatus;    //on off status
    TXB1D1 = lampsRefreshRate;      //lamps refres rate
    TXB1D2 = lampsPWMFrequency;     //pwm frequency counter
    TXB1D3 = lampsPWMDutyCycle;     //pwm duty cycle
    TXB1D4 = 0xFF;                  //not used
    TXB1D5 = 0xFF;                  //not used
    TXB1D6 = 0xFF;                  //not used
    TXB1D7 = 0xFF;                  //not used
    //set buffer to transmit
    TXB1CONbits.TXREQ = 1;
}*/

void setAllLampsValue(int value) {
    int lamp_index;

    for(lamp_index = 0; lamp_index < 6; lamp_index++) {
        lampsDigits[lamp_index] = (value);
    }
}

void voltageProfile(uint8_t detoxRange) {
    uint32_t waitTime, j;
    uint8_t oldDutyCycle;
    uint8_t currentDutyCycle;
    
    oldDutyCycle = lampsPWMDutyCycle;
    //PR2 = lampsPWMFrequency - 20;

    waitTime = 0xAFFFF;

    for(currentDutyCycle = oldDutyCycle; currentDutyCycle > (oldDutyCycle - detoxRange); currentDutyCycle= currentDutyCycle - 4) {
        CCPR1L = currentDutyCycle;
        for(j = 0; j < 0xFFF; j++) ;
    }

    for(j = 0; j < waitTime; j++) ;

    for(currentDutyCycle = oldDutyCycle - detoxRange; currentDutyCycle < oldDutyCycle; currentDutyCycle= currentDutyCycle + 4) {
        CCPR1L = currentDutyCycle;
        for(j = 0; j < 0xFFF; j++) ;
    }


    //PR2 = lampsPWMFrequency;
    lampsPWMDutyCycle = oldDutyCycle;
}

void detoxProcedure(void) {
    uint8_t value_to_detox;

    //turn off interrupts from CAN
    PIE3 = 0x00;

    lampsDots = 0x00;

    for(value_to_detox = 0; value_to_detox < 10; value_to_detox++) {
        setAllLampsValue(value_to_detox);

        voltageProfile(0x20);
    }

    setAllLampsValue(0x0D);

    lampsDots = 0xFF;

    voltageProfile(0x17);
    //turn on interrupts from CAN
    PIE3 = 0xFF;
}

void initNixie(void)
{
    sourcedevice_CLASS      = EEPROM_READ(DEFAULT_SOURCE_CLASS);
    sourcedevice_ID         = EEPROM_READ(DEFAULT_SOURCE_ID);
}