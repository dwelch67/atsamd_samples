
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define PORT_BASE       0x41004400
#define PORT_DIRSETA    (PORT_BASE+0x00+0x08)
#define PORT_OUTCLRA    (PORT_BASE+0x00+0x14)
#define PORT_OUTSETA    (PORT_BASE+0x00+0x18)

#define STK_CSR         0xE000E010
#define STK_RVR         0xE000E014
#define STK_CVR         0xE000E018

#define SYSCTRL_BASE    0x40000800
#define OSC8M           (SYSCTRL_BASE+0x20)

void do_delay ( unsigned int sec )
{
    unsigned int ra,rb;

    for(rb=0;rb<sec;)
    {
        ra=GET32(STK_CSR);
        if(ra&(1<<16)) rb++;
    }
}

void notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    PUT32(PORT_DIRSETA,(1<<8)|(1<<9));

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,1000000-1);
    PUT32(STK_CSR,0x00000005);

    for(rx=0;rx<3;rx++)
    {
        PUT32(PORT_OUTSETA,(1<<8)|(1<<9));
        do_delay(2);
        PUT32(PORT_OUTCLRA,(1<<8)|(1<<9));
        do_delay(2);
    }


    ra=GET32(OSC8M);
    ra&=~(3<<8);
    PUT32(OSC8M,ra);

    while(1)
    {
        PUT32(PORT_OUTSETA,(1<<8)|(1<<9));
        do_delay(2);
        PUT32(PORT_OUTCLRA,(1<<8)|(1<<9));
        do_delay(2);
    }


}

/*

halt
flash write_image erase blinker03/notmain.elf

*/

