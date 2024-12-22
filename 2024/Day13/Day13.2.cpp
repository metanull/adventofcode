#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <map>
#include <stdexcept>
#include <filesystem>
#include <regex>
// #include <compare>

#include "..\include\AdventOfCode.h"
#include "..\include\Point.h"

using namespace std;

const char * banner = "AdventOfCode 2024 Day 13!";
const char * inputFilePath = "C:/Users/phave/OneDrive/Documents/adventofcode/2024/Input/Day13.txt";

bool PointInList(const std::vector<Point> & v, const Point & t) {
    return std::find(v.begin(), v.end(), t) != v.end();
}

struct Machine {
    Point Prize;
    Point A;
    Point B;
};
struct Solution {
    uint64_t a;
    uint64_t b;
    uint64_t cost() const {
        return (a * 3) + (b * 1);
    }
};
bool cmpCost(const Solution & a, const Solution & b) {
    return a.cost() < b.cost();
}

template <typename T>
std::vector<T> factorize(T num) {   // works great for smooth numbers
    vector<T> v;
    if (num < 4) { v.push_back(num); return v; }
    T d {2};
    while (num >= d * d) {
        while (num % d == 0) {  // remove all repeats of this divisor
            v.push_back(d);
            num /= d;
        }
        ++d;
        if (d * d > num && num > 1) { v.push_back(num); return v; }
    }
    return v;
}

int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl;

    // Read input file
    std::ifstream inputFile(inputFilePath);
    if(!inputFile || !inputFile.is_open()) {
        std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
        return 1;
    }

    std::vector<Machine> Machines;
    std::string line;
    while(getline(inputFile, line)) {
        std::regex rx("^(.*): X[=\\+](\\d+), Y[=\\+](\\d+)$");
        std::smatch match;

        Machine machine;
        std::regex_search(line, match, rx);
        machine.A = Point(std::stoi(match[2]), std::stoi(match[3]));

        getline(inputFile, line);
        std::regex_search(line, match, rx);
        machine.B = Point(std::stoi(match[2]), std::stoi(match[3]));

        getline(inputFile, line);
        std::regex_search(line, match, rx);
        machine.Prize = Point(std::stoi(match[2]), std::stoi(match[3]));

        Machines.push_back(machine);
        getline(inputFile, line);
    }
    inputFile.close();

    // Process all machines
    uint64_t totalCost = 0;
    for(auto & machine : Machines) {
        std::cout << "Machine A: " << machine.A << std::endl;
        std::cout << "Machine B: " << machine.B << std::endl;
        std::cout << "Prize: " << machine.Prize << std::endl;
        std::cout << std::endl;

        uint64_t TargetX = (uint64_t)machine.Prize.x + 10000000000000;
        uint64_t TargetY = (uint64_t)machine.Prize.y + 10000000000000;
        uint64_t AX = (uint64_t)machine.A.x;
        uint64_t AY = (uint64_t)machine.A.y;
        uint64_t BX = (uint64_t)machine.B.x;
        uint64_t BY = (uint64_t)machine.B.y;

        //auto factors = factorize<uint64_t>(TargetX);
        //factors = factorize<uint64_t>(TargetY);

        std::vector<Solution> solutions;
        for(uint64_t i = 0; i < max((TargetX/AX),(TargetX/BX)); i++) {
            if((AX * i > TargetX || BX * i > TargetX) && (AY * i > TargetY || BY * i > TargetY)) {
                break;
            }
            if((TargetX - (AX * i)) % BX == 0) {
                uint64_t j = (TargetX - (AX * i)) / BX;
                if((TargetY - (AY * i)) % BY == 0 && (TargetY - (AY * i)) / BY == j) {
                    Solution s = {i, j};
                    std::cout << '.';
                    /*std::cout << "Found solution: " << s.a << " " << s.b << " cost: " << s.cost() << std::endl;
                    std::cout << "  Prize: " << machine.Prize << std::endl;
                    std::cout << "  Machine A: " << machine.A << std::endl;
                    std::cout << "  Machine B: " << machine.B << std::endl;
                    std::cout << std::endl;*/
                    solutions.push_back(s);
                }
            }
        }
        if(solutions.size() == 0) {
            std::cout << "No solution found." << std::endl;
        } else {
            auto min_cost = *std::min_element(solutions.begin(),solutions.end(),cmpCost);
            std::cout << "Minimum cost: " << min_cost.cost() << std::endl;
            totalCost += min_cost.cost();
        }
    }

    std::cout << "Total cost: " << totalCost << std::endl;

    return 0;
}
