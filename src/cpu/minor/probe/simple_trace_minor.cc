/*-
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 *
 */

#include "cpu/minor/probe/simple_trace_minor.hh"

#include "base/trace.hh"
#include "debug/SimpleTrace.hh"

#if 0
void SimpleTraceMinor::traceCyclesMinor(const Minor::MinorDynInstPtr &inst)
{

#if 1
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredInsts 0x%08x "
        "[sNs: %llu %llu %llu %llu %llu] %s.\n",
        curTick(), name(),
        inst->instAddr(),
        inst->id.streamSeqNum, inst->id.predictionSeqNum, inst->id.lineSeqNum,
        inst->id.fetchSeqNum, inst->id.execSeqNum,
        inst->staticInst->disassemble(inst->instAddr()));
#else
    DPRINTFR(SimpleTrace, "%7d: %s : Cycles : %s\n",
        curTick(), name(), typeid(inst).name());
#endif
}
#endif

void SimpleTraceMinor::traceRetiredInstsMinor(
    const Minor::MinorDynInstPtr &inst)
{

#if 1
    //const auto *inst =
    //    reinterpret_cast<const Minor::MinorDynInst *>(cinst.get());
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredInsts 0x%08x "
        "[sNs: %llu %llu %llu %llu %llu] %s.\n",
        curTick(), name(),
        inst->instAddr(),
        inst->id.streamSeqNum, inst->id.predictionSeqNum, inst->id.lineSeqNum,
        inst->id.fetchSeqNum, inst->id.execSeqNum,
        inst->staticInst->disassemble(inst->instAddr()));
#else
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredInsts %s\n",
        curTick(), name(), typeid(inst).name());
#endif
}

void SimpleTraceMinor::traceRetiredLoadsMinor(
    const Minor::MinorDynInstPtr &inst)
{

#if 1
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredLoads 0x%08x "
        "[sNs: %llu %llu %llu %llu %llu] %s.\n",
        curTick(), name(),
        inst->instAddr(),
        inst->id.streamSeqNum, inst->id.predictionSeqNum, inst->id.lineSeqNum,
        inst->id.fetchSeqNum, inst->id.execSeqNum,
        inst->staticInst->disassemble(inst->instAddr()));
#else
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredLoads %s\n",
        curTick(), name(), typeid(inst).name());
#endif
}

void SimpleTraceMinor::traceRetiredStoresMinor(
    const Minor::MinorDynInstPtr &inst)
{

#if 1
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredStores 0x%08x "
        "[sNs: %llu %llu %llu %llu %llu] %s.\n",
        curTick(), name(),
        inst->instAddr(),
        inst->id.streamSeqNum, inst->id.predictionSeqNum, inst->id.lineSeqNum,
        inst->id.fetchSeqNum, inst->id.execSeqNum,
        inst->staticInst->disassemble(inst->instAddr()));
#else
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredStores %s\n",
        curTick(), name(), typeid(inst).name());
#endif
}

void SimpleTraceMinor::traceRetiredBranchesMinor(
    const Minor::MinorDynInstPtr &inst)
{

#if 1
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredBranches 0x%08x "
        "[sNs: %llu %llu %llu %llu %llu] %s.\n",
        curTick(), name(),
        inst->instAddr(),
        inst->id.streamSeqNum, inst->id.predictionSeqNum, inst->id.lineSeqNum,
        inst->id.fetchSeqNum, inst->id.execSeqNum,
        inst->staticInst->disassemble(inst->instAddr()));
#else
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredBranches %s\n",
        curTick(), name(), typeid(inst).name());
#endif
}

void SimpleTraceMinor::traceBranchMispredicted(
    const Minor::MinorDynInstPtr &inst)
{

#if 1
    DPRINTFR(SimpleTrace, "%7d: %s : BranchMispredicted 0x%08x "
        "[sNs: %llu %llu %llu %llu %llu] %s.\n",
        curTick(), name(),
        inst->instAddr(),
        inst->id.streamSeqNum, inst->id.predictionSeqNum, inst->id.lineSeqNum,
        inst->id.fetchSeqNum, inst->id.execSeqNum,
        inst->staticInst->disassemble(inst->instAddr()));
#else
    DPRINTFR(SimpleTrace, "%7d: %s : BranchMispredicted %s\n",
        curTick(), name(), typeid(inst).name());
#endif
}

void SimpleTraceMinor::regProbeListeners()
{
    DPRINTF(SimpleTrace, "XXX-BZ We went through "
        "SimpleTraceMinor::regProbeListener()...\n");

    typedef ProbeListenerArg<SimpleTraceMinor, Minor::MinorDynInstPtr>
        staticInstListener;
#if 0
    listeners.push_back(new staticInstListener(this,
        "CPUMinorCycles", &SimpleTraceMinor::traceCyclesMinor));
#endif
    listeners.push_back(new staticInstListener(this,
        "CPUMinorRetiredInsts", &SimpleTraceMinor::traceRetiredInstsMinor));
    listeners.push_back(new staticInstListener(this,
        "CPUMinorRetiredLoads", &SimpleTraceMinor::traceRetiredLoadsMinor));
    listeners.push_back(new staticInstListener(this,
        "CPUMinorRetiredStores", &SimpleTraceMinor::traceRetiredStoresMinor));
    listeners.push_back(new staticInstListener(this,
         "CPUMinorRetiredBranches",
         &SimpleTraceMinor::traceRetiredBranchesMinor));
    listeners.push_back(new staticInstListener(this,
        "CPUMinorBranchMispredict",
        &SimpleTraceMinor::traceBranchMispredicted));
}

SimpleTraceMinor*
SimpleTraceMinorParams::create()
{
    return new SimpleTraceMinor(this);
}
