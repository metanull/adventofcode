#include "opcode.h"

size_t mem::operand(char o) const {
    switch(o) {
        case 6: return c;
        case 5: return b;
        case 4: return a;
        default: return (size_t)o;
    }
}
void mem::operation(char op, char o) {
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

memr op(mem & m, char op, char o) {
    memr mr;
    mr.old = m;
    mr.res = m;
    mr.op = op;
    mr.o = o;

    switch(op) {
        case 0: mr.res.adv(o);  break;
        case 1: mr.res.bxl(o);  break;
        case 2: mr.res.bst(o);  break;
        case 3: mr.res.jnz(o);  break;
        case 4: mr.res.bxc(o);  break;
        case 5: mr.res.out(o);  break;
        case 6: mr.res.bdv(o);  break;
        case 7: mr.res.cdv(o);  break;
    }
    return mr;
}
std::ostream & operator<<(std::ostream & os, const memr& r) {
    bool dumpo = true;
    switch(r.op) {
        case 0: std::cout << "ADV {A = A  >> ";  break;
        case 1: std::cout << "ADV {B = B XOR ";  break;
        case 2: std::cout << "ADV {B = B  &  7}";  
            dumpo = false;
            break;
        case 3: std::cout << (r.res.a == 0 ? "NOOP //" : "JUMP ") << r.o;  
            dumpo = false;
            break;
        case 4: std::cout << "BXC {B = B  ^  C}";  
            dumpo = false;
            break;
        case 5: std::cout << "OUT {<<  B  &  7}";
            dumpo = false;
            break;
        case 6: std::cout << "BDV {A = B >> ";  break;
        case 7: std::cout << "CDV {A = C >> ";  break;
    }
    if(dumpo) {
        switch(r.o) {
            case 6: 
                os << "[C ("<<std::setw(8)<<std::right<<r.old.operand(r.o)<<")]";
                break;
            case 5: 
                os << "[B ("<<std::setw(8)<<std::right<<r.old.operand(r.o)<<")]";
                break;
            case 4: 
                os << "[A ("<<std::setw(8)<<std::right<<r.old.operand(r.o)<<")]";
                break;
            default: 
                os << std::setw(8)<<std::right<<r.old.operand(r.o);
                break;
        }
        os << "}";
    }
    return os;
}
