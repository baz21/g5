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

#ifndef __ARCH_ARM_FREEBSD_SYSTEM_HH__
#define __ARCH_ARM_FREEBSD_SYSTEM_HH__

#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "arch/arm/system.hh"
#include "base/output.hh"
#include "kern/freebsd/events.hh"
#include "params/FreebsdArmSystem.hh"
#include "sim/core.hh"

/*
 * The following definitions have been replicated from FreeBSD's
 * sys/boot/.. directory.
 * We wish that the FreeBSD framework had a proper single source
 * for these things rather than a lot of copy&paste, so we could just
 * include a clean file.
 */

/* Page size definition for amd64, so we can properly align things. */
#define PAGE_SHIFT  12              /* LOG2(PAGE_SIZE) */
#define PAGE_SIZE   (1<<PAGE_SHIFT) /* bytes/page */

/*
 * Modinfo definitions in order to pass meta-information in to the
 * kernel environment.
 */
/* From src/sys/linker.h */
#define MODINFO_END             0x0000          /* End of list */
#define MODINFO_NAME            0x0001          /* Name of module (string) */
#define MODINFO_TYPE            0x0002          /* Type of module (string) */
#define MODINFO_ADDR            0x0003          /* Loaded address */
#define MODINFO_SIZE            0x0004          /* Size of module */
#define MODINFO_EMPTY           0x0005          /* Has been deleted */
#define MODINFO_ARGS            0x0006          /* Parameters string */
#define MODINFO_METADATA        0x8000          /* Module-specfic */

#define MODINFOMD_ENVP          0x0006          /* envp[] */
#define MODINFOMD_HOWTO         0x0007          /* boothowto */
#define MODINFOMD_KERNEND       0x0008          /* kernend */

/* From src/sys/arm64/include/metadata.h */
#define MODINFOMD_DTBP          0x1002

/*
 * Adjusted macros to write out the individual modinfo meta-information.
 */
/* Based on src/sys/boot/i386/libi386/bootinfo64.c */
/*
 * Copy module-related data into the load area, where it can be
 * used as a directory for loaded modules.
 *
 * Module data is presented in a self-describing format.  Each datum
 * is preceded by a 32-bit identifier and a 32-bit size field.
 *
 * Currently, the following data are saved:
 *
 * MOD_NAME     (variable)              module name (string)
 * MOD_TYPE     (variable)              module type (string)
 * MOD_ARGS     (variable)              module parameters (string)
 * MOD_ADDR     sizeof(vm_offset_t)     module load address
 * MOD_SIZE     sizeof(size_t)          module size
 * MOD_METADATA (variable)              type-specific metadata
 */
#define	PADDING()	(inAArch64(threadContexts[0]) ? sizeof(u_int64_t) : \
    sizeof(u_int32_t))

#define COPY32(v, a, c) {                       \
    u_int32_t   x = (v);                        \
    if (c)                                      \
        physProxy.writeBlob(a, (uint8_t *)&x, sizeof(x)); \
    a += sizeof(x);                             \
}

#define MOD_STR(t, a, s, c) {                   \
    COPY32(t, a, c);                            \
    COPY32(s.size() + 1, a, c);                 \
    if (c) {                                    \
        physProxy.writeBlob(a, (uint8_t *)s.c_str(), s.size() + 1); \
    }                                           \
    a += roundUp(s.size() + 1, PADDING()); \
}

#define MOD_NAME(a, s, c)       MOD_STR(MODINFO_NAME, a, s, c)
#define MOD_TYPE(a, s, c)       MOD_STR(MODINFO_TYPE, a, s, c)
#define MOD_ARGS(a, s, c)       MOD_STR(MODINFO_ARGS, a, s, c)

#define MOD_VAR(t, a, s, c) {                   \
    COPY32(t, a, c);                            \
    COPY32(sizeof(s), a, c);                    \
    if (c)                                      \
        physProxy.writeBlob(a, (uint8_t *)&s, sizeof(s)); \
    a += roundUp(sizeof(s), PADDING()); \
}

#define MOD_ADDR(a, s, c)       MOD_VAR(MODINFO_ADDR, a, s, c)
#define MOD_SIZE(a, s, c)       MOD_VAR(MODINFO_SIZE, a, s, c)

#define MOD_METADATA(a, type, size, data, c) {  \
    COPY32(MODINFO_METADATA | type, a, c);      \
    COPY32(size, a, c);                         \
    if (c)                                      \
        physProxy.writeBlob(a, (uint8_t *)data, size); \
    a += roundUp(size, PADDING());      \
}

