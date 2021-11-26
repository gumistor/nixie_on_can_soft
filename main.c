/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "nixieFunctions.h"
#include "kho_can.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */
uint8_t lampsDigits[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
/*
void ECAN_Transmit(uint8_t dlc, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3
        , uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    TXB0DLC = dlc;
    TXB0D0 = d0;
    TXB0D1 = d1;
    TXB0D2 = d2;
    TXB0D3 = d3;
    TXB0D4 = d4;
    TXB0D5 = d5;
    TXB0D6 = d6;
    TXB0D7 = d7;

    TXB0CONbits.TXREQ = 1; //Set the buffer to transmit
}
*/
void doCAN(void)
{
    uint16_t i;
    uint8_t readValue;

    if(CAN_data_received_flag == CAN_DATA_FULL)
    {
        switch(CAN_data_received.CAN_address & 0x1FFF0000)
        {
            case CAN_ADDR_ACTUATOR:
                //no action is needed
            break;
            case CAN_ADDR_DISPLAY:
                i = ((sourcedevice_CLASS<<8)|sourcedevice_ID);
                if((CAN_data_received.CAN_address & 0x0000FFFF) == i)
                {
                    if(CAN_data_received.CAN_DLC > 0) lampsDigits[5] = CAN_data_received.CAN_data[0];
                    if(CAN_data_received.CAN_DLC > 1) lampsDigits[4] = CAN_data_received.CAN_data[1];
                    if(CAN_data_received.CAN_DLC > 2) lampsDigits[3] = CAN_data_received.CAN_data[2];
                    if(CAN_data_received.CAN_DLC > 3) lampsDigits[2] = CAN_data_received.CAN_data[3];
                    if(CAN_data_received.CAN_DLC > 4) lampsDigits[1] = CAN_data_received.CAN_data[4];
                    if(CAN_data_received.CAN_DLC > 5) lampsDigits[0] = CAN_data_received.CAN_data[5];
                    if(CAN_data_received.CAN_DLC > 6) lampsDots =  CAN_data_received.CAN_data[6];
                }
            break;
            case CAN_ADDR_CONFIG_REQ:
                    switch(CAN_data_received.CAN_data[0])
                    {
                    case 0x00: //setup time in DS1340 RTC
                        newlampsPWMDutyCycle = CAN_data_received.CAN_data[2];
                        if(newlampsPWMDutyCycle != lampsPWMDutyCycle)
                        {
                            readValue = EEPROM_READ(DEFAULT_PWM_DUTYCYCLE);
                            if((newlampsPWMDutyCycle < readValue + DEFAULT_PWM_DUTYCYCLE_MARGIN) && (newlampsPWMDutyCycle > readValue - DEFAULT_PWM_DUTYCYCLE_MARGIN))
                            {
                                lampsPWMDutyCycle = newlampsPWMDutyCycle;
                                CCPR1L = lampsPWMDutyCycle;
                            }
                        }
                        newlampsPWMFrequency = CAN_data_received.CAN_data[1];
                        if(newlampsPWMFrequency != lampsPWMFrequency)
                        {
                            readValue = EEPROM_READ(DEFAULT_PWM_FREQUENCY);
                            if((newlampsPWMFrequency < readValue + DEFAULT_PWM_FREQUENCY_MARGIN) && (newlampsPWMFrequency > readValue - DEFAULT_PWM_FREQUENCY_MARGIN))
                            {
                                lampsPWMFrequency = newlampsPWMFrequency;
                                PR2 = lampsPWMFrequency;
                            }
                        }
                        break;
                    case 0x01: //setup date in DS1340 RTC
                        if(lampsNixieOnStatus == NIXIE_OFF)
                        {
                            lampsNixieOnStatus = NIXIE_ON;
                        }
                        else
                        {
                            lampsNixieOnStatus = NIXIE_OFF;
                        }
                        break;
                    case 0x02: 
                        configureTXbuffer(TX2BUFFER, CAN_ADDR_CONFIG_RES         | can_node_class<<8 |    device_id,1);
                        sendTXbuffer(TX2BUFFER, 0x02,lampsNixieOnStatus,lampsRefreshRate,lampsPWMFrequency,lampsPWMDutyCycle,sourcedevice_CLASS,sourcedevice_ID,0x00,0x07);
                        break;
                    case 0x03:  // setup oscilator to work of DS1340
                        sourcedevice_CLASS = CAN_data_received.CAN_data[1];
                        sourcedevice_ID = CAN_data_received.CAN_data[2];
                        break;
                    case 0x04:
                        newlampsRefreshRate = CAN_data_received.CAN_data[1];
                        if(newlampsRefreshRate > 0)
                        {
                            lampsRefreshRate = newlampsRefreshRate;
                        }
                        break;
                    case 0xFE:
                        detoxProcedure();
                        break;
                    case 0xFF:
                        EEPROM_WRITE(0xFF,0xFF);
                        for(i=0; i<2000; i++);
                        Reset();
                        break;
                    default:    // no action
                        break;

                    }
                    //configureTXbuffer(TX2BUFFER, CAN_ADDR_CONFIG_RES         | can_node_class<<8 |    device_id,1);
                    //sendTXbuffer(TX2BUFFER, 0x02, CAN_data_received.CAN_address>>24,CAN_data_received.CAN_address>>16,CAN_data_received.CAN_address>>8,CAN_data_received.CAN_address,CAN_data_received.CAN_DLC,
                    //CAN_data_received.CAN_data[0],CAN_data_received.CAN_data[1],0x08);
                break;
            case CAN_ADDR_REQUEST:

            break;
            case CAN_ADDR_RESPONSE:

            break;
            case CAN_ADDR_NM:
                //nm frame received
                //clear wait to sleep frame
                //configureTXbuffer(TX2BUFFER, CAN_ADDR_CONFIG_RES         | can_node_class<<8 |    device_id,1);
                //sendTXbuffer(TX2BUFFER, 0x07, CAN_data_received.CAN_address>>24,CAN_data_received.CAN_address>>16,CAN_data_received.CAN_address>>8,CAN_data_received.CAN_address,CAN_data_received.CAN_DLC,
                //CAN_data_received.CAN_data[0],CAN_data_received.CAN_data[1],0x08);
            break;
            default:
                ;
            }
        CAN_data_received_flag = CAN_DATA_EMPTY;
    }
}

void main(void)
{
    //static uint8_t old_lampsNixieOnStatus;
    //static uint8_t old_lampsPWMFrequency;
    //static uint8_t old_lampsPWMDutyCycle;
    //static uint8_t old_lampsRefreshRate;
    
    lampsDots = 0x00;
    lampsDigits[0] = 0x05;
    lampsDigits[1] = 0x06;
    lampsDigits[2] = 0x07;
    lampsDigits[3] = 0x08;
    lampsDigits[4] = 0x09;
    lampsDigits[5] = 0x04;


    ConfigureOscillator();
    InitApp();

    lampsNixieOnStatus = NIXIE_ON;

    while(1) {
        if(READOUT_ON == sensor_readout_start)
        {
            doERRORS();

            if(systemTick - old_systemTick > 0)
            {
               // if((systemTick & 0x0007) == 0x0004)
               // {
               //     doNM(0x01);
               // }
               //old_systemTick = systemTick;
            }
            else
            {
                //error, lost tick
            }
            doCAN();
        }
        else
        {
            //do nothing
        }
    }

    //old_lampsNixieOnStatus = lampsNixieOnStatus;
    //old_lampsRefreshRate = lampsRefreshRate;
    //old_lampsPWMFrequency = lampsPWMFrequency;
    //old_lampsPWMDutyCycle = lampsPWMDutyCycle;

   /* while(1) {
            if(START_DETOX_KATHODE_PROCEDURE == newDetox) {
                detoxProcedure();
            }
            if(UPDATE_DIGITS == (updateEnable & UPDATE_DIGITS)) {
                updateEnable &= ~UPDATE_DIGITS;
            }
            if(UPDATE_LAMPS == (updateEnable & UPDATE_LAMPS)) {
                updateEnable &= ~UPDATE_LAMPS;
            }
            if(UPDATE_RESERVED == (updateEnable & UPDATE_RESERVED)) {
                updateEnable &= ~UPDATE_RESERVED;
            }
            if(UPDATE_CONFIG == (updateEnable & UPDATE_CONFIG)) {
                if((newlampsPWMDutyCycle < DEFAULT_PWM_DUTYCYCLE + DEFAULT_PWM_DUTYCYCLE_MARGIN) && (newlampsPWMDutyCycle > DEFAULT_PWM_DUTYCYCLE - DEFAULT_PWM_DUTYCYCLE_MARGIN)) {
                    lampsPWMDutyCycle = newlampsPWMDutyCycle;
                    CCPR1L = lampsPWMDutyCycle;

                    //update pwm duty
                }
                if((newlampsPWMFrequency < DEFAULT_PWM_FREQUENCY + DEFAULT_PWM_FREQUENCY_MARGIN) && (newlampsPWMFrequency > DEFAULT_PWM_FREQUENCY - DEFAULT_PWM_FREQUENCY_MARGIN)) {
                    lampsPWMFrequency = newlampsPWMFrequency;
                    PR2 = lampsPWMFrequency;
                }
                if(newlampsRefreshRate > 0) {
                    lampsRefreshRate = newlampsRefreshRate;
                }
                
                lampsNixieOnStatus = newlampsNixieOnStatus;
                updateEnable &= ~UPDATE_CONFIG;
            }
    }*/
}

