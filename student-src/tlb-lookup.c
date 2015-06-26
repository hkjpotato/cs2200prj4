#include <stdlib.h>
#include <stdio.h>
#include "tlb.h"
#include "pagetable.h"
#include "global.h" /* for tlb_size */
#include "statistics.h"

/*******************************************************************************
 * Looks up an address in the TLB. If no entry is found, attempts to access the
 * current page table via cpu_pagetable_lookup().
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t tlb_lookup(vpn_t vpn, int write) {
   pfn_t pfn = 0;

   /* 
    * FIX ME : Step 6
    */

   /* 
    * Search the TLB for the given VPN. Make sure to increment count_tlbhits if
    * it was a hit!
    */

   for (int i = 0; i < tlb_size; i++) {
    if (tlb[i].valid && tlb[i].vpn == vpn) {
      count_tlbhits++;
      tlb[i].dirty = write;
      tlb[i].used = 1;
      pfn = tlb[i].pfn;
      // goto hitHouseKeeping;
      return tlb[i].pfn; 
      /* return pfn here so the rest no need to run */ 
    }
   } 
    

   /* If it does not exist (it was not a hit), call the page table reader */
   pfn = pagetable_lookup(vpn, write);
   //HOUSEKEEPING is done here. In the pagetable_lookup(), if it found a valid pte. It will updates its information 
   //including used, dirty.
   //pagetable_lookup might encounter page fault, in which case the page table will be updated

   /* 
    * Replace an entry in the TLB if we missed. Pick invalid entries first,
    * then do a clock-sweep to find a victim.
    */
    for (int i = 0; i < tlb_size; i++) {
      if (!tlb[i].valid) {
        //if an entry in tlb is invalid, it can be one that is removed by the tlb_clearone. 
        //Thus it is already marked as invalid in the pagetable of that victim process. No need to 
        //update that victim process
        tlb[i].vpn = vpn;
        tlb[i].pfn = pfn;
        tlb[i].valid = 1;
        tlb[i].used = 1;
        tlb[i].dirty = write;
        //we already did houseKeeping in pagetable_lookup()
        return pfn;

      }
    }

    int i = 0;
    while (1) {
      if (!tlb[i].used) {
        tlb[i].vpn = vpn;
        tlb[i].pfn = pfn;
        tlb[i].valid = 1;
        tlb[i].used = 1;
        tlb[i].dirty = write;
        //we already did houseKeeping in pagetable_lookup()
        return pfn;
      } else {
        tlb[i].used = 0;
        i++;
        i  = i % tlb_size;
      }
    }



  // //I am quite confused about this part
  // hitHouseKeeping: 
  //   //If you have a hit, you should not need to access the memory(current_pagetable) again, otherwise tlb is meaningless
  //   //However, this just been used pfn should be updated so that when other process call a page-replacement
  //   //It will have a chance to live longer (and if it has to be swap out, the os should know if it is dirty)
  //   current_pagetable[vpn].used = 1;
  //   current_pagetable[vpn].dirty = write;



   /*
    * Perform TLB house keeping. This means marking the found TLB entry as
    * accessed and if we had a write, dirty. We also need to update the page
    * table in memory with the same data.
    *
    * We'll assume that this write is scheduled and the CPU doesn't actually
    * have to wait for it to finish (there wouldn't be much point to a TLB if
    * we didn't!).
    */

   // return pfn;
}

