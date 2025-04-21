#ifndef __ASSEMBLER__

#include "types.h"
#include "macros.h"

// Read a CSR 
#define asm_csrr_r(n,r) \
static inline uint64 r_ ##n() {                 \
  uint64 x;                                     \
  asm volatile("cssr %0, " STR(r) : "=r" (x) ); \
  return x;                                     \
}
#define asm_csr_r(n) asm_csrr_r(n,n)

// Write into a CSR
#define asm_csrr_w(n,r) \
static inline void w_ ##n(uint64 x) {              \
  asm volatile("cssw " STR(r) ",%0" : : "r" (x) ); \
}
#define asm_csr_w(n) asm_csrr_w(n,n)

// Read a register
#define asm_reg_r(r)                          \
static inline uint64 r_ ##r() {               \
uint64 x;                                     \
  asm volatile("mv %0, " STR(r) : "=r" (x) ); \
  return x;                                   \
}

// Write into a register
#define asm_reg_w(r)                          \
static inline void r_ ##r( uint64 x) {        \
  asm volatile("mv " STR(r) ",%0" : : "r" (x) ); \
}

// which hart (core) is this?
asm_csr_r(mhartid)

// Machine Status Register, mstatus

#define MSTATUS_MPP_MASK (3L << 11) // previous mode.
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define MSTATUS_MPP_U (0L << 11)
#define MSTATUS_MIE (1L << 3)    // machine-mode interrupt enable.

asm_csr_r(mstatus)
asm_csr_w(mstatus)

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
asm_csr_w(mepc)

// Supervisor Status Register, sstatus

#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4) // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)  // User Interrupt Enable

asm_csr_r(sstatus)
asm_csr_w(sstatus)

// Supervisor Interrupt Pending
asm_csr_r(sip)
asm_csr_w(sip)

// Supervisor Interrupt Enable
#define SIE_SEIE (1L << 9) // external
#define SIE_STIE (1L << 5) // timer
#define SIE_SSIE (1L << 1) // software

asm_csr_r(sie)
asm_csr_w(sie)

// Machine-mode Interrupt Enable
#define MIE_STIE (1L << 5)  // supervisor timer

asm_csr_r(mie)
asm_csr_w(mie)

// supervisor exception program counter, holds the
// instruction address to which a return from
// exception will go.

asm_csr_r(sepc)
asm_csr_w(sepc)

// Machine Exception Delegation

asm_csr_r(medeleg)
asm_csr_w(medeleg)

// Machine Interrupt Delegation
asm_csr_r(mideleg)
asm_csr_w(mideleg)

// Supervisor Trap-Vector Base Address
// low two bits are mode.

asm_csr_r(stvec)
asm_csr_w(stvec)

// Supervisor Timer Comparison Register
asm_csrr_r(stimecmp,0x14d)
asm_csrr_w(stimecmp,0x14d)

// Machine Environment Configuration Register
asm_csrr_r(menvcfg,0x30a)
asm_csrr_w(menvcfg,0x30a)

// Physical Memory Protection
asm_csr_w(pmpcfg0)
asm_csr_w(pmpaddr0)

// use riscv's sv39 page table scheme.
#define SATP_SV39 (8L << 60)

#define MAKE_SATP(pagetable) (SATP_SV39 | (((uint64)pagetable) >> 12))

// supervisor address translation and protection;
// holds the address of the page table.
asm_csr_r(satp)
asm_csr_w(satp)

// Supervisor Trap Cause
asm_csr_r(scause)

// Supervisor Trap Value
asm_csr_r(stval)

// Machine-mode Counter-Enabler
asm_csr_r(mcounteren)
asm_csr_w(mcounteren)

// machine-mode cycle counter
asm_csr_r(time)

// enable device interrupts
static inline void
intr_on()
{
  w_sstatus(r_sstatus() | SSTATUS_SIE);
}

// disable device interrupts
static inline void
intr_off()
{
  w_sstatus(r_sstatus() & ~SSTATUS_SIE);
}

// are device interrupts enabled?
static inline int
intr_get()
{
  uint64 x = r_sstatus();
  return (x & SSTATUS_SIE) != 0;
}

asm_reg_r(sp)

// read and write tp, the thread pointer, which xv6 uses to hold
// this core's hartid (core number), the index into cpus[].
asm_reg_r(tp)
asm_reg_w(tp)

asm_reg_r(ra)

// flush the TLB.
static inline void
sfence_vma()
{
  // the zero, zero means flush all TLB entries.
  asm volatile("sfence.vma zero, zero");
}

typedef uint64 pte_t;
typedef uint64 *pagetable_t; // 512 PTEs

#endif // __ASSEMBLER__

#define PGSIZE 4096 // bytes per page
#define PGSHIFT 12  // bits of offset within a page

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

#define PTE_V (1L << 0) // valid
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4) // user can access

// shift a physical address to the right place for a PTE.
#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)

#define PTE2PA(pte) (((pte) >> 10) << 12)

#define PTE_FLAGS(pte) ((pte) & 0x3FF)

// extract the three 9-bit page table indices from a virtual address.
#define PXMASK          0x1FF // 9 bits
#define PXSHIFT(level)  (PGSHIFT+(9*(level)))
#define PX(level, va) ((((uint64) (va)) >> PXSHIFT(level)) & PXMASK)

// one beyond the highest possible virtual address.
// MAXVA is actually one bit less than the max allowed by
// Sv39, to avoid having to sign-extend virtual addresses
// that have the high bit set.
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))