#define MOD_END(a, c) {                         \
    COPY32(MODINFO_END, a, c);                  \
    COPY32(0, a, c);                            \
}

/*
 * BootHowTo definitions.
 */
/* Based on src/sys/boot/i386/libi386/bootinfo.c, src/sys/sys/reboot.c. */
#define RB_AUTOBOOT  0       /* flags for system auto-booting itself */
#define RB_ASKNAME   0x001   /* ask for file name to reboot from */
#define RB_SINGLE    0x002   /* reboot to single user only */
#define RB_NOSYNC    0x004   /* dont sync before reboot */
#define RB_HALT      0x008   /* don't reboot, just halt */
#define RB_INITNAME  0x010   /* name given for /etc/init (unused) */
#define RB_DFLTROOT  0x020   /* use compiled-in rootdev */
#define RB_KDB       0x040   /* give control to kernel debugger */
#define RB_RDONLY    0x080   /* mount root fs read-only */
#define RB_DUMP      0x100   /* dump kernel memory before reboot */
#define RB_MINIROOT  0x200   /* mini-root present in memory at boot time */
#define RB_VERBOSE   0x800   /* print all potentially useful info */
#define RB_SERIAL    0x1000  /* use serial port as console */
#define RB_CDROM     0x2000  /* use cdrom as root */
#define RB_POWEROFF  0x4000  /* turn the power off if possible */
#define RB_GDB       0x8000  /* use GDB remote debugger instead of DDB */
#define RB_MUTE      0x10000 /* start up with the console muted */
#define RB_SELFTEST  0x20000 /* don't complete the boot; do selftest */
#define RB_RESERVED1 0x40000 /* reserved for internal use of boot blocks */
#define RB_RESERVED2 0x80000 /* reserved for internal use of boot blocks */
#define RB_PAUSE     0x100000 /* pause after each output line during probe */
#define RB_MULTIPLE  0x20000000 /* use multiple consoles */
#define RB_BOOTINFO  0x80000000 /* have `struct bootinfo *' arg */

class FreebsdArmSystem : public GenericArmSystem
{
  public:
    /** Boilerplate params code */
    typedef FreebsdArmSystemParams Params;
    const Params *
    params() const
    {
        return dynamic_cast<const Params *>(_params);
    }

    /** When enabled, dump stats/task info on context switches for
     *  Streamline and per-thread cache occupancy studies, etc. */
    bool enableContextSwitchStatsDump;

    /** This map stores a mapping of OS process IDs to internal Task IDs. The
     * mapping is done because the stats system doesn't tend to like vectors
     * that are much greater than 1000 items and the entire process space is
     * 65K. */
    std::map<uint32_t, uint32_t> taskMap;

    /** This is a file that is placed in the run directory that prints out
     * mappings between taskIds and OS process IDs */
    std::ostream* taskFile;

    FreebsdArmSystem(Params *p);
    ~FreebsdArmSystem();

    void initState();

    void startup();

    /** This function creates a new task Id for the given pid.
     * @param tc thread context that is currentyl executing  */
    void mapPid(ThreadContext* tc, uint32_t pid);

  private:
    /** Event to halt the simulator if the kernel calls panic()  */
    PCEvent *kernelPanicEvent;

    /** Event to halt the simulator if the kernel calls oopses  */
    PCEvent *kernelOopsEvent;

    /**
     * PC based event to skip udelay(<time>) calls and quiesce the
     * processor for the appropriate amount of time. This is not functionally
     * required but does speed up simulation.
     */
    FreeBSD::UDelayEvent *uDelaySkipEvent;

    /** Another PC based skip event for const_udelay(). Similar to the udelay
     * skip, but this function precomputes the first multiply that is done
     * in the generic case since the parameter is known at compile time.
     * Thus we need to do some division to get back to us.
     */
    FreeBSD::UDelayEvent *constUDelaySkipEvent;

    /** These variables store addresses of important data structures
     * that are normaly kept coherent at boot with cache mainetence operations.
     * Since these operations aren't supported in gem5, we keep them coherent
     * by making them uncacheable until all processors in the system boot.
     */
    Addr secDataPtrAddr;
    Addr secDataAddr;
    Addr penReleaseAddr;
    Addr pen64ReleaseAddr;
    Addr bootReleaseAddr;


    std::vector<std::string> loaderConfig;	// Just tunables a=b
    size_t loaderConfigLen;
};

#endif // __ARCH_ARM_FREEBSD_SYSTEM_HH__

