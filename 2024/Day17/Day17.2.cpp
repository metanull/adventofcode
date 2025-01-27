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

    // Initializes the Opcodes
    std::map<std::string, size_t> registers = {{"A",0},{"B",0},{"C",0},{"P",0}};
    using Op = std::function<void(char)>;
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
    
    Op ADV = [&registers,operand](char op) -> void {
        std::cerr << "ADV: [A] = [A] + OP => " 
            << registers["A"] << " + OP(" << (op == 6 ? "[C]" : (op == 5 ? "[B]" : (op == 4 ? "[A]" : std::string(1,'0'+op)))) << ") => " 
            << registers["A"] << " + " << operand(op) << " => " 
            << (registers["A"] + operand(op)) << std::endl;
        registers["A"] = registers["A"] / (size_t)pow((long double)2, (long double)operand(op));
        registers["P"] += 2;
    };
    Op BXL = [&registers,operand](char op) -> void {
        std::cerr << "BXL: [B] = [B] XOR OP => " 
            << registers["B"] << " XOR OP(" << (op == 6 ? "[C]" : (op == 5 ? "[B]" : (op == 4 ? "[A]" : std::string(1,'0'+op)))) << ") => " 
            << registers["B"] << " XOR " << operand(op) << " => " 
            << (registers["B"] ^ operand(op)) << std::endl;
        registers["B"] = registers["B"] ^ operand(op);
        registers["P"] += 2;
    };
    Op BST = [&registers,operand](char op) -> void {
        std::cerr << "BST: [B] = OP % 8 => " 
            << registers["B"] << " = OP(" << (op == 6 ? "[C]" : (op == 5 ? "[B]" : (op == 4 ? "[A]" : std::string(1,'0'+op)))) << ") % 8 => " 
            << registers["B"] << " = " << operand(op) << " % 8 => " 
            << (operand(op) % 8) << std::endl;
        registers["B"] = operand(op) % 8;
        registers["P"] += 2;
    };
    Op JNZ = [&registers,operand](char op) -> void {
        std::cerr << "JNZ: IF [A] != 0 THEN [P] = OP => " 
            << "IF " << registers["A"] << " != 0 THEN " << registers["P"] << " = OP(" << (op == 6 ? "[C]" : (op == 5 ? "[B]" : (op == 4 ? "[A]" : std::string(1,'0'+op)))) << ") => " 
            << "IF " << registers["A"] << " != 0 THEN " << registers["P"] << " = " << operand(op) << std::endl;
        if(registers["A"] != 0) {
            std::cerr << "[P] = OP => " << registers["P"] << " = " << operand(op) << std::endl;
            registers["P"] = op;
        } else {
            std::cerr << "[P] += 2 => " << registers["P"] << " += 2" << std::endl;
            registers["P"] += 2;
        }
    };
    Op BXC = [&registers,operand](char /* unused */) -> void {
        std::cerr << "BXC: [B] = [B] XOR [C] => " 
            << registers["B"] << " XOR " << registers["C"] << " => " 
            << (registers["B"] ^ registers["C"]) << std::endl;
        registers["B"] = registers["B"] ^ registers["C"];
        registers["P"] += 2;
    };
    Op OUT = [&registers,operand](char op) -> void {
        std::cerr << "OUT: OUTPUT OP % 8 => " 
            << "OUTPUT OP(" << (op == 6 ? "[C]" : (op == 5 ? "[B]" : (op == 4 ? "[A]" : std::string(1,'0'+op)))) << ") % 8 => " 
            << "OUTPUT " << operand(op) << " % 8 => " 
            << (operand(op) % 8) << std::endl;
        std::cout << "\033[32m" << (operand(op) % 8) << "\033[0m" << std::endl;
        registers["P"] += 2;
    };
    Op BDV = [&registers,operand](char op) -> void {
        std::cerr << "BDV: [B] = [A] / 2^OP => " 
            << registers["B"] << " = [A] / 2^OP(" << (op == 6 ? "[C]" : (op == 5 ? "[B]" : (op == 4 ? "[A]" : std::string(1,'0'+op)))) << ") => " 
            << registers["B"] << " = " << registers["A"] << " / 2^" << operand(op) << " => " 
            << registers["A"] / (size_t)pow((long double)2, (long double)operand(op)) << std::endl;
        registers["B"] = registers["A"] / (size_t)pow((long double)2, (long double)operand(op));
        registers["P"] += 2;
    };
    Op CDV = [&registers,operand](char op) -> void {
        std::cerr << "CDV: [C] = [A] / 2^OP => " 
            << registers["C"] << " = [A] / 2^OP(" << (op == 6 ? "[C]" : (op == 5 ? "[B]" : (op == 4 ? "[A]" : std::string(1,'0'+op)))) << ") => " 
            << registers["C"] << " = " << registers["A"] << " / 2^" << operand(op) << " => " 
            << registers["A"] / (size_t)pow((long double)2, (long double)operand(op)) << std::endl;
        registers["C"] = registers["A"] / (size_t)pow((long double)2, (long double)operand(op));
        registers["P"] += 2;
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

    // Print the state at start
    std::cerr << "Pointer  P: " << registers["P"] << std::endl;
    std::cerr << "Register A: " << registers["A"] << std::endl;
    std::cerr << "Register B: " << registers["B"] << std::endl;
    std::cerr << "Register C: " << registers["C"] << std::endl;
    std::cerr << "Instructions: ";
    for(auto i : instructions) {
        std::cout << (char)('0'+i) << ",";
    }
    std::cout << std::endl;

    // Run the "program"
    std::cout << "Result: ";
    while(registers["P"] < instructions.size()) {
        char op = (char)*(instructions.begin() + registers["P"]);
        char operand = (char)*(instructions.begin() + registers["P"] + 1);
        opcode[op](operand);
    }
    std::cout << std::endl;
    
    
    return 0;
}
