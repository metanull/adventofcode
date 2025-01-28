#ifndef __OPCODE_2024_DAY_17_H__
#define __OPCODE_2024_DAY_17_H__

#include <functional>
#include <bitset>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>

struct mem {
    uint64_t a = 0;
    uint64_t b = 0;
    uint64_t c = 0;
    uint64_t d = 0;
    uint64_t ptr = 0;
    char r[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    uint64_t rptr = 0;

    mem() = default;
    mem(const mem &) = default;
    mem& operator=(const mem &) = default;

    uint64_t operand(char o) const {
        switch(o) {
            case 6: return c;
            case 5: return b;
            case 4: return a;
            default: return (uint64_t)o;
        }
    }
    void operation(char op, char o) {
        switch(op) {
            case 0: adv(o);  break;
            case 1: bxl(o);  break;
            case 2: bst(o);  break;
            case 3: jnz(o);  break;
            case 4: bxc(o);  break;
            case 5: out(o);  break;
            case 6: bdv(o);  break;
            case 7: cdv(o);  break;
        }
    }
    inline void adv(char o) {
        a = a >> operand(o);
        ptr += 2;
    }
    inline void bxl(char o) {
        b = b ^ operand(o);
        ptr += 2;
    }
    inline void bst(char o) {
        b = operand(o) & 7;
        ptr += 2;
    }
    inline void jnz(char o) {
        if(a != 0) {
            ptr = o;
        } else {
            ptr += 2;
        }
    }
    inline void bxc(char o) {
        b = b ^ c;
        ptr += 2;
    }
    inline void out(char o) {
        r[rptr ++] = (char)(operand(o) & 7);
        ptr += 2;
    }
    inline void bdv(char o) {
        b = a >>operand(o);
        ptr += 2;
    }
    inline void cdv(char o) {
        c = a >>operand(o);
        ptr += 2;
    }
};

#endif // __OPCODE_2024_DAY_17_H__