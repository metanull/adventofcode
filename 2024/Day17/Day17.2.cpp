#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stack>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <chrono>
#include <set>
#include <map>
#include <functional>
#include <cmath>
#include <bitset>

#include "Constant.h"

// ---------------------------------------------------------
const char *banner = "AdventOfCode 2024 Day 17!";
const char *inputFilePath = INPUT_PATH;


int main(int argc, char **argv, char **envp)
{
    std::vector<std::string> args(argv, argv + argc);
    if (args.size() == 1)
    {
        // No input file provided, use the default one
        args.push_back(inputFilePath);
    }
    size_t banner_width = 80;
    std::cout << "\033[7;34;1m" << std::endl
              << std::setfill('#') << std::setw(banner_width) << '#' << std::endl
              << std::setw((banner_width - strlen(banner)) / 2) << '#'
              << banner
              << std::setw(((banner_width - strlen(banner)) / 2) + ((banner_width - strlen(banner)) % 2)) << '#' << std::endl
              << std::setfill('#') << std::setw(banner_width) << '#' << std::endl
              << "\033[0m" << std::endl
              << std::endl;
    /*
    // Initializes the Opcodes
    std::map<std::string, size_t> registers = {{"A",0},{"B",0},{"C",0},{"P",0}};
    using Op = std::function<std::string(char)>;
    using Oper = std::function<size_t(char)>;

    Oper operand = [&registers](char op) -> size_t {
        switch(op) {
            case 0: return 0;
            case 1: return 1;
            case 2: return 2;
            case 3: return 3;
            case 4: return registers["A"];
            case 5: return registers["B"];
            case 6: return registers["C"];
        }
        throw std::runtime_error("Invalid operand");
    };
    
    Op ADV = [&registers,operand](char op) -> std::string {
        registers["A"] = registers["A"] / (size_t)pow((long double)2, (long double)operand(op));
        registers["P"] += 2;

        std::stringstream ss;
        ss << "ADV => [A] = [A] / Pow(2, ";
        switch(op) {
            case 6: ss << "[C ::= " << operand(op) << "]"; break;
            case 5: ss << "[B ::= " << operand(op) << "]"; break;
            case 4: ss << "[A ::= " << operand(op) << "]"; break;
            default: ss << (int)(op); break;
        }
        ss << ")" << std::endl << " => \033[33m[A] = " << registers["A"] << "\033[0m";
        return ss.str();
    };

    Op BXL = [&registers,operand](char op) -> std::string {
        registers["B"] = registers["B"] ^ operand(op);
        registers["P"] += 2;

        std::stringstream ss;
        ss << "BXL => [B] = [B] XOR ";
        switch(op) {
            case 6: ss << "[C ::= " << operand(op) << "]"; break;
            case 5: ss << "[B ::= " << operand(op) << "]"; break;
            case 4: ss << "[A ::= " << operand(op) << "]"; break;
            default: ss << (int)(op); break;
        }
        ss << "" << std::endl << " => \033[33m[B] = " << registers["B"] << "\033[0m";
        return ss.str();
    };

    Op BST = [&registers,operand](char op) -> std::string {
        registers["B"] = operand(op) % 8;
        registers["P"] += 2;

        std::stringstream ss;
        ss << "BST => [B] = (";
        switch(op) {
            case 6: ss << "[C ::= " << operand(op) << "]"; break;
            case 5: ss << "[B ::= " << operand(op) << "]"; break;
            case 4: ss << "[A ::= " << operand(op) << "]"; break;
            default: ss << (int)(op); break;
        }
        ss << " % 8 )" << std::endl << " => \033[33m[B] = " << (operand(op) % 8) << "\033[0m";
        return ss.str();
    };

    Op JNZ = [&registers,operand](char op) -> std::string {
        if(registers["A"] != 0) {
            registers["P"] = op;
        } else {
            registers["P"] += 2;
        }

        std::stringstream ss;
        ss << "JNZ => [P] = ([A] == 0 ? ([P] + 2) : " << (int)op << ")";
        ss << "" << std::endl << " => \033[33m [P] = " << registers["P"] << "\033[0m";
        return ss.str();
    };

    Op BXC = [&registers,operand](char) -> std::string {
        registers["B"] = registers["B"] ^ registers["C"];
        registers["P"] += 2;

        std::stringstream ss;
        ss << "BXC => [B] = [B] XOR [C]";
        ss << "" << std::endl << " => \033[33m[B] = " << registers["B"] << "\033[0m";
        return ss.str();
    };

    Op OUT = [&registers,operand](char op) -> std::string {
        std::cout << "\033[7;32m" << std::setw(1) << (operand(op) % 8) << "\033[0m" << std::endl;
        registers["P"] += 2;

        std::stringstream ss;
        ss << "OUT => (";
        switch(op) {
            case 6: ss << "[C ::= " << operand(op) << "]"; break;
            case 5: ss << "[B ::= " << operand(op) << "]"; break;
            case 4: ss << "[A ::= " << operand(op) << "]"; break;
            default: ss << (int)(op); break;
        }
        ss << " % 8 )" << std::endl << " => \033[33;7m[B] = " << (operand(op) % 8) << "\033[0m";
        return ss.str();
    };

    Op BDV = [&registers,operand](char op) -> std::string {
        registers["B"] = registers["A"] / (size_t)pow((long double)2, (long double)operand(op));
        registers["P"] += 2;

        std::stringstream ss;
        ss << "BDV => [B] = [A] / Pow(2, ";
        switch(op) {
            case 6: ss << "[C ::= " << operand(op) << "]"; break;
            case 5: ss << "[B ::= " << operand(op) << "]"; break;
            case 4: ss << "[A ::= " << operand(op) << "]"; break;
            default: ss << (int)(op); break;
        }
        ss << ")" << std::endl << " => \033[33m[B] = " << registers["B"] << "\033[0m";
        return ss.str();
    };

    Op CDV = [&registers,operand](char op) -> std::string {
        registers["C"] = registers["A"] / (size_t)pow((long double)2, (long double)operand(op));
        registers["P"] += 2;

        std::stringstream ss;
        ss << "CDV => [C] = [A] / Pow(2, ";
        switch(op) {
            case 6: ss << "[C ::= " << operand(op) << "]"; break;
            case 5: ss << "[B ::= " << operand(op) << "]"; break;
            case 4: ss << "[A ::= " << operand(op) << "]"; break;
            default: ss << (int)(op); break;
        }
        ss << ")" << std::endl << " => \033[33m[C] = " << registers["A"] << "\033[0m";
        return ss.str();
    };

    std::map<char, Op> opcode = {
        {0, ADV},
        {1, BXL},
        {2, BST},
        {3, JNZ},
        {4, BXC},
        {5, OUT},
        {6, BDV},
        {7, CDV}
    };

    // Read the input file
    registers = {{"A",64751475},{"B",0},{"C",0},{"P",0}};
    std::vector<char> instructions = {2,4,1,2,7,5,4,5,1,3,5,5,0,3,3,0};

    // Run the "program"
    std::cerr << "Instructions: ";
    for(auto i : instructions) {
        std::cout << (char)('0'+i) << ",";
    }

    std::cout << "Result: ";
    while(registers["P"] < instructions.size()) {
        static size_t count = 1;
        char op = (char)*(instructions.begin() + registers["P"]);
        char operand = (char)*(instructions.begin() + registers["P"] + 1);

        std::cerr << std::endl << "\033[32m";
        std::cerr << "[OPER]: " << std::setw(1) << (int)op << "\t";
        std::cerr << "[OPND]: " << std::setw(1) << (int)operand << "\t";
        std::cerr << "[P]: " << std::setw(8) << registers["P"] << "\t";
        std::cerr << "[A]: " << std::setw(8) << registers["A"] << "\t";
        std::cerr << "[B]: " << std::setw(8) << registers["B"] << "\t";
        std::cerr << "[C]: " << std::setw(8) << registers["C"] << "\t";
        std::cerr << "\033[0m" << std::endl;
        
        std::cout << opcode[op](operand) << std::endl;

        std::cerr << "[P]: " << std::setw(8) << registers["P"] << std::endl;
    }
    std::cout << std::endl;
    */

    long t = 64751475;

    std::function <char(long &)> f = [&f](long &a) -> char {
        long b = 0;
        long c = 0;
        long r = 0;
        b = a % 8;
        b = b ^ 2;
        c = a / pow(2, b);
        b = b ^ c;
        b = b ^ 3;
        a = a / pow(2, 3);
        return b % 8;
    };
    std::function <void(long &)> f2 = [&f](long &a) -> void {
        std::cout << (((((a & 7) ^ 2) ^ (a >> ((a & 7) ^ 2))) ^3) & 7);
        a = a / (((((a & 7) ^ 2) ^ (a >> ((a & 7) ^ 2))) ^3) >>3);
    };
    std::function <char(long &)> g = [&f](long &a) -> char {
        long b = 0;
        long c = 0;
        long r = 0;

        std::cout << "\033[33;7m A ::= " << a << ".\033[0m" << std::endl;
        std::cout << " \033[7mB = A % 8 \033[0m" << std::endl;
        std::cout << " A: " << std::bitset<32>(a) << std::endl;
        std::cout << " 8: " << std::bitset<32>(8) << std::endl;
        std::cout << " => " << std::bitset<32>(a % 8) << std::endl;
        b = a % 8;
        std::cout << " \033[7mB = B XOR 2 \033[0m" << std::endl;
        std::cout << " B: " << std::bitset<32>(b) << std::endl;
        std::cout << " 2: " << std::bitset<32>(2) << std::endl;
        std::cout << " => " << std::bitset<32>(b ^ 2) << std::endl;
        b = b ^ 2;
        std::cout << " \033[7mC = A / POW(2,B) \033[0m" << std::endl;
        std::cout << " A: " << std::bitset<32>(a) << std::endl;
        std::cout << "^B: " << std::bitset<32>(pow(2, b)) << std::endl;
        std::cout << " => " << std::bitset<32>(a / pow(2, b)) << std::endl;
        c = a / pow(2, b);
        std::cout << " \033[7mB = B XOR C \033[0m" << std::endl;
        std::cout << " B: " << std::bitset<32>(b) << std::endl;
        std::cout << " C: " << std::bitset<32>(c) << std::endl;
        std::cout << " => " << std::bitset<32>(b ^ c) << std::endl;
        b = b ^ c;
        std::cout << " \033[7mB = B XOR 3 \033[0m" << std::endl;
        std::cout << " B: " << std::bitset<32>(b) << std::endl;
        std::cout << " 3: " << std::bitset<32>(3) << std::endl;
        std::cout << " => " << std::bitset<32>(b ^ 3) << std::endl;
        b = b ^ 3;
        std::cout << " \033[7mA = A / POW(2,3) \033[0m" << std::endl;
        std::cout << " A: " << std::bitset<32>(a) << std::endl;
        std::cout << "^3: " << std::bitset<32>(pow(2,3)) << std::endl;
        std::cout << " => " << std::bitset<32>(a / pow(2, 3)) << std::endl;
        a = a / pow(2, 3);
        std::cout << " \033[7mOUT = B % 8 \033[0m" << std::endl;
        std::cout << " B: " << std::bitset<32>(a) << std::endl;
        std::cout << "%8: " << std::bitset<32>(7) << std::endl;
        std::cout << " => " << std::bitset<32>(b % 8) << std::endl;
        return b % 8;
    };
    /*
    while(t != 0) {
        auto byte = g(t);
        std::bitset<8> binary(byte);
        std::cout << (char)('0' + byte) << " - " << binary << " - A: " << t << std::endl << std::endl;
    }
    */

    while(t != 0) {
        std::cout << (int)f(t) << std::endl;
    }   


    std::function <std::string(const long &)> h = [&f](const long &a) -> std::string {
        std::stringstream ss;
        long t = a;
        while(t != 0) {
            auto byte = f(t);
            ss << (int)byte;
        }
        return ss.str();
        
        /*long b = 0;
        long c = 0;
        long r = 0;
        b = a % 8;
        b = b ^ 2;
        c = a / pow(2, b);
        b = b ^ c;
        b = b ^ 3;
        a = a / pow(2, 3);
        return b % 8;*/
    };

    std::cout << h(64751475) << std::endl;
    std::cout << std::endl;

    for(long long i = (1 << 24); i < LLONG_MAX; i++) {
        if(h(i) == "21741503") {
            std::cout << "SOLUTION: " << i << std::endl;
            break;
        } else {
            // std::cout << "\033[2J\033[1;1H" << i << std::endl;
            if(i % 100000 == 0) {
                std::cout << "\033[1F" << i << std::endl;
            }
        }
    }
    

/*
    std::vector<int> v = {2,4,1,2,7,5,4,5,1,3,5,5,0,3,3,0};
    std::vector<int> reversed_v;
    for (auto it = v.rbegin(); it != v.rend(); it += 2) {
        reversed_v.push_back(*(it+1));
    }
*/
    return 0;
}
