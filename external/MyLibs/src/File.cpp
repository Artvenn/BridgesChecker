#include <iostream>
#include "../include/File.h"
#include "../include/Str.h"


namespace ml {
    File::File(const Str& file_path) {
        filepath = file_path;
    }
    
    Str File::read() {
        i_stream = std::ifstream(filepath.head);
        if (!i_stream.is_open()) {
            std::cerr << "Error\n Cant open file for reading: " << filepath << std::endl;
            exit(-1);
        }
        Str result;
        i_stream >> result;
        i_stream.close();
        return result;
    }

    void File::write(const Str& data) {
        o_stream = std::ofstream(filepath.head);
        if (!o_stream.is_open()) {
            std::cerr << "Error\n Cant open file for writing: " << filepath << std::endl;
            exit(-1);
        }
        Str result;
        o_stream << data;
        o_stream.close();
    }

    File::~File() {
        if (i_stream.is_open()) i_stream.close();
        if (o_stream.is_open()) o_stream.close();
    }
}