/*
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 */

#include "cpu/o3/probe/simple_trace_tlb.hh"

#include "base/trace.hh"
#include "cpu/inst_seq.hh"      // for InstSeqNum
#include "cpu/static_inst.hh"
#include "debug/SimpleTrace.hh"

void SimpleTraceTLB::traceRefills(const TLBAddrArgs &args)
{
    DPRINTFR(SimpleTrace, "%7d: %s : Refills vaddr %#018x pc %#018x "
        "size %#18x\t%s\n", curTick(), name(),
        std::get<0>(args), std::get<1>(args), std::get<2>(args), "<not yet>");
}

void SimpleTraceTLB::traceHit(const TLBAddrArgs &args)
{
    DPRINTFR(SimpleTrace, "%7d: %s : Hit vaddr %#018x pc %#018x "
        "size %#018x\t%s\n", curTick(), name(),
        std::get<0>(args), std::get<1>(args), std::get<2>(args), "<not yet>");
}

void SimpleTraceTLB::traceMiss(const TLBAddrArgs &args)
{
    DPRINTFR(SimpleTrace, "%7d: %s : Miss vaddr %#018x pc %#018x\t%s\n",
        curTick(), name(), std::get<0>(args), std::get<1>(args), "<not yet>");
}

void SimpleTraceTLB::regProbeListeners()
{
    DPRINTF(SimpleTrace, "XXX-BZ We went through this SimpleTraceTLB ....\n");
    typedef ProbeListenerArg<SimpleTraceTLB, TLBAddrArgs> AddrListener;
    listeners.push_back(new AddrListener(this,
        "TLBRefills", &SimpleTraceTLB::traceRefills));
    listeners.push_back(new AddrListener(this,
        "TLBHit", &SimpleTraceTLB::traceHit));
    listeners.push_back(new AddrListener(this,
        "TLBMiss", &SimpleTraceTLB::traceMiss));
}

SimpleTraceTLB*
SimpleTraceTLBParams::create()
{
    return new SimpleTraceTLB(this);
}
