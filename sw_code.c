#include "ioCC2530.h"

#define LED1 P1_0 
#define SW1 P1_2

void delay(unsigned int time){
  unsigned int i;
  unsigned char j;
  for(i=0;i<time;i++)
    for(j=0;j<240;j++){
      asm("NOP");
      asm("NOP");
      asm("NOP");
    }
}

void main(void){
  int a =0;
  P1SEL&=~0x05;
  P1DIR|=0x01;
  P1DIR&=~0x04;
  
  LED1=0;
  while(1){
    if(SW1==0){
      delay(20);
      if(SW1==0){
        a=a==1?0:1;
        while(!SW1);
      }
    }
        if(a==1){
         LED1=~LED1;
         delay(300);
        }
        if(a==0){
         LED1=0;
         delay(300);
        }
  }
}