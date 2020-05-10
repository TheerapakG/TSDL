//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

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