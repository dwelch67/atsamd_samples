
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define PORT_BASE       0x41004400
#define PORT_DIRSETA    (PORT_BASE+0x00+0x08)
#define PORT_OUTCLRA    (PORT_BASE+0x00+0x14)
#define PORT_OUTSETA    (PORT_BASE+0x00+0x18)

void notmain ( void )
{
    unsigned int ra;

    PUT32(PORT_DIRSETA,(1<<8)|(1<<9));
    while(1)
    {
        PUT32(PORT_OUTSETA,(1<<8));
        PUT32(PORT_OUTCLRA,(1<<9));
        for(ra=0;ra<20000;ra++) dummy(ra);
        PUT32(PORT_OUTCLRA,(1<<8));
        PUT32(PORT_OUTSETA,(1<<9));
        for(ra=0;ra<20000;ra++) dummy(ra);
    }
}

/*

halt
flash write_image erase blinker01/notmain.elf

*/

