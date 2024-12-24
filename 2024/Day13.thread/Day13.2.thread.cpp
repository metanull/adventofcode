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
#include <thread>
#include <queue>
#include <condition_variable>
// #include <compare>

//#include "..\include\AdventOfCode.h"
//#include "..\include\Point.h"

using namespace std;

const char * banner = "AdventOfCode 2024 Day 13 (thread)!";
const char * inputFilePath = "C:/Users/phave/OneDrive/Documents/adventofcode/2024/Input/Day13.txt";
const int nThreads = 1;

struct XY {
    uint64_t x;
    uint64_t y;

    std::string toString() const {
        std::stringstream ss;
        ss << "(X: " << x << " Y: " << y << ")";
        return ss.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const XY& xy);
};
struct Machine {
    XY Prize;
    XY A;
    XY B;
    bool test(uint64_t M, uint64_t N) {
        return (M * A.x + N * B.x == Prize.x) && (M * A.y + N * B.y == Prize.y);
    }
    static uint64_t cost(uint64_t M, uint64_t N) {
        return M * 3 + N * 1;
    }
    std::string toString() const {
        std::stringstream ss;
        ss << "Machine {Prize: " << Prize << " A: " << A << " B: " << B << "}";
        return ss.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const Machine& m);
};

std::queue<Machine> Machines;
std::mutex MachinesMutex;
std::condition_variable MachinesCV;
std::mutex TotalCostMutex;
bool done = false;
uint64_t TotalCost = 0;
uint64_t HardLimit = 0;

inline int64_t gcd(int64_t a, int64_t b) { 
    while (b != 0) { 
        int64_t t = b; 
        b = a % b; 
        a = t; 
    }
    return a;
}

