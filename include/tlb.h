#ifndef _TLB_H_
#define _TLB_H_

#include <common.h>
#include <mips.h>

/* C0_CONTEXT register */
#define PTEBASE_MASK 0xff800000
#define BADVPN2_MASK 0x007ffff0
#define BADVPN2_SHIFT 9

/* MIPS® Architecture For Programmers Volume III, section 9.6 */
#define PTE_NO_READ     0x80000000
#define PTE_NO_EXEC     0x40000000
#define PTE_PFN_MASK    0x03ffffc0
#define PTE_PFN_SHIFT   6
#define PTE_CACHE_MASK  0x00000038
#define PTE_CACHE_SHIFT 3
#define PTE_DIRTY       0x00000004
#define PTE_VALID       0x00000002
#define PTE_GLOBAL      0x00000001

#define PTE_PFN(addr)     (((addr) >> PTE_PFN_SHIFT) & PTE_PFN_MASK)
#define PTE_CACHE(cache)  (((cache) << PTE_CACHE_SHIFT) & PTE_CACHE_MASK)
#define PTE_PFN_OF(pte)   (((pte) & PTE_PFN_MASK) << PTE_PFN_SHIFT)
#define PTE_CACHE_OF(pte) (((cache) & PTE_CACHE_MASK) >> PTE_CACHE_MASK)

#define PTE_VPN2_MASK 0xffffe000
#define PTE_ASID_MASK 0x000000ff

#define PTE_VPN2(addr) ((addr) & PTE_VPN2_MASK)
#define PTE_ASID(asid) ((asid) & PTE_VPN2_MASK)

#define PTE_BASE MIPS_KSEG2_START

void tlb_init();
void tlb_print();

/* 
 * Note that MIPS implements variuos page sizes by specifying PageMask register,
 * so intuitively these functions shall specify PageMask. However in current
 * implementation we aren't going to use other page size than 4KB.
 */

/* Returns the number of entries in the TLB. */
uint32_t tlb_size();

/* Probes the TLB for an entry matching hi, and if present invalidates it. */
void tlb_invalidate(tlbhi_t hi);

/* Invalidate the entire TLB. */
void tlb_invalidate_all();

/* Reads the TLB entry with specified by index, and returns the EntryHi,
 * EntryLo0, EntryLo1, and parts in *hi, *lo0, *lo1 respectively. */
void tlb_read_index(tlbhi_t *hi, tlblo_t *lo0, tlblo_t *lo1, int index);

/* Writes hi, lo0, lo1 into the TLB entry specified by index. */
void tlb_write_index(tlbhi_t hi, tlblo_t lo0, tlblo_t lo1, int i);

/* Writes hi, lo0, lo1 and msk into the TLB entry specified by the Random
 * Register. */
void tlb_write_random(tlbhi_t hi, tlblo_t lo0, tlblo_t lo1);

/* Probes the TLB for an entry matching hi and returns its index, or -1 if not
 * found. If found, then the EntryLo0, EntryLo1 and PageMask parts of the entry
 * are also returned in *plo0, *plo1 respectively. */
void tlb_probe2(tlbhi_t hi, tlblo_t *lo0, tlblo_t *lo1);

/* If there is entry matching hi overwrites it, else writes into random
 * register. Safest way to update TLB. */
void tlb_overwrite_random(tlbhi_t hi, tlblo_t lo0, tlblo_t lo1);

#endif /* _TLB_H */
