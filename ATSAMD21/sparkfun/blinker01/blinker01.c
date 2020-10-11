
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

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    PUT32(PORTADIRSET,1<<17);
    PUT32(PORTADIRSET,1<<27);
    PUT32(PORTBDIRSET,1<<3);

    for(rx=0;;rx++)
    {
        PUT32(PORTAOUTSET,1<<17);
        PUT32(PORTAOUTSET,1<<27);
        PUT32(PORTBOUTSET,1<<3);
        for(ra=0;ra<20000;ra++) dummy(ra);
        PUT32(PORTAOUTCLR,1<<17);
        PUT32(PORTAOUTCLR,1<<27);
        PUT32(PORTBOUTCLR,1<<3);
        for(ra=0;ra<20000;ra++) dummy(ra);
    }
    return(0);
}
