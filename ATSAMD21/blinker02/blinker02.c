
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define PORTBASE 0x41004400

#define PORTADIRSET (PORTBASE+0x08)
#define PORTAOUTCLR (PORTBASE+0x14)
#define PORTAOUTSET (PORTBASE+0x18)

#define PORTBDIRSET (PORTBASE+0x88)
#define PORTBOUTCLR (PORTBASE+0x94)
#define PORTBOUTSET (PORTBASE+0x98)


#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define SYSCTRL 0x40000800

#define OSC8M (SYSCTRL+0x20)


int delay ( unsigned int n )
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

if(1)
{
    //just in case you run this more than once without a reset
    ra=GET32(OSC8M);
    ra|=(3<<8);
    PUT32(OSC8M,ra);
}

    PUT32(PORTADIRSET,1<<17);
    PUT32(PORTADIRSET,1<<27);
    PUT32(PORTBDIRSET,1<<3);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

if(1)
{
    for(ra=0;ra<3;ra++)
    {
        PUT32(PORTAOUTSET,1<<17);
        PUT32(PORTAOUTSET,1<<27);
        PUT32(PORTBOUTSET,1<<3);
        delay(8);
        PUT32(PORTAOUTCLR,1<<17);
        PUT32(PORTAOUTCLR,1<<27);
        PUT32(PORTBOUTCLR,1<<3);
        delay(8);
    }
}

    ra=GET32(OSC8M);
    ra&=~(3<<8);
    PUT32(OSC8M,ra);

    while(1)
    {
        PUT32(PORTAOUTSET,1<<17);
        PUT32(PORTAOUTSET,1<<27);
        PUT32(PORTBOUTSET,1<<3);
        delay(8);
        PUT32(PORTAOUTCLR,1<<17);
        PUT32(PORTAOUTCLR,1<<27);
        PUT32(PORTBOUTCLR,1<<3);
        delay(8);
    }

    return(0);
}
