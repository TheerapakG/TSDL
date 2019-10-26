#ifndef TSDL_FEATURES_
#define TSDL_FEATURES_

#if __has_include(<concept>)
#include <concepts>
#endif

#ifndef __cpp_concepts
    #define TSDL_NO_CONCEPTS
#else
    #ifndef __cpp_lib_concepts
        #define TSDL_NO_LIB_CONCEPTS
    #endif
#endif

#endif