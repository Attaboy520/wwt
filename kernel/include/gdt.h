
#ifndef _KERNEL_GDT_H_
#define _KERNEL_GDT_H_

#include <stdint.h>

#define GDT_CS        (0x00180000000000)  /*** code segment descriptor ***/
#define GDT_DS        (0x00100000000000)  /*** data segment descriptor ***/

#define C             (0x00040000000000)  /*** conforming ***/
#define DPL0          (0x00000000000000)  /*** descriptor privilege level 0 ***/
#define DPL1          (0x00200000000000)  /*** descriptor privilege level 1 ***/
#define DPL2          (0x00400000000000)  /*** descriptor privilege level 2 ***/
#define DPL3          (0x00600000000000)  /*** descriptor privilege level 3 ***/
#define P             (0x00800000000000)  /*** present ***/
#define L             (0x20000000000000)  /*** long mode ***/
#define D             (0x40000000000000)  /*** default op size ***/
#define W             (0x00020000000000)  /*** writable data segment ***/

#define MAX_GDT 32

typedef struct _tss {
    uint32_t reserved0;
    void* rsp0;
    void* rsp1;
    void* rsp2;
    uint32_t reserved1;
    uint32_t reserved2;
    void* ist1;
    void* ist2;
    void* ist3;
    void* ist4;
    void* ist5;
    void* ist6;
    void* ist7;
    uint32_t reserved3;
    uint32_t reserved4;
    uint16_t reserved5;
    uint16_t iopb;
}__attribute__((packed)) TSS;

typedef struct _systemSegmentDescriptor {
  uint64_t lsbLimit :16;/* segment extent (lsb) */
  uint64_t lsbBase :24; /* segment base address (lsb) */
  uint64_t type :5; /* segment type */
  uint64_t dpl :2; /* segment descriptor priority level */
  uint64_t present :1; /* segment descriptor present */
  uint64_t msbLimit :4; /* segment extent (msb) */
  uint64_t reserved0 :3; /* avl, long and def32 (not used) */
  uint64_t gran :1; /* limit granularity (byte/page) */
  uint64_t msbBase :40; /* segment base address (msb) */
  uint64_t reserved1 :8; /* reserved */
  uint64_t zero :5; /* must be zero */
  uint64_t reserved2 :19; /* reserved */
}__attribute__((packed)) SystemSegmentDescriptor;

typedef struct _gdtr {
    uint16_t limit;
    uint64_t offset;
} __attribute__((packed)) GDTR;

void initializeGDT(void);


#endif
