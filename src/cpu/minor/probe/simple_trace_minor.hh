/*-
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 *
 */

#ifndef __CPU_MINOR_PROBE_SIMPLE_TRACE_HH__
#define __CPU_MINOR_PROBE_SIMPLE_TRACE_HH__

#include "cpu/minor/dyn_inst.hh"
#include "params/SimpleTraceMinor.hh"
#include "sim/probe/probe.hh"

class SimpleTraceMinor : public ProbeListenerObject {

  public:
    SimpleTraceMinor(const SimpleTraceMinorParams *params):
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
#if 0
    void traceCyclesMinor(const Minor::MinorDynInstPtr &inst);
#endif
    void traceRetiredInstsMinor(const Minor::MinorDynInstPtr &inst);
    void traceRetiredLoadsMinor(const Minor::MinorDynInstPtr &inst);
    void traceRetiredStoresMinor(const Minor::MinorDynInstPtr &inst);
    void traceRetiredBranchesMinor(const Minor::MinorDynInstPtr &inst);
    void traceBranchMispredicted(const Minor::MinorDynInstPtr &inst);

};
#endif//__CPU_MINOR_PROBE_SIMPLE_TRACE_HH__