// Function to implement the Extended Euclidean Algorithm
int64_t extendedEuclidean(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t gcd = extendedEuclidean(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

// Function to find all pairs (A, B) that satisfy both equations
// A*X1 + B*Y1 = C1 and A*X2 + B*Y2 = C2
// Function to find the cheapest pair (A, B) that satisfies both equations
std::pair<int64_t, int64_t> findCheapestSolution(int64_t x1, int64_t y1, int64_t C1, int64_t x2, int64_t y2, int64_t C2) {
    std::pair<int64_t, int64_t> cheapestSolution;
    int64_t minCost = std::numeric_limits<int64_t>::max();

    // Coefficients for the linear combination
    int64_t a1, b1, a2, b2;
    int64_t gcd1 = extendedEuclidean(x1, y1, a1, b1);
    int64_t gcd2 = extendedEuclidean(x2, y2, a2, b2);

    // Check if solutions exist
    if (C1 % gcd1 != 0 || C2 % gcd2 != 0) {
        return cheapestSolution;
    }

    // Adjust the coefficients for the given C1 and C2
    a1 *= (C1 / gcd1);
    b1 *= (C1 / gcd1);
    a2 *= (C2 / gcd2);
    b2 *= (C2 / gcd2);

    // Find the cheapest pair (A, B) using the coefficients
    for (int64_t k1 = 0; ; ++k1) { 
        for (int64_t k2 = 0; ; ++k2) { 
            int64_t A = a1 + k1 * (y1 / gcd1);
            int64_t B = b2 + k2 * (x2 / gcd2);
            if (A >= 0 && B >= 0 && (A * x1 + B * y1 == C1) && (A * x2 + B * y2 == C2)) {
                int64_t cost = 3 * A + 1 * B;
                if (cost < minCost) {
                    minCost = cost;
                    cheapestSolution = {A, B};
                } else {
                    // If the current cost is higher than the minimum cost found, break the inner loop
                    break;
                }
            }
        }
        // If the current cost is higher than the minimum cost found, break the outer loop
        if (3 * (a1 + k1 * (y1 / gcd1)) >= minCost) {
            break;
        }
    }

    return cheapestSolution;
}

uint64_t SolveMachine2(const Machine & machine) {
    std::pair<int, int> cheapestSolution = findCheapestSolution(machine.A.x, machine.B.x, machine.Prize.x, machine.A.y, machine.B.y, machine.Prize.y); 
    if (cheapestSolution.first == 0 && cheapestSolution.second == 0) { 
        std::stringstream ss;
        ss << "No solutions exist for the given equations." << std::endl; 
        std::cout << ss.str();
        return 0;
    }
    std::stringstream ss;
    ss << "Cheapest solution (A, B) is: (" << cheapestSolution.first << ", " << cheapestSolution.second << ")" << std::endl; std::cout << "Cost: " << 3 * cheapestSolution.first + 1 * cheapestSolution.second << "€" << std::endl; 
    std::cout << ss.str();
    return Machine::cost(cheapestSolution.first,cheapestSolution.second);
}

uint64_t SolveMachine(const Machine & machine) {
    uint64_t SolutionCost = 0;

    auto AMaxX = (machine.Prize.x / machine.A.x) + 1;
    auto BMaxX = (machine.Prize.x / machine.B.x) + 1;
    auto AMaxY = (machine.Prize.y / machine.A.y) + 1;
    auto BMaxY = (machine.Prize.y / machine.B.y) + 1;

    auto MaxX = std::max(AMaxX,BMaxX);
    auto MaxY = std::max(AMaxY,BMaxY);

    auto Max = std::max(MaxX,MaxY);
    if(HardLimit > 0) {
        Max = std::min(Max, HardLimit);
    }

    // Given Prize = M*A + N*B
    // Try all possible values of M and N between 0 and Prize.x/A.x and Prize.x/B.x
    // ... half because we are solving from both ends at once
    for(uint64_t M = 0; M < Max && (SolutionCost == 0 || M <= SolutionCost) ; M++ ) {
        if(
            ((machine.Prize.x - (machine.A.x * M)) % machine.B.x == 0) &&
            ((machine.Prize.y - (machine.A.y * M)) % machine.B.y == 0)
        ) {
            auto A = M;
            auto B = (machine.Prize.x - (machine.A.x * M)) / machine.B.x;
            auto machineCost = Machine::cost(A,B);
            if(SolutionCost == 0 || machineCost < SolutionCost) {
                SolutionCost = machineCost;
/*
                std::stringstream ss;
                ss << machine << " - Solution: { A: " << A << " B: " << B << " Cost: " << SolutionCost << "}" << std::endl;
                std::cout << ss.str();
*/
            }
        }
        if(
            ((machine.Prize.x - (machine.B.x * M)) % machine.A.x == 0) &&
            ((machine.Prize.y - (machine.B.y * M)) % machine.A.y == 0)
        ) {
            auto B = M;
            auto A = (machine.Prize.x - (machine.B.x * M)) / machine.A.x;
            auto machineCost = Machine::cost(A,B);
            if(SolutionCost == 0 || machineCost < SolutionCost) {
                SolutionCost = machineCost;
/*
                std::stringstream ss;
                ss << machine << " - Solution: { A: " << A << " B: " << B << " Cost: " << SolutionCost << "}" << std::endl;
                std::cout << ss.str();
*/
            }
        }
    }

    if(SolutionCost != 0) {
        std::stringstream ss;
        ss << machine << " - Cost: " << SolutionCost << "}" << std::endl;
        std::cout << ss.str();
    }
    return SolutionCost;
}

void worker(int id) {
    while(true) {
        Machine machine;
        {
            std::unique_lock<std::mutex> lock(MachinesMutex);
            MachinesCV.wait(lock, []() { return !Machines.empty() || done; });

            if (done && Machines.empty()) {
                return;
            }

            machine = Machines.front();
            Machines.pop();
            lock.unlock();

            auto solution = SolveMachine(machine);
            { 
                std::lock_guard<std::mutex> TotalCostLock(TotalCostMutex); 
                TotalCost += solution; 
            }
            std::stringstream ss;
            ss << "Worker " << id << ": " << machine << " - " << solution << std::endl;
            std::cout << ss.str();

            lock.lock();
            MachinesCV.notify_all();
        }
    }
}


int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl;

    // Read input file
    std::ifstream inputFile(inputFilePath);
    if(!inputFile || !inputFile.is_open()) {
        std::cerr << "Unable to open " << inputFilePath << "." << std::endl;
        return 1;
    }

    // Launch N Workers
    std::vector<std::thread> threads;
    threads.reserve(nThreads);
    for(int i = 0; i < nThreads; i++) {
        threads.push_back(std::thread(worker, i));
    }

    // Read/Enqueue Machines
    std::string line;
    while(getline(inputFile, line)) {
        std::regex rx("^(.*): X[=\\+](\\d+), Y[=\\+](\\d+)$");
        std::smatch match;

        Machine machine;
        std::regex_search(line, match, rx);
        machine.A.x = std::stoi(match[2]);
        machine.A.y = std::stoi(match[3]);

        getline(inputFile, line);
        std::regex_search(line, match, rx);
        machine.B.x = std::stoi(match[2]);
        machine.B.y = std::stoi(match[3]);

        getline(inputFile, line);
        std::regex_search(line, match, rx);
        machine.Prize.x = std::stoi(match[2]);
        machine.Prize.y = std::stoi(match[3]);

        // 13.2 Add 10^12 to Prize        
        // machine.Prize.x += 10000000000000;
        // machine.Prize.y += 10000000000000;
        

        std::unique_lock<std::mutex> lock(MachinesMutex);
        Machines.push(machine);
        MachinesCV.notify_one();
        
        getline(inputFile, line);
    }
    inputFile.close();

    // Signal workers to finish
    {
        std::unique_lock<std::mutex> lock(MachinesMutex);
        done = true;
        MachinesCV.notify_all();
    }

    // Wait for workers to finish
    for(auto & thread : threads) {
        thread.join();
    }
    
    std::cout << "Total cost: " << TotalCost << std::endl;

    return 0;
}

std::ostream& operator<<(std::ostream& os, const XY& xy) {
    os << xy.toString();
    return os;
}
std::ostream& operator<<(std::ostream& os, const Machine& xy) {
    os << xy.toString();
    return os;
}

