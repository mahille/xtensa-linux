#ifndef __LINUX_CYCLES_H_
#define __LINUX_CYCLES_H_

#include <linux/linkage.h>

static inline unsigned long _get_cycles(void) {
#ifdef CONFIG_XTENSA
    unsigned val;
    __asm__ volatile (
        "rsr    %0, CCOUNT;"
        : "=a" (val) : : "memory"
    );
    return val;
#else
    unsigned u,l;
    __asm__ volatile ("rdtsc" : "=a" (l), "=d" (u));
    return (unsigned long)u << 32 | l;
#endif
}

static inline unsigned long _gem5_debug(unsigned msg) {
    unsigned long res;
    __asm__ volatile (
        ".byte 0x0F, 0x04;"
        ".word 0x63;"
        : "=a"(res) : "D"(msg)
    );
    return res;
}

static inline unsigned long prof_start(unsigned id) {
    return _gem5_debug(0x1ff10000 | id);
}

static inline unsigned long prof_stop(unsigned id) {
    return _gem5_debug(0x1ff20000 | id);
}

asmlinkage long sys_get_cycles(unsigned long *val);

#endif

