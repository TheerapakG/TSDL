#!/usr/bin/env sh
g++ -o ./out/test_$1_$2 -std=c++20 -fcoroutines -I ./include -I ~/libs/vcpkg/installed/x64-linux/include/ -DTGFX_ENABLE_SPDLOG -DTGFX_ENABLE_BOOST_STACKTRACE -DBOOST_STACKTRACE_LINK ./tests/test_$1_$2.cpp -L ~/libs/vcpkg/installed/x64-linux/lib/ -lboost_stacktrace_basic -lfmt -ldl && ./out/test_$1_$2
