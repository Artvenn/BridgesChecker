#pragma once
#include "./Str.h"

namespace ml {
    class Sys {
    public:
        static Str exec(const char* cmd);
        static Str exec(const Str& cmd);
    };
};