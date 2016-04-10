
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void dummy ( unsigned int );

#define PORT_BASE 0x41004400

#define PORTA_DIRSET (PORT_BASE+0x00+0x08)
#define PORTA_OUTCLR (PORT_BASE+0x00+0x14)
#define PORTA_OUTSET (PORT_BASE+0x00+0x18)
#define PORTA_OUTTGL (PORT_BASE+0x00+0x1C)

#define PORTB_DIRSET (PORT_BASE+0x80+0x08)
#define PORTB_OUTCLR (PORT_BASE+0x80+0x14)
#define PORTB_OUTSET (PORT_BASE+0x80+0x18)
#define PORTB_OUTTGL (PORT_BASE+0x80+0x1C)

#define PORTA_PMUX05    (PORT_BASE+0x00+0x30+5)
#define PORTA_PINCFG10  (PORT_BASE+0x00+0x40+10)
#define PORTA_PINCFG11  (PORT_BASE+0x00+0x40+11)

#define PORTB_PMUX01    (PORT_BASE+0x80+0x30+1)
#define PORTB_PMUX11    (PORT_BASE+0x80+0x30+11)
#define PORTB_PINCFG03  (PORT_BASE+0x80+0x40+3)
#define PORTB_PINCFG22  (PORT_BASE+0x80+0x40+22)
#define PORTB_PINCFG23  (PORT_BASE+0x80+0x40+23)

#define GCLK_BASE       0x40000C00
#define GCLK_CTRL       (GCLK_BASE+0x00)
#define GCLK_STATUS     (GCLK_BASE+0x01)
#define GCLK_CLKCTRL    (GCLK_BASE+0x02)
#define GCLK_GENCTRL    (GCLK_BASE+0x04)
#define GCLK_GENDIV     (GCLK_BASE+0x08)

#define PM_BASE         0x40000400
#define APBCMASK        (PM_BASE+0x20)

#define SYSCTRL_BASE    0x40000800
#define OSC8M           (SYSCTRL_BASE+0x20)

#define SERCOM5_BASE        0x42001C00
#define SERCOM5_CTRLA       (SERCOM5_BASE+0x00)
#define SERCOM5_CTRLB       (SERCOM5_BASE+0x04)
#define SERCOM5_BAUD        (SERCOM5_BASE+0x0C)
#define SERCOM5_SYNCBUSY    (SERCOM5_BASE+0x1C)
#define SERCOM5_DATA        (SERCOM5_BASE+0x28)

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

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(OSC8M);
    ra&=~(3<<8);
    PUT32(OSC8M,ra);

    ra=GET32(APBCMASK);
    ra|=1<<7; //enable SERCOM5
    ra|=1<<2; //enable SERCOM0
    PUT32(APBCMASK,ra);

    PUT32(GCLK_GENCTRL,0x00010605);

    PUT16(GCLK_CLKCTRL,0x4514);

    PUT8(PORTA_PINCFG10,0x01);
    PUT8(PORTA_PINCFG11,0x01);
    PUT8(PORTA_PMUX05,0x22);

    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLA,0x00000000);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLA,0x00000001);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLA,0x40310004);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLB,0x00030000);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT16(SERCOM0_BAUD,50436);
    while(GET32(SERCOM0_SYNCBUSY)) continue;
    PUT32(SERCOM0_CTRLA,0x40310006);
    while(GET32(SERCOM0_SYNCBUSY)) continue;

    PUT16(GCLK_CLKCTRL,0x4519);

    PUT8(PORTB_PINCFG22,0x01);
    PUT8(PORTB_PINCFG23,0x01);
    PUT8(PORTB_PMUX11,0x33);

    while(GET32(SERCOM5_SYNCBUSY)) continue;
    PUT32(SERCOM5_CTRLA,0x00000000);
    while(GET32(SERCOM5_SYNCBUSY)) continue;
    PUT32(SERCOM5_CTRLA,0x00000001);
    while(GET32(SERCOM5_SYNCBUSY)) continue;
    PUT32(SERCOM5_CTRLA,0x40310004);
    while(GET32(SERCOM5_SYNCBUSY)) continue;
    PUT32(SERCOM5_CTRLB,0x00030000);
    while(GET32(SERCOM5_SYNCBUSY)) continue;
    PUT16(SERCOM5_BAUD,50436);
    while(GET32(SERCOM5_SYNCBUSY)) continue;
    PUT32(SERCOM5_CTRLA,0x40310006);
    while(GET32(SERCOM5_SYNCBUSY)) continue;

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    PUT32(PORTA_DIRSET,1<<17);
    PUT32(PORTA_DIRSET,1<<27);

    while(1)
    {
        PUT16(SERCOM0_DATA,0x55);
        PUT16(SERCOM5_DATA,0x55);
        PUT32(PORTA_OUTTGL,1<<17);
        PUT32(PORTA_OUTTGL,1<<27);
        delay(8);
        PUT16(SERCOM0_DATA,0x56);
        PUT16(SERCOM5_DATA,0x56);
        PUT32(PORTA_OUTTGL,1<<17);
        PUT32(PORTA_OUTTGL,1<<27);
        delay(8);
    }

    return(0);
}
