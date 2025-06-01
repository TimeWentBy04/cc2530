#include<ioCC2530.h>
#include<string.h>
#define LED P1_0
#define uint16 unsigned short
#define uint32 unsigned long
#define uint unsigned int
uint flag,counter=0;
unsigned char s[9];
void adc_Init()
{
  APCFG|=1;
  P0SEL|=(1<<(0));
  P0DIR&=~(1<<(0));
}
void InitLED()
{
  P1SEL&=~0x01;
  P1DIR|=0x01;
  LED=0;
}
uint16 get_adc(void)
{
  uint32 value;
  ADCIF=0;
  ADCCON3=(0x80|0x10|0x00);
  while(!ADCIF)
  {
    ;
  }
  value=ADCH;
  value=value<<8;
  value|=ADCL;
  value=(value*330);
  value=value>>15;
  return (uint16)value;
}
void initUART0()
{
  PERCFG=0x00;
  P0SEL=0x3c;
  U0CSR|=0x80;
  U0BAUD=216;
  U0GCR=10;
  U0UCR|=0x80;
  UTX0IF=0;
  EA=1;
}
void initTimer1()
{
  CLKCONCMD&=0x80;
  T1CTL=0x0E;
  T1CCTL0|=0x04;
  T1CC0L=50000&0xFF;
  T1CC0H=((50000&0xFF00)>>8);
  
  T1IF=0;
  T1STAT&=~0x01;
  TIMIF&=~0x40;
  IEN1|=0x02;
  EA=1;
}
void UART0SendByte(unsigned char c)
{
  U0DBUF=c;
  while(!UTX0IF);
  UTX0IF=0;
}
void UART0SendString(unsigned char *str)
{
  while(*str!='\0')
  {
    UART0SendByte(*str++);
  }
}
void Get_val()
{
  uint16 sensor_val;
  sensor_val=get_adc();
  s[0]=sensor_val/100+'0';
  s[1]='.';
  s[2]=sensor_val/10%10+'0';
  s[3]=sensor_val%10+'0';
  s[4]='V';
  s[5]='\r';
  s[6]='\n';
  s[7]='\0';
}
#pragma vector=T1_VECTOR
__interrupt void T1_ISR()
{
  EA=0;
  counter++;
  T1STAT &=~0x01;
  EA=1;
}
void main()
{
  InitLED();
  initTimer1();
  initUART0();
  adc_Init();
  while(1)
  {
    if(counter>=15)
    {
     counter=0;
     LED=1;
     Get_val();
     UART0SendString("���մ�������ѹֵ��");
     UART0SendString(s);
     LED=0;
    }
  }
}
