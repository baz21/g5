/*
 * Copyright (c) 2016 Bjoern A. Zeeb
 * All rights reserved
 */

#include "cpu/o3/probe/simple_trace_cache.hh"

#include "base/trace.hh"
#include "cpu/inst_seq.hh"      // for InstSeqNum
#include "cpu/static_inst.hh"
#include "debug/SimpleTrace.hh"

void SimpleTraceCACHE::traceHit(const CACHEAddrArgs &args)
{

    DPRINTFR(SimpleTrace, "%7d: %s : Hit vaddr %#018x pc %#018x size %#018x "
        "cmdString %s\t%s\n", curTick(), name(),
        std::get<0>(args), std::get<1>(args), std::get<2>(args),
        std::get<3>(args), "<not yet>");
}

void SimpleTraceCACHE::traceMiss(const CACHEAddrArgs &args)
{
    DPRINTFR(SimpleTrace, "%7d: %s : Miss vaddr %#018x pc %#018x "
        "cmdString %s\t%s\n", curTick(), name(),
        std::get<0>(args), std::get<1>(args), std::get<3>(args), "<not yet>");
}

void SimpleTraceCACHE::regProbeListeners()
{

    DPRINTF(SimpleTrace, "XXX-BZ We went through this SimpleTraceCACHE...\n");
    typedef ProbeListenerArg<SimpleTraceCACHE, CACHEAddrArgs> AddrListener;
    listeners.push_back(new AddrListener(this,
        "CACHEHit", &SimpleTraceCACHE::traceHit));
    listeners.push_back(new AddrListener(this,
        "CACHEMiss", &SimpleTraceCACHE::traceMiss));
}

SimpleTraceCACHE*
SimpleTraceCACHEParams::create()
{
    return new SimpleTraceCACHE(this);
}
