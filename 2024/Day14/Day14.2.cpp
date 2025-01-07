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
#include <set>
#include <unordered_map>
// #include <compare>

#include "..\AdventOfCode.h"
#include "..\Math.h"
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

#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm> // for std::sort

using namespace std;

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Function to find the value of X with the greatest number of consecutive vertically aligned points
pair<long, int> findMostFrequentConsecutiveX(const set<pair<long, long>>& seen) {
    // Convert set to vector for sorting
    vector<pair<long, long>> vec(seen.begin(), seen.end());

    // Sort the vector by X value first, then by Y value
    sort(vec.begin(), vec.end(), [](const pair<long, long>& a, const pair<long, long>& b) {
        if (a.first != b.first) {
            return a.first < b.first;
        }
        return a.second < b.second;
    });

    // Count consecutive vertical points for each X value
    unordered_map<long, int> frequency;
    long prevX = -1;
    long prevY = -1;
    int consecutiveCount = 0;
    int maxCount = 0;
    long mostFrequentX = -1;

    for (const auto& p : vec) {
        if (p.first == prevX && p.second == prevY + 1) {
            consecutiveCount++;
        } else {
            if (consecutiveCount > maxCount) {
                maxCount = consecutiveCount;
                mostFrequentX = prevX;
            }
            if (p.first == prevX) {
                consecutiveCount = 1; // Reset the count for the next consecutive sequence
            } else {
                consecutiveCount = 1; // Reset the count for a new X value
            }
        }
        prevX = p.first;
        prevY = p.second;
    }

    // Check the last sequence
    if (consecutiveCount > maxCount) {
        maxCount = consecutiveCount;
        mostFrequentX = prevX;
    }

    return {mostFrequentX, maxCount};
}


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

    int maxCount = 0;
    std::cout << std::endl;
    for(long long i = 0; i < 50000; i ++) {
        std::cout << "\x1b[1;F - " << i << std::endl;

        std::set<std::pair<long, long>> seen;
        for(auto j = 0; j < positions.size(); j++) {
            auto nx = (positions[j].first + i * velocities[j].first) % Z.x;
            auto ny = (positions[j].second + i * velocities[j].second) % Z.y;
            // Ensure nx and ny are non-negative
            if (nx < 0) { nx += Z.x; }
            if (ny < 0) { ny += Z.y; }

            seen.emplace(nx, ny);
        }
        
        // Let's bet that all robots are in a distinct X,Y position
        // If it doesn't work, comment out the if; or replace with a lower value, e.g. (see.size() >= (positions.size() - 10 ))
        if(seen.size() == positions.size()) {
            // Find the trunc of the tree (would be a straight vertical line of a consequent size, let's say 24, it's X-Mas)
            pair<long, int> result = findMostFrequentConsecutiveX(seen);
            if(result.second > maxCount) {
                maxCount = result.second;
                std::cout << "X: " << result.first << " - consecutive Y: " << result.second << " - SEEN: " << seen.size() << std::endl << std::endl;

                if(result.second >= 24) {
                    std::cout << "TRYING AT " << i << std::endl << std::endl;    
                    vector<pair<long, long>> vec(seen.begin(), seen.end());
                    DumpPositions(vec, Z.x, Z.y);

                    std::cout << "Number of iterations: " << i << std::endl << std::endl;    
                    // If that was not the right one, comment out
                    break;
                }
                
            }
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
