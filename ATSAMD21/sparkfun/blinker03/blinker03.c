
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void dummy ( unsigned int );

#define PORT_BASE 0x41004400

#define PORTADIRSET (PORT_BASE+0x08)
#define PORTAOUTCLR (PORT_BASE+0x14)
#define PORTAOUTSET (PORT_BASE+0x18)
#define PORTAOUTTGL (PORT_BASE+0x1C)

#define TC4_BASE 0x42003000
#define TC4_CTRLA (TC4_BASE+0x00)
#define TC4_COUNT (TC4_BASE+0x10)
#define TC4_STATUS (TC4_BASE+0x0F)

#define GCLK_BASE 0x40000C00
#define GCLK_CTRL (GCLK_BASE+0x00)
#define GCLK_STATUS (GCLK_BASE+0x01)
#define GCLK_CLKCTRL (GCLK_BASE+0x02)
#define GCLK_GENCTRL (GCLK_BASE+0x04)
#define GCLK_GENDIV (GCLK_BASE+0x08)

#define PM_BASE 0x40000400
#define APBCMASK (PM_BASE+0x20)

#define SYSCTRL_BASE 0x40000800
#define OSC8M (SYSCTRL_BASE+0x20)

static int delay_bit ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            ra=GET16(TC4_COUNT);
            while(GET8(TC4_STATUS)&0x80) continue;
            if(ra&0x1000) break;
        }
        while(1)
        {
            ra=GET16(TC4_COUNT);
            while(GET8(TC4_STATUS)&0x80) continue;
            if((ra&0x1000)==0) break;
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

    PUT32(PORTADIRSET,1<<17);
    PUT32(PORTADIRSET,1<<27);

    //Enable clocks to (turn on) TC4
    ra=GET32(APBCMASK);
    ra|=1<<12; //enable TC4
    PUT32(APBCMASK,ra);

    //use OSC8M for GCLKGEN5
    PUT32(GCLK_GENCTRL,0x00010605);
    //select a clock
    PUT16(GCLK_CLKCTRL,0x451C);

    PUT16(TC4_CTRLA,0x0000);
    while(GET8(TC4_STATUS)&0x80) continue;

    PUT16(TC4_CTRLA,0x0702);
    while(GET8(TC4_STATUS)&0x80) continue;

    while(1)
    {
        PUT32(PORTAOUTTGL,1<<17);
        PUT32(PORTAOUTTGL,1<<27);
        delay_bit(1);
    }

    return(0);
}
