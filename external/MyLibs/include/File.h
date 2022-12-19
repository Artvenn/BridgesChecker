#pragma once

#include <fstream>
#include "Str.h"

namespace ml {
    class File {
        Str filepath;
        std::ifstream i_stream;
        std::ofstream o_stream;
    public:
        File(const Str& str);
        Str read();
        void write(const Str& data);
        ~File();
    };
}