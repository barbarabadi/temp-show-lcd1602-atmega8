#include <stdio.h>
#include <stdlib.h>
#include <delay.h>
#include <alcd.h>

#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))

unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
delay_us(10);
ADCSRA|=(1<<ADSC);
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}
void showtemp(unsigned int adcval)
{
   static int i=0;
   int tm, tm2;
   char buffer[12];
   
   i++;
   if (i>20)
   {
      i=0;
      tm=adcval/4;
      tm2=(adcval-tm*4)*10/4;
      sprintf(buffer,"+%2d.%1d",tm,tm2);
      lcd_gotoxy(9,0);
      lcd_puts(buffer);
      lcd_putchar(223);
      lcd_putchar('C');
   } 
}
void main(void)
{
char play[5][6]={".    ","..   ","...  ",".... ","....."};
int blink_cnt=0,sch=0,direc=1;
unsigned char str[15];
unsigned int adcval;
int awecnt=0, pausecnt=0;

ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ACME);

PORTD.0 = 1;
lcd_init(16);
while (1)
      {
          sprintf(str, "TEMP%s",play[sch]);
          lcd_gotoxy(0,0);
          lcd_puts(str);
          if (direc>0)
          { 
             lcd_gotoxy(awecnt/7,1);
          }
          else
          {
             lcd_gotoxy((48-awecnt)/7,1);
          }
          lcd_puts(" AWESOME! ");
          if (awecnt!=24)awecnt++;
          else 
          {
              pausecnt++;
              if (pausecnt==40)
              {
                 pausecnt=0;
                 awecnt++;
              }
          }
          if (awecnt>48)
          {
             awecnt=0;
             direc=-direc;
          }  
          adcval=read_adc(5);     
          showtemp(adcval);
          
          delay_ms(100);

          blink_cnt++;
          if(blink_cnt>=2){
             blink_cnt=0;
             sch++;
             if (sch>=5) sch=0; 
          }  
      }
}
