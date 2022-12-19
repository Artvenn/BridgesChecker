#include <iostream>
#include <stdio.h>
#include "../include/Str.h"
#include "../include/System.h"

namespace ml {
    Str Sys::exec(const char* cmd) {
        Str result;
        char buf[1024];
        FILE* pipe = popen(cmd, "r");
        if (!pipe) {
            std::cerr << "Error of executing: " << cmd << std::endl;
            exit(-1);
        }
        while (fgets(buf, sizeof buf, pipe) != NULL)
            result = result + buf;
            
        pclose(pipe);

        return result;
    }
    
    Str Sys::exec(const Str& cmd) {
        return Sys::exec(cmd.head);
    }
}