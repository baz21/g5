/*
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 */
#ifndef __SIM_PROBE_CACHE_HH__
#define __SIM_PROBE_CACHE_HH__

#include <memory>

#include "sim/probe/probe.hh"

namespace ProbePoints {

/**
 * CACHE probe point
 *
 * This probe point provides a unified interface for CACHE
 * instrumentation of SimObjects. SimObjects that need CACHE
 * instrumentation should implement probes of this type call the
 * notify method with ...
 */
typedef ProbePointArg<std::tuple<Addr, Addr, uint64_t, const std::string>>
    CACHE;
typedef std::unique_ptr<CACHE> CACHEUPtr;

}

#endif
