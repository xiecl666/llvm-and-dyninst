#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/smp.h>

MODULE_AUTHOR("cn");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0");

#if !defined(__arm__) && !defined(__aarch64__)
#error module only support arm32 arm64.
#endif


#ifdef __aarch64__
typedef unsigned long ulint;  //64
#elif defined __arm__
typedef unsigned int ulint;  //32
#endif

static void en_access(void* en)
{
    ulint i=0,tmpvar=0;

#ifdef __aarch64__
    asm volatile("mrs %0, mpidr_el1 ":"=r"(i));
    i =  (i >>8) &0xff;
#else
    asm volatile("mrc p15,0,%0,c0,c0,5 ":"=r"(i));
    i =  i & 3;
#endif  
    asm volatile (  
#ifdef __aarch64__
            "mrs %0,pmuserenr_el0 \n"
            "orr %0, %0,%1 \n"
            "msr pmuserenr_el0,%0"
#else
            "mrc p15, 0, %0, c9, c14, 0 \n"
            "orr %0, %0,%1 \n"
            "mcr p15, 0, %0, c9, c14, 0 \n"
#endif
            :"+r"(tmpvar):"r"(0xf));
    asm volatile(   
#ifdef __aarch64__
            "mrs %0, pmcr_el0 \n"
            "orr %0, %0, %1 \n" //32  0x41
            "bic %0, %0, %2 \n"
            "msr pmcr_el0,%0 \n"
#else
            "mrc p15, 0, %0, c9, c12, 0 \n"
            "orr %0, %0,%1 \n"
            "bic %0, %0, %2 \n"
            "mcr p15, 0, %0, c9, c12, 0 \n"
#endif
            :"+r"(tmpvar):"r"(0x81),"r"(0x28));
    asm volatile(   
#ifdef __aarch64__
            "msr pmcntenset_el0,%1 \n"
            "mrs %0, cntvct_el0 \n"
#else
            "mcr p15, 0, %1, c9, c12, 1 \n"
            "mrc p15, 0, %0, c9, c13, 0 \n"
#endif
            :"=r"(tmpvar) :"r"(0xffffffff));
    printk("core %lu tsc = %lx",(unsigned long)i, (unsigned long)tmpvar );
}

static void restore_access(void* re) {
    ulint i,tmpvar=0;
#ifdef __aarch64__
    asm volatile( "mrs %0, mpidr_el1": "=r"(i));
    i =  (i >> 8)&0xff;
#else
    asm volatile("mrc p15,0,%0,c0,c0,5 \n" : "=r"(i));
    i =  i & 3;
#endif

    asm volatile (
#ifdef __aarch64__
            "mrs %0,pmcr_el0 \n"
            "bic %0,%0, %2 \n"
            "msr pmcr_el0,%0\n"
            "msr pmuserenr_el0,%1\n"
            "mrs %0, cntvct_el0 \n"
#else
            "mrc p15, 0, %0, c9, c14, 0 \n"
            "bic %0,%0, %2 \n"
            "mcr p15, 0, %0, c9, c14, 0 \n"
            "mcr p15, 0, %1, c9, c12, 1 \n"
            "mrc p15, 0, %0, c9, c13, 0 \n"
#endif
            :"+r" (tmpvar):"r"(0),"r"(1));
    printk("un core %lx tsc = %lx",(unsigned long)i, (unsigned long)tmpvar );

}
static int __init  start(void) 
{ 
    on_each_cpu(en_access, NULL, 1);
    printk(KERN_INFO "pmu access enabled\n"); 
    return 0; 
} 

static void __exit stop(void) 
{ 
    on_each_cpu(restore_access, NULL, 1);
    printk(KERN_INFO "pmu access disabled\n"); 
} 

module_init(start); 
module_exit(stop);