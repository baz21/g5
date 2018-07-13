 /*
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 *
 */

#ifndef __CPU_O3_PROBE_SIMPLE_TRACE_TLB_HH__
#define __CPU_O3_PROBE_SIMPLE_TRACE_TLB_HH__

#include "cpu/o3/dyn_inst.hh"
#include "cpu/o3/impl.hh"
#include "params/SimpleTraceTLB.hh"
#include "sim/probe/probe.hh"
#include "sim/probe/tlb.hh"

class SimpleTraceTLB : public ProbeListenerObject {

  public:
    typedef typename std::tuple<Addr, Addr, unsigned> TLBAddrArgs;

    SimpleTraceTLB(const SimpleTraceTLBParams *params):
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
    void traceRefills(const TLBAddrArgs &args);
    void traceHit(const TLBAddrArgs &args);
    void traceMiss(const TLBAddrArgs &args);

};
#endif//__CPU_O3_PROBE_SIMPLE_TRACE_TLB_HH__
