#pragma once

#if (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__) == 4)
    #define CPU_32_BITS
#else
    #define CPU_64_BITS
#endif
