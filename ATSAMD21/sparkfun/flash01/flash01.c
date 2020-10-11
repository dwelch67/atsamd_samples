
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#include "flash-bin.h"

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
#define SERCOM5_INTFLAG     (SERCOM5_BASE+0x18)
#define SERCOM5_SYNCBUSY    (SERCOM5_BASE+0x1C)
#define SERCOM5_DATA        (SERCOM5_BASE+0x28)

#define SERCOM0_BASE        0x42000800
#define SERCOM0_CTRLA       (SERCOM0_BASE+0x00)
#define SERCOM0_CTRLB       (SERCOM0_BASE+0x04)
#define SERCOM0_BAUD        (SERCOM0_BASE+0x0C)
#define SERCOM0_INTFLAG     (SERCOM0_BASE+0x18)
#define SERCOM0_SYNCBUSY    (SERCOM0_BASE+0x1C)
#define SERCOM0_DATA        (SERCOM0_BASE+0x28)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define ACTLR 0xE000E008
#define CPUID 0xE000ED00

#define NVMCTRL_BASE 0x41004000
#define NVM_CTRLA   (NVMCTRL_BASE+0x00)
#define NVM_CTRLB   (NVMCTRL_BASE+0x04)
#define NVM_PARAM   (NVMCTRL_BASE+0x08)
#define NVM_INTFLAG (NVMCTRL_BASE+0x14)
#define NVM_STATUS  (NVMCTRL_BASE+0x18)
#define NVM_ADDR    (NVMCTRL_BASE+0x1C)
#define NVM_LOCK    (NVMCTRL_BASE+0x20)

//------------------------------------------------------------------------
static void clock_init ( void )
{
    unsigned int ra;

    ra=GET32(OSC8M);
    ra&=~(3<<8);
    PUT32(OSC8M,ra);
}
//------------------------------------------------------------------------
#ifdef USE_SERCOM0
//TX PA10  SERCOM0 PAD[2] FUNCTION C  SERCOM2 PAD[2]  FUNCTION D
//RX PA11  SERCOM0 PAD[3] FUNCTION C  SERCOM2 PAD[3]  FUNCTION D
//------------------------------------------------------------------------
static void uart_init ( void )
{
    unsigned int ra;

    ra=GET32(APBCMASK);
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
}
//------------------------------------------------------------------------
//static void uart_flush ( void )
//{
    //while(1)
    //{
        //if(GET8(SERCOM0_INTFLAG)&2) break;
    //}
//}
//------------------------------------------------------------------------
static void uart_send ( unsigned int d )
{
    while(1)
    {
        if(GET8(SERCOM0_INTFLAG)&1) break;
    }
    PUT8(SERCOM0_DATA,d&0xFF);
}
//------------------------------------------------------------------------
//static unsigned int uart_recv ( void )
//{
    //while(1)
    //{
        //if(GET8(SERCOM0_INTFLAG)&4) break;
    //}
    //return(GET8(SERCOM0_DATA)&0xFF);
//}
//------------------------------------------------------------------------
#else
//TX PB22   SERCOM5 PAD[2] PORT FUNCTION D
//RX PB23   SERCOM5 PAD[3] PORT FUNCTION D
//------------------------------------------------------------------------
static void uart_init ( void )
{
    unsigned int ra;

    ra=GET32(APBCMASK);
    ra|=1<<7; //enable SERCOM5
    ra|=1<<2; //enable SERCOM0
    PUT32(APBCMASK,ra);

    PUT32(GCLK_GENCTRL,0x00010605);

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
}
//------------------------------------------------------------------------
//static void uart_flush ( void )
//{
    //while(1)
    //{
        //if(GET8(SERCOM5_INTFLAG)&2) break;
    //}
//}
//------------------------------------------------------------------------
static void uart_send ( unsigned int d )
{
    while(1)
    {
        if(GET8(SERCOM5_INTFLAG)&1) break;
    }
    PUT8(SERCOM5_DATA,d&0xFF);
}
//------------------------------------------------------------------------
//static unsigned int uart_recv ( void )
//{
    //while(1)
    //{
        //if(GET8(SERCOM5_INTFLAG)&4) break;
    //}
    //return(GET8(SERCOM5_DATA)&0xFF);
//}
//------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------
static void hexstrings ( unsigned int d )
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
//------------------------------------------------------------------------
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
static void flash_busy ( void )
{
    while(1)
    {
        if(GET8(NVM_INTFLAG)&(1<<0)) break;
    }
}
//------------------------------------------------------------------------
static void flash_command ( unsigned int cmd )
{
    PUT16(NVM_CTRLA,0xA500+cmd);
    flash_busy();
}
//------------------------------------------------------------------------
#define FLASH_ER 0x02
#define FLASH_WP 0x04
#define FLASH_UR 0x41
#define FLASH_PBC 0x44
#define FLASH_INVALL 0x46
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int addr;
    unsigned int page_size;
    unsigned int row_size;
    unsigned int pages;
    unsigned int rows;


    clock_init();
    uart_init();
    hexstring(0x12345678);
    hexstring(GET32(ACTLR));
    hexstring(GET32(CPUID));
    hexstring(GET32(NVM_PARAM));
    ra=GET32(NVM_PARAM);
    pages=ra&0xFFFF;
    page_size=(ra>>16)&7;
    page_size=8<<page_size;
    row_size=page_size<<2;
    rows=pages>>2;
    hexstring(pages);
    hexstring(page_size);
    hexstring(rows);
    hexstring(row_size);

    flash_busy();
    flash_command(FLASH_INVALL); //where do you use this if at all?
    for(addr=0x0000;addr<0x8000;addr+=0x100)
    {
        hexstrings(addr); hexstring(GET8(NVM_INTFLAG));
        PUT32(NVM_ADDR,addr);
        flash_command(FLASH_UR); //unlock
        flash_command(FLASH_ER); //erase row
    }

    for(ra=0x0000;ra<0x0040;ra+=4)
    {
        hexstrings(ra); hexstring(GET32(ra));
    }

if(1)
{
    flash_command(FLASH_INVALL); //where do you use this if at all?
    flash_command(FLASH_PBC); //page buffer clear
    for(addr=0x0000,ra=0;ra<(0x800>>2);ra++,addr+=4)
    {
        if((addr&0x3F)==0) hexstring(addr);
        PUT32(addr,rom[ra]);
        if((addr&0x3F)==0x3C) flash_busy();
    }
    for(ra=0x0000;ra<0x0040;ra+=4)
    {
        hexstrings(ra); hexstring(GET32(ra));
    }
}

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
