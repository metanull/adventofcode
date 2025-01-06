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
#include <numeric> // pour std::gcd
// #include <compare>

#include "..\AdventOfCode.h"
#include "..\Point.h"
#include "..\Vector2D.h"

using namespace std;

const char * banner = "AdventOfCode 2024 Day 14!";
const char * inputFilePath = "C:/Users/phave/OneDrive/Documents/adventofcode/2024/Input/Day14.txt";

/*
bool PointInList(const std::vector<Point> & v, const Point & t) {
    return std::find(v.begin(), v.end(), t) != v.end();
}
*/
void DumpPositions(const std::vector<std::pair<long, long>> & positions, long width, long height);

// Fonction pour trouver l'inverse multiplicatif de V modulo M
long long modInverse(long long V, long long M);

int main(int argc, char ** argv, char ** envp) {
    std::cout << banner << std::endl;
    //std::cout << "CWD: " << std::filesystem::current_path() << std::endl;   // Requires c++ 17 (enabled via CMake configuration)

    std::vector<std::pair<long, long>> positions;
    std::vector<std::pair<long, long>> velocities;

    // Read input file
    std::ifstream inputFile(inputFilePath);
    if(!inputFile || !inputFile.is_open()) {
        std::cerr << "Unable to open " << inputFilePath << "." /* << "CWD: " << std::filesystem::current_path()*/  << std::endl;
        return 1;
    }
    std::string line;
    while(getline(inputFile, line)) {
        std::istringstream iss(line);
        long px = 0, py = 0, vx = 0, vy = 0;
        iss.ignore(2);
        iss >> px;
        iss.ignore(1, ',');
        iss >> py;
        iss.ignore(3);
        iss >> vx;
        iss.ignore(1, ',');
        iss >> vy;
        positions.push_back(std::make_pair(px, py));
        velocities.push_back(std::make_pair(vx, vy));
    }
    inputFile.close();

    Point O = Point::origin();
    //Point Z = Point(11,7);
    Point Z = Point(101,103);

    // Dump the input
    /*
    for(size_t i = 0; i < positions.size(); i++) {
        std::cout << "pos=<" << positions[i].first << "," << positions[i].second << ">, vel=<" << velocities[i].first << "," << velocities[i].second << ">" << std::endl;
    }
    */

/*
    // Simulate N times
    long MaxN=100000000;
    long MinN=23000000;
    long MaxCenterColumn = 0;
    for(auto N = MinN; N < MaxN; N++) {
        long quadrants[4] = {0,0,0,0};
        long axis[2] = {0,0};
        std::vector<std::pair<long, long>> newPositions;
        for(size_t i = 0; i < positions.size(); i++) {
            auto x = positions[i].first + (N*velocities[i].first);
            auto y = positions[i].second + (N*velocities[i].second);

            if(std::abs(x) > std::abs(Z.x)) {
                x = x % Z.x;
            }
            if(std::abs(y) > std::abs(Z.y)) {
                y = y % Z.y;
            }
            if(x < 0) {
                x += Z.x;
            }
            if(y < 0) {
                y += Z.y;
            }

            newPositions.push_back(std::make_pair(x, y));
            // std::cout << "pos=<" << positions[i].first << "," << positions[i].second << ">" << "\t";
            // std::cout << "<" << x << "," << y << ">" << "\t";
            if(x != (Z.x -1) / 2 && y != (Z.y -1) / 2) {
                if(x < (Z.x -1) / 2 && y < (Z.y -1) / 2) {
                    // std::cout << "QUADRANT 1" << std::endl;
                    quadrants[0] ++;
                } else if(x > (Z.x -1) / 2 && y < (Z.y -1) / 2) {
                    // std::cout << "QUADRANT 2" << std::endl;
                    quadrants[1] ++;
                } else if(x < (Z.x -1) / 2 && y > (Z.y -1) / 2) {
                    // std::cout << "QUADRANT 3" << std::endl;
                    quadrants[2] ++;
                } else if(x > (Z.x -1) / 2 && y > (Z.y -1) / 2) {
                    // std::cout << "QUADRANT 4" << std::endl;
                    quadrants[3] ++;
                }
            } else {
                if(x == (Z.x -1) / 2 ) {
                    // std::cout << "CENTER COLOUMN" << std::endl;
                    axis[0]++;
                } else
                if(y == (Z.y -1) / 2 ) {
                    // std::cout << "CENTER ROW" << std::endl;
                    axis[1]++;
                } else {
                    // std::cout << "CENTER" << std::endl;
                    axis[0]++;
                    axis[1]++;
                }
            }
        }

        if( axis[0] > MaxCenterColumn) {
            MaxCenterColumn = axis[0];
            std::cout << MaxCenterColumn << " @ " << N << std::endl << std::endl;
        } else {
            std::cout << "\x1b[1;F - " << N << std::endl;
        }

        if(axis[0] > Z.y / 4) {
            DumpPositions(newPositions, Z.x, Z.y);
            std::cout << "N: " << N << std::endl << std::endl;
            // break;
        }
        // DumpPositions(newPositions, Z.x, Z.y);
    }

    // std::cout << "New positions:" << newPositions.size() << std::endl;
    // std::cout << "Quadrants:" << quadrants[0] << " " << quadrants[1] << " " << quadrants[2] << " " << quadrants[3] << std::endl;
    // std::cout << "Safety factor: " << (quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]) << std::endl;
*/

    for(size_t i = 0; i < positions.size(); i++) {
        /*
        auto x = positions[i].first + (N*velocities[i].first);
        auto y = positions[i].second + (N*velocities[i].second);
        if(std::abs(x) > std::abs(Z.x)) {x = x % Z.x;}
        if(std::abs(y) > std::abs(Z.y)) {y = y % Z.y;}
        if(x < 0) {x += Z.x;}
        if(y < 0) {y += Z.y;}
        */

        long long X = (long long)positions[i].first;
        long long V = (long long)velocities[i].first;
        long long M = (long long)Z.x;
        long long k = ((M - 1) / 2) - X;
        long long N = 0;

        if(std::gcd(V, M) != 1) {
           std::cout << "V: " << V << " and " << M << " are not coprime" << std::endl;
        } else {
            auto V_inv = modInverse(V, M);
            if(V_inv == 0) {
                std::cout << "V: " << V << " has no inverse modulo " << M << std::endl;
            } else {
                long long prev = 0;
                for (int j = 0; j < 10; ++j) {
                    N = ((k + j * M) * V_inv) % M;
                    while(N != prev && N < 0) {
                        prev = N;
                        N = ((k + j * M) * V_inv) % M;
                    }
                }
                if( N > 0) {
                    std::cout << "N: " << N << std::endl;
                } else {
                    std::cout << "No solution found" << std::endl;
                }
            }
            if(0) {}
        }
    }

    return 0;
}

