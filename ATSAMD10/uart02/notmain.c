
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET8 ( unsigned int );
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
#define SERCOM0_INTFLAG     (SERCOM0_BASE+0x18)

#define CPUID 0xE000ED00

static void uart_send ( unsigned int d )
{
    while(1)
    {
        if(GET8(SERCOM0_INTFLAG)&1) break;
    }
    PUT8(SERCOM0_DATA,d&0xFF);
}

static unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET8(SERCOM0_INTFLAG)&4) break;
    }
    return(GET8(SERCOM0_DATA)&0xFF);
}

void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}

void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
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

    for(ra=0;ra<10000;ra++)
    {
        uart_send(0x30+(ra&7));
    }
    uart_send(0x0D);
    uart_send(0x0A);
    hexstring(0x12345678);
    hexstring(GET32(CPUID));

    while(1)
    {
        ra=uart_recv();
        hexstring(ra);
    }

}

/*

halt
flash write_image erase uart02/notmain.elf

*/

