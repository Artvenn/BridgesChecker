#include "../include/Convert.h"
#include <iostream>
#include "Str.h"

namespace ml {
    i32 Convert::str_to_i32(const ml::Str& str) {
        i32 sum = 0;
        char current;

        for (i32 i = 0; i < str.len(); i++) {
            current = str.to_c_str()[i];
            if (current < '0' || current > '9') {
                std::cerr << "cant convert: " << str << " to i32" << std::endl;
                exit(-1);
            }

            sum =  sum * 10 +  current - '0';
        }
        return sum;
    }
};