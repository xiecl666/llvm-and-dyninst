#include<stdio.h>
void set_event_count() {
  // set pmselr and pmxevcntr
  asm volatile(
      "msr pmselr_el0, %0 \n"  
      "msr pmxevtyper_el0, %1 \n"  
      :
      : "r"(0), "r"(0x11)
      : "memory");
}
void setzero()
{
  asm volatile(
      "msr pmxevcntr_el0, %0 \n"
      :
      : "r"(0)
      : "memory");
}
unsigned long readcyc()
{
  unsigned long event_count0=0;
  asm volatile(
      // "mrs pmxevcntr_el0, %0 \n"
      "mrs %0, pmxevcntr_el0 \n"
      : "=r"(event_count0)
      :
      : "memory");
  return event_count0;
}
void print(int b1,int b2,long cycles)
{
  printf("d%d_%d cost %lu\n",b1,b2,cycles);
}
unsigned int a[11];
  int time=0;
int main()
{
  int time=0;  int  i,j, temp;
  a[0] = 0; a[1] = 11; a[2]=10;a[3]=9; a[4]=8; 
  a[5]=7; a[6]=6; a[7]=5; a[8] =4; a[9]=3; a[10]=2;
  i = 2;
  while(i <= 10){
      j = i;
      while (a[j] < a[j-1])
      {
	temp = a[j];
	a[j] = a[j-1];
	a[j-1] = temp;
	j--;
      }
      i++;
    }
    return 1;
}