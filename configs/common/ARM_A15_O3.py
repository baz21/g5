
from m5.objects import *

# https://en.wikipedia.org/wiki/ARM_Cortex-A15

# Instruction Cache
class ARM_A15_O3_ICache(Cache):
    tag_latency = 1
    data_latency = 1
    response_latency = 1
    mshrs = 2
    tgts_per_mshr = 8
    size = '32kB'			# OK
    assoc = 2
    is_read_only = True
    # Writeback clean lines as well
    writeback_clean = True

# Data Cache
class ARM_A15_O3_DCache(Cache):
    tag_latency = 2
    data_latency = 2
    response_latency = 2
    mshrs = 6
    tgts_per_mshr = 8
    size = '32kB'			# OK
    assoc = 2
    write_buffers = 16
    # Consider the L2 a victim cache also for clean lines
    writeback_clean = True

# L2 Cache
class ARM_A15_O3_L2(Cache):
    tag_latency = 12
    data_latency = 12
    response_latency = 12
    mshrs = 16
    tgts_per_mshr = 8
    size = '4MB'			# up to 4MB
    assoc = 16
    write_buffers = 8
    prefetch_on_access = True
    clusivity = 'mostly_excl'
    # Simple stride prefetcher
    prefetcher = StridePrefetcher(degree=8, latency = 1)
    tags = RandomRepl()

# L3 Cache, NONE
class ARM_A15_O3_L3(Cache):
    pass

# TLB Cache, NONE
class ARM_A15_O3_iTLBL2(Cache):
    pass

# end
