/*-
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 *
 */

#ifndef __CPU_MINOR_PROBE_SIMPLE_TRACE_HH__
#define __CPU_MINOR_PROBE_SIMPLE_TRACE_HH__

#include "cpu/simple_thread.hh"
#include "cpu/static_inst.hh"
#include "params/SimpleTraceSimple.hh"
#include "sim/probe/probe.hh"

class SimpleTraceSimple : public ProbeListenerObject {

  public:
    SimpleTraceSimple(const SimpleTraceSimpleParams *params):
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
    void traceCyclesSimple(const std::pair<SimpleThread*, StaticInstPtr>&);
#endif
    void traceRetiredInstsSimple(
        const std::pair<SimpleThread*, StaticInstPtr>&);
    void traceRetiredLoadsSimple(
        const std::pair<SimpleThread*, StaticInstPtr>&);
    void traceRetiredStoresSimple(
        const std::pair<SimpleThread*, StaticInstPtr>&);
    void traceRetiredBranchesSimple(
        const std::pair<SimpleThread*, StaticInstPtr>&);
    void traceMispredictedBranches(
        const std::pair<SimpleThread*, StaticInstPtr>&);

};
#endif//__CPU_MINOR_PROBE_SIMPLE_TRACE_HH__
