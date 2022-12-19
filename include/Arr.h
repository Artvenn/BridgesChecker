#pragma once

#include <memory.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include "./TypeAliases.h"
#include "./Str.h"

namespace ml {

    template <typename T>
    class Arr {
        T* head;
        T* virt_head;
        u32 actual_len;
        u32 current_len;
        const u16 AMORT_LEN = 16;
        const u16 AMORT_MID = AMORT_LEN / 2;

        friend std::ostream& operator<<(std::ostream& out, Arr<T>& arr) {
            out << "[";
            for (u32 i = 0; i < arr.len(); i++) 
                out << arr[i] << ((i == arr.len()-1) ? "" : "; ");

            out << "]";
            return out;
        }
    public:
        Arr() {
            head = nullptr;
            virt_head = nullptr;
            actual_len = 0;
            current_len = 0;
        }

        u32 len() const {return current_len;}

        void push(const T& el) {
            if ((i32)actual_len-(i32)AMORT_MID-(i32)current_len > 0) {
                virt_head[current_len] = el;
                current_len++;
                return;
            }
            T* temp_p = new T[current_len+AMORT_LEN];
            for (u32 i = 0; i < len(); i++)
                temp_p[AMORT_MID+i] = virt_head[i];

            temp_p[current_len+AMORT_MID] = el;
            delete[] head;
            head = temp_p;
            virt_head = head + AMORT_MID;
            actual_len = current_len + AMORT_LEN;
            current_len++;
        }

        const T& pop() {
            if (current_len == 0) {
                std::cerr << "calling pop() on empty array!" << std::endl;
                exit(-1);
            }
            current_len--;
            return virt_head[current_len];
        }

        //TODO: check correctness
        void shift(const T& el) {
            if (current_len == 0) {
                push(el);
                current_len++;
                return;
            }
            if ((virt_head - head) / sizeof(T) >= 1) {
                virt_head[-1] = el;
                virt_head--;
                current_len++;
                return;
            }

            T* temp_p = new T[current_len + AMORT_LEN];
            for (u32 i = 0; i < current_len*sizeof(T); i++)
                temp_p[AMORT_MID] = virt_head[i];

            virt_head = temp_p + AMORT_MID - 1;
            *virt_head = el;
            delete[] head;
            head = temp_p;
            current_len++;
            actual_len++;
        }

        const T& unshift() {
            if (current_len == 0) {
                std::cerr << "calling unshift() on empty array!" << std::endl;
                exit(-1);
            }
            if (current_len == 1) return pop();
            current_len--;
            virt_head++;
            return virt_head[-1];
        }

        void insert(const u32& index, const T& el) {
            if (index == 0) {
                shift(el);
                return;
            } 
            if (index == current_len) {
                push(el);
                return;
            }

            if ((actual_len - current_len) != 0) {
                --virt_head;
                for (i32 i = 0; i < index; i++)
                    virt_head[i] = virt_head[i+1];
                
                virt_head[index] = el;
                ++current_len;
                return;
            }

            if ((actual_len - AMORT_MID - current_len) != 0) {
                for (i32 i = current_len; i > index; --i)
                    virt_head[i] = virt_head[i-1];

                virt_head[index] = el;
                ++current_len;
                return;
            }

            T* temp_p = new T[current_len+AMORT_LEN];
            for (i32 i = current_len; i > index; --i) 
                temp_p[AMORT_MID+i] = virt_head[i-1];

            temp_p[index+AMORT_MID] = el;
            for (u32 i = 0; i < index; i++) 
                temp_p[AMORT_MID+i] = virt_head[i];

            delete[] head;
            head = temp_p;
            virt_head = head;
            actual_len += AMORT_LEN;
            ++current_len;
        }

        T& operator[](const u32& index) {
            if (index >= current_len) {
                printf("reading out of range: [%u]\n", index);
                exit(-1);
            }
            return virt_head[index];
        }

        template <typename Lam>
        i32 first_where(Lam&& lam) const {
            for (i32 i = 0; i < static_cast<i32>(current_len); i++)
                if (lam(virt_head[i])) return i;

            return -1;
        }

        template <typename Lam>
        void for_each(Lam&& lam) const {
            for (u32 i = 0; i < current_len; i++)
                lam(virt_head[i]);
        }
        
        template <typename Lam>
        Arr<T> filter(Lam&& lam) const {
            Arr<T> new_arr;
            for (i32 i = 0; i < static_cast<i32>(current_len); i++)
                if (lam(virt_head[i])) new_arr.push(virt_head[i]);
    
            return new_arr;
        }

        Str join(const char separator) const {
            u32 all_str_len = 0;

            for_each([&all_str_len] (Str el) {
                u32 len = el.len();
                all_str_len += (len + 1);
            });

            char* const new_str = new char[all_str_len + 1];
            char* offset_p = new_str;

            for_each([&offset_p, &separator] (auto el) {
                memcpy(offset_p, el.head, el.len());
                offset_p[el.len()] = separator;
                offset_p += el.len() + 1;
            });

            new_str[all_str_len-1] = '\0';
            return Str(new_str);
        }

        void shuffle() {
            srand(time(nullptr));

            for (u32 i = 0; i < current_len; i++) {
                if (current_len < 2) {
                    std::cerr << "Error: shuffling array lenght less 2" << std::endl;
                    exit(-1);
                }
                u32 rand_num;
                rand_num = rand() % current_len;

                T temp = virt_head[rand_num];
                virt_head[rand_num] = virt_head[i];
                virt_head[i] = temp;
            }
        }

        ~Arr() {
            delete [] head;
        }
    };
};