 /*
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 *
 */

#ifndef __CPU_O3_PROBE_SIMPLE_TRACE_CACHE_HH__
#define __CPU_O3_PROBE_SIMPLE_TRACE_CACHE_HH__

#include "cpu/o3/dyn_inst.hh"
#include "cpu/o3/impl.hh"
#include "params/SimpleTraceCACHE.hh"
#include "sim/probe/probe.hh"
#include "sim/probe/tlb.hh"

class SimpleTraceCACHE : public ProbeListenerObject {

  public:
    typedef typename std::tuple<Addr, Addr, uint64_t, const std::string>
        CACHEAddrArgs;

    SimpleTraceCACHE(const SimpleTraceCACHEParams *params):
        ProbeListenerObject(params)
    {
    }

    /** Register the probe listeners. */
    void regProbeListeners();

    /** Returns the name of the trace. */
    const std::string name() const {
        return ProbeListenerObject::name() + ".trace";
    }

  private:
    void traceHit(const CACHEAddrArgs &args);
    void traceMiss(const CACHEAddrArgs &args);

};
#endif//__CPU_O3_PROBE_SIMPLE_TRACE_CACHE_HH__
