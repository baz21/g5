/*
 * Copyright (c) 2013 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Matt Horsnell
 */

#include "cpu/o3/probe/simple_trace.hh"

#include "base/trace.hh"
#include "debug/SimpleTrace.hh"

void SimpleTrace::traceFetch(const O3CPUImpl::DynInstPtr &dynInst)
{

    DPRINTFR(SimpleTrace, "%7d: %s : Fetch 0x%08x [sN: %llu] %s.\n",
             curTick(), name(),
             dynInst->instAddr(),
             dynInst->seqNum,
             dynInst->staticInst->disassemble(dynInst->instAddr()));
}

void SimpleTrace::traceMispredict(const O3CPUImpl::DynInstPtr &dynInst)
{

    DPRINTFR(SimpleTrace, "%7d: %s : Mispredict 0x%08x [sN: %llu] %s.\n",
             curTick(), name(),
             dynInst->instAddr(),
             dynInst->seqNum,
             dynInst->staticInst->disassemble(dynInst->instAddr()));
}

void SimpleTrace::traceCommit(const O3CPUImpl::DynInstPtr &dynInst)
{

    DPRINTFR(SimpleTrace, "%7d: %s : Commit 0x%08x [sN: %llu] %s.\n",
             curTick(), name(),
             dynInst->instAddr(),
             dynInst->seqNum,
             dynInst->staticInst->disassemble(dynInst->instAddr()));
}

#if 0
void SimpleTrace::traceCyclesO3(const O3CPUImpl::DynInstPtr &inst)
{
    DPRINTFR(SimpleTrace, "%7d: %s : RetiredInsts 0x%08x [sN: %llu] %s.\n",
             curTick(), name(),
             inst->instAddr(),
             inst->seqNum,
             inst->staticInst->disassemble(inst->instAddr()));
}
#endif

void SimpleTrace::traceRetiredInstsO3(const O3CPUImpl::DynInstPtr &inst)
{

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredInsts 0x%08x [sN: %llu] %s.\n",
             curTick(), name(),
             inst->instAddr(),
             inst->seqNum,
             inst->staticInst->disassemble(inst->instAddr()));
}

void SimpleTrace::traceRetiredLoadsO3(const O3CPUImpl::DynInstPtr &inst)
{

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredLoads 0x%08x [sN: %llu] %s.\n",
             curTick(), name(),
             inst->instAddr(),
             inst->seqNum,
             inst->staticInst->disassemble(inst->instAddr()));
}

void SimpleTrace::traceRetiredStoresO3(const O3CPUImpl::DynInstPtr &inst)
{

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredStores 0x%08x [sN: %llu] %s.\n",
             curTick(), name(),
             inst->instAddr(),
             inst->seqNum,
             inst->staticInst->disassemble(inst->instAddr()));
}

void SimpleTrace::traceRetiredBranchesO3(const O3CPUImpl::DynInstPtr &inst)
{

    DPRINTFR(SimpleTrace, "%7d: %s : RetiredBranches 0x%08x [sN: %llu] %s.\n",
             curTick(), name(),
             inst->instAddr(),
             inst->seqNum,
             inst->staticInst->disassemble(inst->instAddr()));
}

void SimpleTrace::regProbeListeners()
{
    DPRINTF(SimpleTrace, "XXX-BZ We went through "
        "SimpleTrace::regProbeListener()...\n");

    typedef ProbeListenerArg<SimpleTrace, O3CPUImpl::DynInstPtr>
        DynInstListener;
    listeners.push_back(new DynInstListener(this,
        "Fetch", &SimpleTrace::traceFetch));
    listeners.push_back(new DynInstListener(this,
        "Mispredict", &SimpleTrace::traceMispredict));
    listeners.push_back(new DynInstListener(this,
        "Commit", &SimpleTrace::traceCommit));

#if 0
    listeners.push_back(new DynInstListener(this,
        "CPUO3Cycles", &SimpleTrace::traceCyclesO3));
#endif
    listeners.push_back(new DynInstListener(this,
        "CPUO3RetiredInsts", &SimpleTrace::traceRetiredInstsO3));
    listeners.push_back(new DynInstListener(this,
        "CPUO3RetiredLoads", &SimpleTrace::traceRetiredLoadsO3));
    listeners.push_back(new DynInstListener(this,
        "CPUO3RetiredStores", &SimpleTrace::traceRetiredStoresO3));
    listeners.push_back(new DynInstListener(this,
        "CPUO3RetiredBranches", &SimpleTrace::traceRetiredBranchesO3));
}

SimpleTrace*
SimpleTraceParams::create()
{
    return new SimpleTrace(this);
}
