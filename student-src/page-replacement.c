#include <stdlib.h>

#include "types.h"
#include "pagetable.h"
#include "global.h"
#include "process.h"

/*******************************************************************************
 * Finds a free physical frame. If none are available, uses a clock sweep
 * algorithm to find a used frame for eviction.
 *
 * @return The physical frame number of a free (or evictable) frame.
 */
pfn_t get_free_frame(void) {
   int i;

   /* See if there are any free frames */
   for (i = 0; i < CPU_NUM_FRAMES; i++) {
      if (rlt[i].pcb == NULL) {
         return (pfn_t) i;
      }
   }


   /* FIX ME : Problem 5 */
   /* IMPLEMENT A CLOCK SWEEP ALGORITHM HERE */
   /* Note: Think of what kinds of frames can you return before you decide
      to evit one of the pages using the clock sweep and return that frame */

   i = 0;
   while (1) {  
      if (rlt[i].pcb->pagetable[rlt[i].vpn].used == 0) {
         return (pfn_t) i;
      } else {
         rlt[i].pcb->pagetable[rlt[i].vpn].used = 0;
         i++;
         i  = i % CPU_NUM_FRAMES;
      }

   }

   // return 0;


   /* If all else fails, return a random frame */
   // return rand() % CPU_NUM_FRAMES;
}
