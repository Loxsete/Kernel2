#pragma once

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
    typedef unsigned char bool;
    #define true  1
    #define false 0
#else
    #ifndef true
        #define true  ((bool)1)
    #endif
    #ifndef false
        #define false ((bool)0)
    #endif
#endif
