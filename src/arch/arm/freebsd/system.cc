/*
 * Copyright (c) 2015 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by the University of Cambridge Computer
 * Laboratory as part of the CTSRD Project, with support from the UK Higher
 * Education Innovation Fund (HEIF).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "arch/arm/freebsd/system.hh"

#include "arch/arm/isa_traits.hh"
#include "arch/arm/utility.hh"
#include "arch/generic/freebsd/threadinfo.hh"
#include "base/loader/dtb_object.hh"
#include "base/loader/object_file.hh"
#include "base/loader/symtab.hh"
#include "cpu/base.hh"
#include "cpu/pc_event.hh"
#include "cpu/thread_context.hh"
#include "debug/Loader.hh"
#include "kern/freebsd/events.hh"
#include "mem/fs_translating_port_proxy.hh"
#include "mem/physical.hh"
#include "sim/stat_control.hh"

using namespace ArmISA;
using namespace FreeBSD;

FreebsdArmSystem::FreebsdArmSystem(Params *p)
    : GenericArmSystem(p),
      enableContextSwitchStatsDump(p->enable_context_switch_stats_dump),
      taskFile(nullptr), kernelPanicEvent(nullptr), kernelOopsEvent(nullptr),
      loaderConfig(p->loader_config)
{
#if 1
    if (p->panic_on_panic) {
        kernelPanicEvent = addKernelFuncEventOrPanic<PanicPCEvent>(
            "panic", "Kernel panic in simulated kernel");
    } else {
#ifndef NDEBUG
        kernelPanicEvent = addKernelFuncEventOrPanic<BreakPCEvent>("panic");
#endif
    }

    if (p->panic_on_oops) {
        kernelOopsEvent = addKernelFuncEventOrPanic<PanicPCEvent>(
            "oops_exit", "Kernel oops in guest");
    }
#endif

    uDelaySkipEvent = addKernelFuncEvent<UDelayEvent>(
        "DELAY", "DELAY", 1000, 0);
}

void
FreebsdArmSystem::initState()
{
    // Moved from the constructor to here since it relies on the
    // address map being resolved in the interconnect

    // Call the initialisation of the super class
    GenericArmSystem::initState();

    // Load symbols at physical address, we might not want
    // to do this permanently, for but early bootup work
    // it is helpful.
    if (params()->early_kernel_symbols) {
        kernel->loadGlobalSymbols(kernelSymtab, 0, 0, loadAddrMask);
        kernel->loadGlobalSymbols(debugSymbolTable, 0, 0, loadAddrMask);
    }

    // Setup boot data structure
    Addr fdt_addr = 0;

    // Check if the kernel image has a symbol that tells us it supports
    // device trees.
    bool kernel_has_fdt_support =
        kernelSymtab->findAddress("fdt_get_range", fdt_addr);
    bool dtb_file_specified = params()->dtb_filename != "";

    if (!dtb_file_specified)
        fatal("dtb file is not specified\n");

    if (!kernel_has_fdt_support)
        fatal("kernel must have fdt support\n");

    // Kernel supports flattened device tree and dtb file specified.
    // Using Device Tree Blob to describe system configuration.

    ObjectFile *dtb_file = createObjectFile(params()->dtb_filename, true);
    if (!dtb_file) {
        fatal("couldn't load DTB file: %s\n", params()->dtb_filename);
    }

    DtbObject *_dtb_file = dynamic_cast<DtbObject*>(dtb_file);

    if (_dtb_file) {
        if (!_dtb_file->addBootCmdLine(params()->boot_osflags.c_str(),
                                       params()->boot_osflags.size())) {
            warn("couldn't append bootargs to DTB file: %s\n",
                 params()->dtb_filename);
        }
    } else {
        warn("dtb_file cast failed; couldn't append bootargs "
             "to DTB file: %s\n", params()->dtb_filename);
    }

    /*
     * Loader type.
     *
     * We have to repliacte a lot of work the FreeBSD loader normally
     * does for us.  We do a lot of C-style programming down there.
     *
     * On ARM64 we need to write to physical addresses but write
     * virtual addresses for the kernel.  Quite confusing; apply
     * voffset to all the addresses we write.
     */

    System *sys = threadContexts[0]->getSystemPtr();
    size_t kernsize = sys->getKernelEnd() - sys->getKernelStart();
    Addr addr = roundUp((kernelEntry & loadAddrMask) + loadAddrOffset +
        kernsize, PAGE_SIZE);
    Addr voffset = sys->getKernelEntry() - ((kernelEntry & loadAddrMask) +
        loadAddrOffset);
    DPRINTF(Loader, "Virtual address offset %#x\n", voffset);

    // Load the DTB file; start and end address.
    Addr dtb = addr;
    addr = roundUp(dtb + dtb_file->textSize(), PAGE_SIZE);
    DPRINTF(Loader, "Loaded DTB file %s at = %#x addr now %#x\n",
        params()->dtb_filename, dtb, addr);

    dtb_file->setTextBase(dtb);
    dtb_file->loadSections(physProxy);
    delete dtb_file;

    Addr modulep = addr;
    DPRINTF(Loader, "modulep = %#x\n", modulep);

    // -----------------------------------------------------------------
    // Write preloaded_file 'elf kernel' metainformation
    DPRINTF(Loader, "kernel '%s' at %#x\n", params()->kernel, addr);
    MOD_NAME(addr, params()->kernel, 1);
    DPRINTF(Loader, "'elf kernel' at %#x\n", addr);
    MOD_TYPE(addr, std::string("elf kernel"), 1);
    //MOD_ARGS(addr, ..., 1);
    uint64_t v;
    v = sys->getKernelStart();
    DPRINTF(Loader, "kernel start %#x at %#x\n", v, addr);
    MOD_ADDR(addr, v, 1);
    v = kernsize;
    DPRINTF(Loader, "kernel size %#x at %#x\n", v, addr);
    MOD_SIZE(addr, v, 1);

    // -----------------------------------------------------------------
    // Add various metadata information.

    Addr dtb_addr = addr;
    dtb += voffset;
    MOD_METADATA(addr, MODINFOMD_DTBP, sizeof(dtb), &dtb, 1);
    DPRINTF(Loader, "MODINFOMD_DTBP %#x written out at = %#x; addr advanced "
        "to %#x\n", dtb, dtb_addr, addr);

    const char *kargs;
    const char *cp;
    int         howto;
    int         active;

    /* Parse kargs */
    DPRINTF(Loader, "boot_osflags=%s\n", params()->boot_osflags);
    kargs = params()->boot_osflags.c_str();
    howto = 0;
    if (kargs != NULL) {
        cp = kargs;
        active = 0;
        while (*cp != 0) {
            if (!active && (*cp == '-')) {
                active = 1;
            } else if (active)
                switch (*cp) {
                case 'a':
                    howto |= RB_ASKNAME;
                    break;
                case 'C':
                    howto |= RB_CDROM;
                    break;
                case 'd':
                    howto |= RB_KDB;
                    break;
                case 'D':
                    howto |= RB_MULTIPLE;
                    break;
                case 'm':
                    howto |= RB_MUTE;
                    break;
                case 'g':
                    howto |= RB_GDB;
                    break;
                case 'h':
                    howto |= RB_SERIAL;
                    break;
                case 'p':
                    howto |= RB_PAUSE;
                    break;
                case 'r':
                    howto |= RB_DFLTROOT;
                    break;
                case 's':
                    howto |= RB_SINGLE;
                    break;
                case 'v':
                    howto |= RB_VERBOSE;
                    break;
                default:
                    active = 0;
                    break;
                }
            cp++;
        }
    }
    DPRINTF(Loader, "BootHowTo=%#x, writing to addr %#x\n", howto, addr);
    MOD_METADATA(addr, MODINFOMD_HOWTO, sizeof(howto), &howto, 1);

    // Don't write it out but consume the space and remember the addr
    // for the meta information.
    Addr envp = 0;
    Addr envp_meta_addr = 0;
    if (loaderConfig.size() > 0) {
        envp_meta_addr = addr;
        MOD_METADATA(addr, MODINFOMD_ENVP, sizeof(envp), &envp, 0);
        DPRINTF(Loader, "MODINFOMD_ENVP made space at = %#x; "
            "addr advanced to %#x\n", envp_meta_addr, addr);
    }

    Addr kea = 0xdeadcafe + voffset;
    Addr kernend_addr = addr;
    MOD_METADATA(addr, MODINFOMD_KERNEND, sizeof(kea), &kea, 0);
    DPRINTF(Loader, "MODINFOMD_KERNEND %#x made space at %#x; "
        "addr advanced to %#x\n", kea, kernend_addr, addr);

