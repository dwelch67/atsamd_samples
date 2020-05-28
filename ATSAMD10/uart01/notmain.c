
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define PORT_BASE       0x41004400
#define PORT_DIRSETA    (PORT_BASE+0x00+0x08)
#define PORT_OUTCLRA    (PORT_BASE+0x00+0x14)
#define PORT_OUTSETA    (PORT_BASE+0x00+0x18)

//14/15  14/2 = 7
#define PORTA_PMUX07    (PORT_BASE+0x00+0x30+7)
//PA14
#define PORTA_PINCFG14  (PORT_BASE+0x00+0x40+14)
//PA15
#define PORTA_PINCFG15  (PORT_BASE+0x00+0x40+15)

#define SYSCTRL_BASE    0x40000800
#define OSC8M           (SYSCTRL_BASE+0x20)

#define PM_BASE         0x40000400
#define APBCMASK        (PM_BASE+0x20)

#define GCLK_BASE       0x40000C00
//#define GCLK_CTRL       (GCLK_BASE+0x00)
//#define GCLK_STATUS     (GCLK_BASE+0x01)
#define GCLK_CLKCTRL    (GCLK_BASE+0x02)
#define GCLK_GENCTRL    (GCLK_BASE+0x04)
//#define GCLK_GENDIV     (GCLK_BASE+0x08)

#define SERCOM0_BASE        0x42000800
#define SERCOM0_CTRLA       (SERCOM0_BASE+0x00)
#define SERCOM0_CTRLB       (SERCOM0_BASE+0x04)
#define SERCOM0_BAUD        (SERCOM0_BASE+0x0C)
#define SERCOM0_SYNCBUSY    (SERCOM0_BASE+0x1C)
#define SERCOM0_DATA        (SERCOM0_BASE+0x28)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


static int delay ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            ra=GET32(STK_CSR);
            if(ra&(1<<16)) break;
        }
    }
    return(0);
}


void notmain ( void )
{
    unsigned int ra;

    ra=GET32(OSC8M);
    ra&=~(3<<8);
    PUT32(OSC8M,ra);

    ra=GET32(APBCMASK);
    ra|=1<<2; //enable SERCOM0
    PUT32(APBCMASK,ra);

    PUT32(GCLK_GENCTRL,0x00010605);
    PUT16(GCLK_CLKCTRL,0x450E);

    //PA14 TXD SERCOM0 PAD[0] MUX C
    //PA15 RXD SERCOM0 PAD[1] MUX C
    PUT8(PORTA_PINCFG14,0x01);
    PUT8(PORTA_PINCFG15,0x01);
    PUT8(PORTA_PMUX07,0x22);

    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLA,0x00000000);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLA,0x00000001);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLA,0x40100004);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLB,0x00030000);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT16(SERCOM0_BAUD,50436);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLA,0x40100006);
    while(GET32(SERCOM0_SYNCBUSY)) continue;

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    while(1)
    {
        PUT16(SERCOM0_DATA,0x55);
        delay(8);
        PUT16(SERCOM0_DATA,0x56);
        delay(8);
    }
}

/*

halt
flash write_image erase uart01/notmain.elf

*/