void DumpPositions(const std::vector<std::pair<long, long>> & positions, long width, long height) {
    // I want to loop through the vector, taking first all the pairs where X (first) is 0, then 1, then 2, etc.
    // This way I can print the positions in a grid-like fashion
    std::map<long, std::vector<std::pair<long, long>>> sortedPositions;

    for (const auto& pos : positions) {
        sortedPositions[pos.first].push_back(pos);
    }

    // for every row, I want to print the positions in order of Y. If a position is missing, I print a space, otherwise I print a *
    std::cout << '+';
    for (long x = 0; x < width; x++) {
        std::cout << '-';
    }
    std::cout << '+' << std::endl;
    for (long y = 0; y < height; y++) {
        std::cout << '|';
        for (long x = 0; x < width; x++) {
            bool found = false;
            // verify if there is a position with x and y
            for (const auto& pos : sortedPositions[x]) {
                if (pos.second == y) {
                    found = true;
                    break;
                }
            }
            if (found) {
                std::cout << "*";
            } else {
                std::cout << " ";
            }
        }
        std::cout << '|';
        std::cout << std::endl;
    }
    std::cout << '+';
    for (long x = 0; x < width; x++) {
        std::cout << '-';
    }
    std::cout << '+' << std::endl;
    
/*
    for(size_t i = 0; i < positions.size(); i++) {
        std::cout << "pos=<" << positions[i].first << "," << positions[i].second << ">" << std::endl;
    }
*/
}


// Fonction pour trouver l'inverse multiplicatif de V modulo M
long long modInverse(long long V, long long M) {
    long long m0 = M, t, q;
    long long x0 = 0, x1 = 1;

    if (M == 1) return 0;

    while (V > 1) {
        // quotient
        q = V / M;
        t = M;

        // M est le reste maintenant, procédez comme dans l'algorithme d'Euclide
        M = V % M, V = t;
        t = x0;

        x0 = x1 - q * x0;
        x1 = t;
    }

    // Assurez-vous que x1 est positif
    if (x1 < 0) x1 += m0;

    return x1;
}
