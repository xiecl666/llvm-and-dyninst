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