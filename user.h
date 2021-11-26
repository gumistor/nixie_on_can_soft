/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define _125MS_SYSTEM_TICK  0xFF-0x40
#define _250MS_SYSTEM_TICK  0xFF-0x80
#define _500MS_SYSTEM_TICK  0xFF-0x00

#define SYSTEM_TICK _125MS_SYSTEM_TICK

#define READOUT_SLEEP   0x00
#define READOUT_ON      0x01
#define READOUT_OFF     0x02
#define READOUT_ERROR   0x03

volatile uint16_t systemTick;   //counter increased by timer, according to
                                //data received from RTC
                                //input from rtc 512 Hz
volatile uint16_t old_systemTick;

uint8_t sensor_readout_start = READOUT_ON;

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);         /* I/O and Peripheral Initialization */
