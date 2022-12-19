#pragma once
#include "./TypeAliases.h"

namespace ml {
    template <typename T> class Arr;

    class Str {
        char* head;
        u32 length;
        u32 strlen(const char* str) const;
        friend std::ostream& operator<<(std::ostream& out, const Str& str);
        friend std::istream& operator>>(std::istream& in, Str& str);
        friend Str exec(const Str& cmd);
        friend Str exec(const char cmd);
        friend class Sys;
        friend class File;
        template<typename Str> friend class Arr;
        enum Case {UPPER, LOWER};
        const char* change_case(const char* str, Str::Case case_type) const;
    public:
        Str();
        Str(const char* str);
        Str(const Str& other);
        Str(const Str&& other);

        u32 len() const;
        Str& operator=(const Str& other);
        Str operator+(const Str& other) const;
        bool operator==(const Str& other) const;
        bool operator!=(const Str& other) const;
        bool starts_with(const Str& str) const;
        bool starts_with(const char* str) const;
        Arr<Str> split(const char separator) const;
        Arr<Str> split(const Str& separator) const;
        Str trim();
        Str to_upper() const;
        Str to_lower() const;
        const char* to_c_str() const;
        ~Str();
    };
};