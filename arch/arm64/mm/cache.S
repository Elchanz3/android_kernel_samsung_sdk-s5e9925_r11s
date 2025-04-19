/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Cache maintenance
 *
 * Copyright (C) 2001 Deep Blue Solutions Ltd.
 * Copyright (C) 2012 ARM Ltd.
 */

#include <linux/errno.h>
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/assembler.h>
#include <asm/cpufeature.h>
#include <asm/alternative.h>
#include <asm/asm-uaccess.h>

/*
 * __flush_cache_user_range(start,end)
 * Optimized for Exynos 2200 with unrolled loops and forced alignment
 */
SYM_FUNC_START(__flush_cache_user_range)
    uaccess_ttbr0_enable x2, x3, x4
alternative_if ARM64_HAS_CACHE_IDC
    dsb ishst
    b 7f
alternative_else_nop_endif

    dcache_line_size x2, x3                // x2 = cache line size (Exynos 2200: 64 bytes)
    sub x3, x2, #1
    bic x4, x0, x3                        // Align start to cacheline boundary
    add x1, x1, x3
    bic x1, x1, x3                        // Align end to cacheline boundary

1:  // Unrolled loop (4 lines per iteration)
.rept 4
    user_alt 9f, "dc cvau, x4", "dc civac, x4", ARM64_WORKAROUND_CLEAN_CACHE
    add x4, x4, x2
    cmp x4, x1
    b.hs 2f
.endr
    b 1b

2:  dsb ish
7:
alternative_if ARM64_HAS_CACHE_DIC
    isb
    b 8f
alternative_else_nop_endif
    invalidate_icache_by_line x0, x1, x2, x3, 9f

8:  mov x0, #0
    uaccess_ttbr0_disable x1, x2
    ret
9:  mov x0, #-EFAULT
    b 8b
SYM_FUNC_END(__flush_cache_user_range)

/*
 * __inval_dcache_area (Optimized for batch processing)
 */
SYM_FUNC_START_PI(__inval_dcache_area)
    add x1, x1, x0
    dcache_line_size x2, x3
    sub x3, x2, #1
    tst x1, x3
    bic x1, x1, x3
    b.eq 1f
    dc civac, x1                          // Clean/invalidate final line (aligned)

1:  tst x0, x3
    bic x0, x0, x3
    b.eq 2f
    dc civac, x0                          // Clean/invalidate initial line (aligned)
    b 3f

2:  // Main loop processing 4 lines per iteration
.rept 4
    dc ivac, x0                           // Invalidate line
    add x0, x0, x2
    cmp x0, x1
    b.hs 4f
.endr
    b 2b

3:  add x0, x0, x2
4:  dsb sy
    ret
SYM_FUNC_END_PI(__inval_dcache_area)

/*
 * __clean_dcache_area_pop (ARMv9 CVAP optimization)
 */
SYM_FUNC_START_PI(__clean_dcache_area_pop)
    alternative_if_not ARM64_HAS_DCPOP
    b __clean_dcache_area_poc
    alternative_else_nop_endif

    dcache_line_size x2, x3
    sub x3, x2, #1
    bic x0, x0, x3                        // Align to cacheline boundary
1:
.rept 4                                  // Process 4 lines per iteration
    dc cvap, x0                           // Clean to PoP (ARMv9 specific)
    add x0, x0, x2
    cmp x0, x1
    b.hs 2f
.endr
    b 1b

2:  dsb sy
    ret
SYM_FUNC_END_PI(__clean_dcache_area_pop)

/*
 * __dma_flush_area (DSB moved outside loop)
 */
SYM_FUNC_START_PI(__dma_flush_area)
    dcache_line_size x2, x3
    sub x3, x2, #1
    bic x0, x0, x3                        // Align start address
    add x1, x1, x0
    bic x1, x1, x3                        // Align end address

1:  dc civac, x0                          // Clean/invalidate line
    add x0, x0, x2
    cmp x0, x1
    b.lo 1b

    dsb sy                                 // Single barrier after loop
    ret
SYM_FUNC_END_PI(__dma_flush_area)
