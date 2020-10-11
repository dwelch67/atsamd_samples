
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define PORT_BASE 0x41004400

#define PORTA_DIRSET (PORT_BASE+0x08)
#define PORTA_OUTCLR (PORT_BASE+0x14)
#define PORTA_OUTSET (PORT_BASE+0x18)

#define PORTB_DIRSET (PORT_BASE+0x88)
#define PORTB_OUTCLR (PORT_BASE+0x94)
#define PORTB_OUTSET (PORT_BASE+0x98)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define SYSCTRL 0x40000800
#define OSC8M (SYSCTRL+0x20)

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
    unsigned int rb;

if(1)
{
    //just in case you run this more than once without a reset
    ra=GET32(OSC8M);
    ra|=(3<<8);
    PUT32(OSC8M,ra);
}

    PUT32(PORTA_DIRSET,1<<17);
    PUT32(PORTA_DIRSET,1<<27);
    PUT32(PORTB_DIRSET,1<<3);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

if(1)
{
    for(ra=0;ra<3;ra++)
    {
        PUT32(PORTA_OUTSET,1<<17);
        PUT32(PORTA_OUTSET,1<<27);
        PUT32(PORTB_OUTSET,1<<3);
        delay(8);
        PUT32(PORTA_OUTCLR,1<<17);
        PUT32(PORTA_OUTCLR,1<<27);
        PUT32(PORTB_OUTCLR,1<<3);
        delay(8);
    }
}

    ra=GET32(OSC8M);
    ra&=~(3<<8);
    PUT32(OSC8M,ra);

if(1)
{
    for(ra=0;ra<7;ra++)
    {
        PUT32(PORTA_OUTSET,1<<17);
        PUT32(PORTA_OUTSET,1<<27);
        PUT32(PORTB_OUTSET,1<<3);
        delay(8);
        PUT32(PORTA_OUTCLR,1<<17);
        PUT32(PORTA_OUTCLR,1<<27);
        PUT32(PORTB_OUTCLR,1<<3);
        delay(8);
    }
}

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,0x00FFFFFF);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    rb=GET32(STK_CVR);
    while(1)
    {
        PUT32(PORTA_OUTSET,1<<17);
        PUT32(PORTA_OUTSET,1<<27);
        PUT32(PORTB_OUTSET,1<<3);
        while(1)
        {
            ra=GET32(STK_CVR);
            if(((rb-ra)&STK_MASK)>=1234567) break;
        }
        rb=ra;
        PUT32(PORTA_OUTCLR,1<<17);
        PUT32(PORTA_OUTCLR,1<<27);
        PUT32(PORTB_OUTCLR,1<<3);
        while(1)
        {
            ra=GET32(STK_CVR);
            if(((rb-ra)&STK_MASK)>=1234567) break;
        }
        rb=ra;
    }

    return(0);
}
