#include <iostream>
#include <memory.h>
#include "../include/TypeAliases.h"
#include "../include/Str.h"
#include "../include/Arr.h"

namespace ml {
    u32 Str::strlen(const char *str) const {
        i32 i;
        for (i = 0; str[i] != '\0'; i++);
        return i;
    }

    Str::Str() {
        length = 0;
        head = nullptr;
    }

    Str::Str(const char *str) {
        length = strlen(str);
        head = new char[length + 1];
        memcpy(head, str, length);
        head[length] = '\0';
    }

    Str::Str(const Str &other) {
        head = new char[other.length + 1];
        memcpy(head, other.head, other.length);
        length = other.length;
        head[length] = '\0';
    }

    Str::Str(const Str &&other) {
        head = new char[other.length + 1];
        memcpy(head, other.head, other.length);
        length = other.length;
        head[length] = '\0';
    }

    Str &Str::operator=(const Str &other) {
        if (head != nullptr)
            delete[] head;
        head = new char[other.length + 1];
        memcpy(head, other.head, other.length);
        length = other.length;
        head[length] = '\0';
        return *this;
    }

    u32 Str::len() const { return length; }

    Str Str::operator+(const Str &other) const {
        u32 len_of_two = length + other.length;
        char *temp_p = new char[len_of_two + 1];
        memcpy(temp_p, head, length);
        memcpy(temp_p + length, other.head, other.length);
        temp_p[len_of_two] = '\0';
        return Str(temp_p);
    }

    bool Str::operator==(const Str& other) const {
        if (length != other.length) return false;
        for (u32 i = 0; i < length; i++)
            if (head[i] != other.head[i]) return false;

        return true;
    }

    bool Str::operator!=(const Str& other) const {
        return !operator==(other);
    }

    bool Str::starts_with(const Str& str) const {
        return starts_with(str.head);
    }

    bool Str::starts_with(const char* str) const {
        for (u32 i = 0; i < strlen(str); i++) {
            if (head[i] != str[i]) return false;
        }
        return true;
    }

    Arr<Str> Str::split(const char separator) const {
        Arr<Str> strs;
        u32 start = 0;
        u32 current = 0;
        char* temp_p = nullptr;

        for (; current < length; current++) {
            if (head[current] == separator) {
                temp_p = new char[current-start+1];
                for (u32 i = start; i < current; i++)
                    temp_p[i-start] = head[i];

                temp_p[current-start] = '\0';
                strs.push(Str(temp_p).trim());
                delete[] temp_p;
                start = current + 1;
                current = start;
            }
        }

        temp_p = new char[length-start+1];
        u32 i;
        for (i = start; i <= current; i++)
            temp_p[i-start] = head[i];

        temp_p[(length)-start] = '\0';
        strs.push(Str(temp_p).trim());
        delete[] temp_p;
        return strs;
    }

    Arr<Str> Str::split(const Str& separator) const {
        Arr<Str> strs;
        char* temp_p = nullptr;
        u32 wind_search_start = 0;

        for (u32 i, wind_pos = 0; wind_pos < length; wind_pos++) {
            for (i = 0; i < separator.len(); i++)
                if (head[wind_pos+i] != separator.head[i]) break;

            if (i != separator.len()) continue;

            temp_p = new char[wind_pos-wind_search_start+1];
            memcpy(temp_p, head+wind_search_start, wind_pos-wind_search_start);
            temp_p[wind_pos-wind_search_start] = '\0';
            strs.push(Str(temp_p).trim());
            delete[] temp_p;
            wind_search_start = wind_pos + separator.len();
            wind_pos = wind_search_start;
        }

        temp_p = new char[length-wind_search_start+1];
        u32 i;
        for (i = 0; i < length-wind_search_start; i++)
            temp_p[i] = head[wind_search_start+i];

        temp_p[i] = '\0';
        strs.push(Str(temp_p).trim());
        delete[] temp_p;
        return strs;
    }

    bool _is_escape_char(const char ch) {
        return  ((ch >= 0) && (ch <= 32));
    }

    Str Str::trim() {
        i32 start;
        i32 end;
        char* temp_p;

        for (start = 0; _is_escape_char(head[start]); ++start);
        for (end = length-1; _is_escape_char(head[end]); --end);
        i32 new_len = end - start + 1;
        temp_p = new char[new_len+1];
        memcpy(temp_p, head+start, new_len);
        temp_p[new_len] = '\0';
        auto trimed = Str(temp_p);
        delete[] temp_p;
        return trimed;
    }

    const char* Str::change_case(const char* str, Str::Case case_type) const {
        char* temp_p = new char[length+1];
        for (u32 i = 0; i < length; i++) {
            temp_p[i] = (
                (head[i] >= 'a' && head[i] <= 'z')
                || (head[i] >= 'A' && head[i] <= 'Z')
            )
            ? head[i] - ((case_type == Case::LOWER) ? -32 : 32)
            : head[i];
        }
        temp_p[length] = '\0';
        return temp_p;
    }

    Str Str::to_upper() const {
        return std::move(Str(change_case(head, Case::UPPER)));
    }
    
    Str Str::to_lower() const {
        return std::move(Str(change_case(head, Case::LOWER)));
    }

    std::ostream& operator<<(std::ostream& out, const Str& str) {
        return out << str.head;
    }

    std::istream& operator>>(std::istream& in, Str& str) {
        const u16 buf_size = 256;
        char buf[buf_size] {};
        delete[] str.head;
        str.head = nullptr;
        str.length = 0;
        i32 t_ch = ' ';

        while (t_ch != EOF) {
            u32 i = 0;
            for (i = 0; i < buf_size; i++) {
                t_ch = in.get();
                if (t_ch == EOF) break;
                buf[i] = t_ch;
            }

            if (str.length > 0) {
                char* temp_p = new char[str.length+i];
                memcpy(temp_p, str.head, str.length);
                memcpy(temp_p+str.length, buf, i);
                temp_p[str.length+i] = '\0';
                delete[] str.head;
                str.head = temp_p;
                str.length += i;
            } else {
                str.head = new char[i];
                memcpy(str.head, buf, i);
                str.head[i] = '\0';
                str.length = i;
            }
        }
        return in;
    }

    const char* Str::to_c_str() const {
        return head;        
    }

    Str::~Str()
    {
        delete[] head;
    }
};