#if 0
    file_addmetadata(kfp, MODINFOMD_MODULEP, sizeof module, &module);
#endif

    // -----------------------------------------------------------------
    // end.
    DPRINTF(Loader, "MOD_END marker at addr %#x\n", addr);
    MOD_END(addr, 1);


    Addr size = 0;
#if 0
    // XXXX-BZ We do not support loading kernel modules yet.
    size = bi_copymodules64(0);
#endif

    /* Copy our environment. */
    if (envp_meta_addr != 0x0) {
        std::vector<std::string>::iterator i;
        Addr p;
        const uint8_t z = '\0';

        p = envp = roundUp(addr + size, PAGE_SIZE);
        i = loaderConfig.begin();
        while (i != loaderConfig.end()) {
            std::string s = *i;
            physProxy.writeBlob(p, (uint8_t *)s.c_str(), s.length());
            p += s.length();
            i++;
            physProxy.writeBlob(p++, &z, 1);
            DPRINTF(Loader, "MODINFOMD_ENVP envp %#x %s %d \\0\n",
                    p, s.c_str(), s.length());
        }
        physProxy.writeBlob(p++, &z, 1);
        addr = p;
        DPRINTF(Loader, "MODINFOMD_ENVP envp %#x at %#x\n", envp, addr);
    }

    /* Set kernend. */
    kernsize = addr - sys->getKernelStart();
    //Addr kernend = roundUp(sys->getKernelEntry() + kernsize , PAGE_SIZE);
    Addr kernend = roundUp(addr , PAGE_SIZE) + voffset;
    DPRINTF(Loader, "kernend (post-modules) = %#x (size=%#x) written to %#x\n",
        kernend, kernsize, kernend_addr);
    /* Update the kernend_addr value in place of the reserved space. */
    MOD_METADATA(kernend_addr, MODINFOMD_KERNEND, sizeof(kernend),
        &kernend, 1);
    DPRINTF(Loader, "MODINFOMD_KERNEND %#x updated at %#x\n",
        kernend, kernend_addr);

    /* Patch MODINFOMD_ENVP. */
    // We actually only write it out now.
    if (envp_meta_addr != 0x0) {
        envp += voffset;
        DPRINTF(Loader, "MODINFOMD_ENVP set at %#x to %#x\n",
            envp_meta_addr, envp);
        MOD_METADATA(envp_meta_addr, MODINFOMD_ENVP, sizeof(envp), &envp, 1);
    }

#if 0
    /* copy module list and metadata */
    (void)bi_copymodules64(*modulep);
#endif

    /*
     * Here's what the kernel expects to find and where.
     * And that's what we write out.
     */

    // Kernel boot requirements to set up r0, r1 and r2 in ARMv7
    for (int i = 0; i < threadContexts.size(); i++) {
#if 1
        threadContexts[i]->setIntReg(0, modulep + voffset);
#else
        threadContexts[i]->setIntReg(0, 0);
#endif
        threadContexts[i]->setIntReg(1, params()->machine_type);
        threadContexts[i]->setIntReg(2, params()->atags_addr + loadAddrOffset);
    }
}

FreebsdArmSystem::~FreebsdArmSystem()
{
    if (uDelaySkipEvent)
        delete uDelaySkipEvent;
    if (constUDelaySkipEvent)
        delete constUDelaySkipEvent;
}

FreebsdArmSystem *
FreebsdArmSystemParams::create()
{
    return new FreebsdArmSystem(this);
}

void
FreebsdArmSystem::startup()
{
}
