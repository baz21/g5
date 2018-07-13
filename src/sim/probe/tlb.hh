/*
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 */
#ifndef __SIM_PROBE_TLB_HH__
#define __SIM_PROBE_TLB_HH__

#include <memory>

#include "sim/probe/probe.hh"

namespace ProbePoints {

/**
 * TLB probe point
 *
 * This probe point provides a unified interface for TLB
 * instrumentation of SimObjects. SimObjects that need TLB
 * instrumentation should implement probes of this type call the
 * notify method with ...
 */
typedef ProbePointArg<std::tuple<Addr, Addr, uint64_t>> TLB;
typedef std::unique_ptr<TLB> TLBUPtr;

}

#endif
