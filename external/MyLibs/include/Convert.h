#pragma once
#include "TypeAliases.h"
#include "Str.h"

namespace ml {
    class Convert {
    public:
        static i32 str_to_i32(const ml::Str& str);
    };
};