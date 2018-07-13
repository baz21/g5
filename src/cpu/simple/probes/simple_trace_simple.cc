/*-
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 *
 */

#include "cpu/simple/probes/simple_trace_simple.hh"

#include "base/trace.hh"
#include "debug/SimpleTrace.hh"

#if 0
void SimpleTraceSimple::traceCyclesSimple(
    const std::pair<SimpleThread*, StaticInstPtr>& p)
{
    SimpleThread* thread = p.first;
    const StaticInstPtr &inst = p.second;

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredInsts 0x%08x %s.\n",
             curTick(), name(),
             thread->instAddr(),
             inst->disassemble(thread->instAddr()));
}
#endif

void SimpleTraceSimple::traceRetiredInstsSimple(
    const std::pair<SimpleThread*, StaticInstPtr>& p)
{
    SimpleThread* thread = p.first;
    const StaticInstPtr &inst = p.second;

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredInsts 0x%08x %s.\n",
             curTick(), name(),
             thread->instAddr(),
             inst->disassemble(thread->instAddr()));
}

void SimpleTraceSimple::traceRetiredLoadsSimple(
    const std::pair<SimpleThread*, StaticInstPtr>& p)
{
    SimpleThread* thread = p.first;
    const StaticInstPtr &inst = p.second;

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredLoads 0x%08x %s.\n",
             curTick(), name(),
             thread->instAddr(),
             inst->disassemble(thread->instAddr()));
}

void SimpleTraceSimple::traceRetiredStoresSimple(
    const std::pair<SimpleThread*, StaticInstPtr>& p)
{
    SimpleThread* thread = p.first;
    const StaticInstPtr &inst = p.second;

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredStores 0x%08x %s.\n",
             curTick(), name(),
             thread->instAddr(),
             inst->disassemble(thread->instAddr()));
}

void SimpleTraceSimple::traceRetiredBranchesSimple(
    const std::pair<SimpleThread*, StaticInstPtr>& p)
{
    SimpleThread* thread = p.first;
    const StaticInstPtr &inst = p.second;

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredBranches 0x%08x %s.\n",
             curTick(), name(),
             thread->instAddr(),
             inst->disassemble(thread->instAddr()));
}

void SimpleTraceSimple::traceMispredictedBranches(
    const std::pair<SimpleThread*, StaticInstPtr>& p)
{
    SimpleThread* thread = p.first;
    const StaticInstPtr &inst = p.second;

    DPRINTFR(SimpleTrace, "%7d: %s : MispredictedBranches 0x%08x %s.\n",
             curTick(), name(),
             thread->instAddr(),
             inst->disassemble(thread->instAddr()));
}

void SimpleTraceSimple::regProbeListeners()
{
    DPRINTF(SimpleTrace, "XXX-BZ We went through "
        "SimpleTraceSimple::regProbeListener()...\n");

    typedef ProbeListenerArg<SimpleTraceSimple,
        std::pair<SimpleThread*,StaticInstPtr>> staticInstListener;
#if 0
    listeners.push_back(new staticInstListener(this,
        "CPUSimpleCycles",
        &SimpleTraceSimple::traceCyclesSimple));
#endif
    listeners.push_back(new staticInstListener(this,
        "CPUSimpleRetiredInsts",
        &SimpleTraceSimple::traceRetiredInstsSimple));
    listeners.push_back(new staticInstListener(this,
        "CPUSimpleRetiredLoads",
        &SimpleTraceSimple::traceRetiredLoadsSimple));
    listeners.push_back(new staticInstListener(this,
        "CPUSimpleRetiredStores",
        &SimpleTraceSimple::traceRetiredStoresSimple));
    listeners.push_back(new staticInstListener(this,
        "CPUSimpleRetiredBranches",
        &SimpleTraceSimple::traceRetiredBranchesSimple));
    listeners.push_back(new staticInstListener(this,
        "CPUSimpleBranchMispredict",
        &SimpleTraceSimple::traceMispredictedBranches));
}

SimpleTraceSimple*
SimpleTraceSimpleParams::create()
{
    return new SimpleTraceSimple(this);
}
