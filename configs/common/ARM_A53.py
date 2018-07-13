
from m5.objects import *

# https://www.arm.com/files/pdf/Juno_ARM_Development_Platform_datasheet.pdf
# http://www.arm.com/products/processors/cortex-a/cortex-a53-processor.php
# https://en.wikipedia.org/wiki/ARM_Cortex-A53

# Instruction Cache
class ARM_A53_ICache(Cache):
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
class ARM_A53_DCache(Cache):
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
class ARM_A53_L2(Cache):
    tag_latency = 12
    data_latency = 12
    response_latency = 12
    mshrs = 16
    tgts_per_mshr = 8
    size = '1MB'			# OK
    assoc = 16
    write_buffers = 8
    prefetch_on_access = True
    clusivity = 'mostly_excl'
    # Simple stride prefetcher
    prefetcher = StridePrefetcher(degree=8, latency = 1)
    tags = RandomRepl()

# L3 Cache, NONE
class ARM_A53_L3(Cache):
    pass

# TLB Cache, NONE
class ARM_A53_iTLBL2(Cache):
    tag_latency = 4
    data_latency = 4
    response_latency = 4
    mshrs = 6
    tgts_per_mshr = 8
    size = '1kB'			# XXX 512 entries, hmmm, unused
    assoc = 4				# OK
    write_buffers = 16
    is_read_only = True
    # Writeback clean lines as well
    writeback_clean = True

# end
