/*
 * Guest memory access tracing engine for PowerPC architecture.
 *
 * Provides tools to translate virtual addresses to physical addreses.
 * Provides tools to save traces in trace files.
 *
 *
 *  Copyright (c) 2019 Alexy Torres Aurora Dugo
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"

#include "qemu/host-utils.h"
#include "cpu.h"

#include "disas/disas.h"

#include "exec/exec-all.h"
#include "exec/cpu_ldst.h"
#include "exec/translator.h"
#include "exec/helper-proto.h"
#include "exec/helper-gen.h"
#include "exec/log.h"

#include "trace-tcg.h"
#include "tcg-op.h"

#include "sysemu/kvm.h"

#include "qemu/error-report.h"
#include "internals.h"


#include "../../qem_trace_config.h" /* QEMTrace configuration */

#if QEM_TRACE_ENABLED
#include "qem_trace_engine.h"
#include "../../qem_trace_engine.h"

typedef struct ARMCacheAttrs {
    unsigned int attrs:8; /* as in the MAIR register encoding */
    unsigned int shareability:2; /* as in the SH field of the VMSAv8-64 PTEs */
} ARMCacheAttrs;

extern bool get_phys_addr(CPUARMState *env, target_ulong address,
                          MMUAccessType access_type, ARMMMUIdx mmu_idx,
                          hwaddr *phys_ptr, MemTxAttrs *attrs, int *prot,
                          target_ulong *page_size,
                          ARMMMUFaultInfo *fi, ARMCacheAttrs *cacheattrs);
extern bool regime_translation_disabled(CPUARMState *env,
                                               ARMMMUIdx mmu_idx);

void qem_arm_get_info_addr_mem_trace(CPUARMState *env, vaddr addr,
                                   int real_access_type, ARMMMUIdx mmu_idx,
                                   target_ulong* phys_addr,
                                   int32_t* write_through_enabled,
                                   int32_t* cache_inhibit,
                                   int32_t* coherency_enabled,
                                   int32_t* access_mode)
{
    int prot;
    target_ulong page_size;
    MemTxAttrs attrs;
    ARMMMUFaultInfo fi;
    ARMCacheAttrs cacheattrs;
    *cache_inhibit = -1;
    *coherency_enabled = 1;
    *phys_addr = -1;
    *write_through_enabled = -1;

    get_phys_addr(env, addr,
                        real_access_type, mmu_idx,
                        (hwaddr*)phys_addr, &attrs, &prot,
                        &page_size,
                        &fi, &cacheattrs);
    if(!regime_translation_disabled(env, mmu_idx))
    {
        *cache_inhibit = (cacheattrs.attrs & 0xF) == 0x4; 
        *write_through_enabled = (cacheattrs.attrs & 0x7) < 4 ? 1 : 0; 
        *access_mode = attrs.secure;
    }
    else 
    {
        /* Check cache state */
        if(real_access_type == MMU_DATA_LOAD || real_access_type == MMU_DATA_STORE)
        {
            *cache_inhibit = !((env->cp15.sctlr_ns & 0x2) >> 2);
        }
        else 
        {
            *cache_inhibit = !((env->cp15.sctlr_ns & 0xC) >> 12);
        }
        

        /* Check cache mode TODO*/
        *write_through_enabled = 1; 

        /* Check current CPU mode */
        *access_mode =  (env->uncached_cpsr & 0x1F) != 0x10;
    }
    
#ifdef QEM_TRACE_PHYSICAL_ADDRESS
#if QEM_TRACE_PHYSICAL_ADDRESS == 0
    *phys_addr = addr;
#endif 
#else 
    *phys_addr = addr;
#endif
    
    
}

#endif /* QEM_TRACE_ENABLED */