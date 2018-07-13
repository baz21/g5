/*
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 */
#ifndef __SIM_PROBE_CACHE_HH__
#define __SIM_PROBE_CACHE_HH__

#include <memory>

#include "base/refcnt.hh"
#include "cpu/minor/dyn_inst.hh"
#include "cpu/static_inst.hh"
#include "sim/probe/probe.hh"

namespace ProbePoints {

/**
 * CPU probe point
 *
 * This probe point provides a unified interface for CPU
 * instrumentation of SimObjects. SimObjects that need CPU
 * instrumentation should implement probes of this type call the
 * notify method with ...
 */
typedef ProbePointArg<RefCountingPtr<StaticInst>> CPU;
typedef std::unique_ptr<CPU> CPUPtr;

#if 0
typedef ProbePointArg<RefCountingPtr<Minor::MinorDynInstPtr>> CPUMinor;
typedef std::unique_ptr<CPUMinor> CPUMinorPtr;
#endif

}

#endif
