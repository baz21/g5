
from m5.objects import *

# http://www.7-cpu.com/cpu/IvyBridge.html (not what we use)

# Ivy Bridge-EP E5-2643 v2  Id=0x306e4  Family=0x6  Model=0x3e  Stepping=4
#  --l1d_size=32kB	8way
#  --l1i_size=32kB	8way
#  --l2_size=256kB	8way
#  --l3_size=25mB	20way
#  --cacheline_size=64bytes	(src/sim/System.py default OK)
#
# TLB sizes?

# Instruction Cache
class Intel_E5_2643_v2_O3_ICache(Cache):
    tag_latency = 2			# OKish, to make it work
    data_latency = 2			# OKish, to make it work
    response_latency = 2		# OKish, to make it work
    mshrs = 6
    tgts_per_mshr = 20
    size = '32kB'			# OK
    assoc = 8				# OK
    is_read_only = True
    # Writeback clean lines as well
    writeback_clean = True

# Data Cache
class Intel_E5_2643_v2_O3_DCache(Cache):
    tag_latency = 2			# OKish, to make it work
    data_latency = 2			# OKish, to make it work
    response_latency = 2		# OKish, to make it work
    mshrs = 6
    tgts_per_mshr = 8
    size = '32kB'			# OK
    assoc = 8				# OK
    write_buffers = 16
    # Consider the L2 a victim cache also for clean lines
    writeback_clean = True

# L2 Cache
class Intel_E5_2643_v2_O3_L2(Cache):
    tag_latency = 2			# OKish, to make it work
    data_latency = 2			# OKish, to make it work
    response_latency = 2		# OKish, to make it work
    mshrs = 16
    tgts_per_mshr = 12
    size = '256kB'			# OK
    assoc = 8				# OK
    write_buffers = 8
    #prefetch_on_access = True
    #clusivity = 'mostly_excl'
    # Simple stride prefetcher
    #prefetcher = StridePrefetcher(degree=8, latency = 1)
    #tags = RandomRepl()

# L3 Cache
class Intel_E5_2643_v2_O3_L3(Cache):
    tag_latency = 30			# OKish, set to 0 elsewhere
    data_latency = 30			# OKish, set to 0 elsewhere
    response_latency = 30		# OKish, set to 0 elsewhere
    mshrs = 512
    tgts_per_mshr = 20
    size = '20MB'			# 25MB but gem5 enforces a
                                        # ^2 for size / (blocksize * assoc)
                                        # thus make it only 20MB
    assoc = 20				# OK
    write_buffers = 256
    #prefetch_on_access = True
    #clusivity = 'mostly_excl'
    # Simple stride prefetcher
    #prefetcher = StridePrefetcher(degree=8, latency = 1)
    #tags = RandomRepl()

# TLB Cache
# Use a cache as a L2 TLB;  DO NOT USE;  IT IS COMPLICATED
# We do use .addPrivateSplitL1Caches() to add Gem5 defaults.
class Intel_E5_2643_v2_O3_iTLBL2(Cache):
    tag_latency = 4
    data_latency = 4
    response_latency = 4
    mshrs = 6
    tgts_per_mshr = 8
    size = '1kB'			# XXX 1024 entries, hmmm, unused
    assoc = 4				# OK
    write_buffers = 16
    is_read_only = True
    # Writeback clean lines as well
    writeback_clean = True

# end
