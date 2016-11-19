#include "xc_config_settings.h"
#include "plib/timers.h"
#include "plib/adc.h"
#include "plib/delays.h"
#include "math.h"

unsigned char volatile sec = 0, min = 0, h = 0, which_display = 0;

void U1_off(void);
void U2_off(void);
void U_off(void);
void U_on(void);
void patterns(unsigned char x);
void enable_global_interrupts(void);
void interrupt isr(void);
void display(unsigned char x);
void time(void);
unsigned char program(void);
void tempe(void);
void declarations(void);
unsigned int get_ADC_value(void);
void notes(char s);

#define one_sec 26473

void main(void)
{
    declarations();
    while(1)
    {
        switch( program() )
        {   case 0: time();break;
            case 1: tempe();break;
            default: LATF=0x7F;break;
        }

    }
}

void declarations(void)
{
    TRISF = 0x00;
    TRISH = 0xFC;
    TRISC = 0x3C;
    LATF=0x00;
    ADCON1=0x0F;
    TRISBbits.RB0 = 1;
    TRISJbits.RJ5 = 1;
    U_off();
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_64 );
    WriteTimer0(one_sec);
    enable_global_interrupts();
}

void time(void)
{
        if(PORTBbits.RB0==0 && which_display<2 && PORTJbits.RJ5 == 1) {which_display++;
                                                                       while(PORTBbits.RB0==0)
                                                                                  LATF=0xFF;
                                                                       }
        else if(PORTBbits.RB0==0 && which_display>=2 && PORTJbits.RJ5 == 1) {which_display=2;}
        if(PORTJbits.RJ5 == 0 && which_display>0 && PORTBbits.RB0 == 1) {which_display--;
                                                                         while(PORTJbits.RJ5==0)
                                                                                  LATF=0xFF;
                                                                        }
          else if(PORTJbits.RJ5 == 0 && which_display<=0 && PORTBbits.RB0 == 1) {which_display=0;}
        switch (which_display)
        {
            case 0 : display(sec);break;
            case 1 : display(min);break;
            case 2 : display(h);break;
        }
}

void tempe(void)
{
    unsigned char temp;
    OpenADC(ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_0_TAD,
            ADC_CH1 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 13);

      temp = ( get_ADC_value() & 0x03FF) >> 2 ;
      temp= round(  temp * 100.0 / 51.0  ) - 273;
      display(temp);
}

void interrupt isr(void)
{
    if(INTCONbits.TMR0IF == 1)
    {
    INTCONbits.TMR0IF = 0;
    sec++;
    if(sec==60) {sec=0;min++;}
    if(min==60) {min=0;  h++;}
    WriteTimer0(one_sec);
    }
    else if(PIR1bits.ADIF)
        PIR1bits.ADIF=0;

}

void U1_off(void)
{
    LATHbits.LATH0=1;
    LATHbits.LATH1=0;
}
void U2_off(void)
{
    LATHbits.LATH0=0;
    LATHbits.LATH1=1;
}

void U_off(void)
{
    LATHbits.LATH0=1;
    LATHbits.LATH1=1;
}
void U_on(void)
{
    LATHbits.LATH0=0;
    LATHbits.LATH1=0;
}

void patterns(unsigned char x)
{
    switch(x)
    {
        case 0: LATF=0x84;break;
        case 1: LATF=0xF5;break;
        case 2: LATF=0x4C;break;
        case 3: LATF=0x64;break;
        case 4: LATF=0x35;break;
        case 5: LATF=0x26;break;
        case 6: LATF=0x06;break;
        case 7: LATF=0xF4;break;
        case 8: LATF=0x04;break;
        case 9: LATF=0x24;break;
    }
}

void enable_global_interrupts(void)
{
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void display(unsigned char x)
{
        U1_off();
        patterns(x/10);
        Delay1KTCYx(29);
        U2_off();
        patterns(x%10);
        Delay1KTCYx(29);
}

unsigned char program(void)
{
    return ( (PORTH&0xF0) + ((PORTC&0x3C)>>2) );
}

unsigned int get_ADC_value(void)
{
    ConvertADC();
    while( BusyADC() )
          ;
    return ReadADC();

}

void notes(char s)
{
    switch(s)
    {
        case 'c': LATJbits.LATJ6=1; Delay1KTCYx(1); LATJbits.LATJ6=0; Delay1KTCYx(1); break;
        case 'd': LATJbits.LATJ6=1; Delay1KTCYx(1); LATJbits.LATJ6=0; Delay1KTCYx(1); break;
        case 'e': LATJbits.LATJ6=1; Delay1KTCYx(1); LATJbits.LATJ6=0; Delay1KTCYx(1); break;
        case 'f': LATJbits.LATJ6=1; Delay1KTCYx(1); LATJbits.LATJ6=0; Delay1KTCYx(1); break;
        case 'g': LATJbits.LATJ6=1; Delay1KTCYx(1); LATJbits.LATJ6=0; Delay1KTCYx(1); break;
        case 'a': LATJbits.LATJ6=1; Delay1KTCYx(1); LATJbits.LATJ6=0; Delay1KTCYx(1); break;
        case 'b': LATJbits.LATJ6=1; Delay1KTCYx(1); LATJbits.LATJ6=0; Delay1KTCYx(1); break;
        case 'C': LATJbits.LATJ6=1; Delay1KTCYx(1); LATJbits.LATJ6=0; Delay1KTCYx(1); break;

    }
}