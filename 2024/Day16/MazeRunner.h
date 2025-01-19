#ifndef __DAY16_MAZERUNNER_H__
#define __DAY16_MAZERUNNER_H__

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <algorithm>
#include <map>
#include <sstream>
#include <chrono>
#include <mutex>

#include "Constant.h"
#include "Compass.h"
#include "Maze.h"

#ifndef UPPERLIMIT
    #define UPPERLIMIT LONG_MAX
#endif

struct MazeRunnerPosition {
    std::pair<int,int> position = std::make_pair(0,0);
    Compass direction = Compass::UNKNOWN;
    long score = 0;

    MazeRunnerPosition() = default;
    MazeRunnerPosition(const MazeRunnerPosition & other) = default;
    MazeRunnerPosition & operator=(const MazeRunnerPosition & other) = default;

    friend std::ostream& operator<<(std::ostream& os, const MazeRunnerPosition & p);
};

struct MazeSegment {
    MazeRunnerPosition origin;
    MazeRunnerPosition end;
    std::vector<MazeRunnerPosition> moves;

    MazeSegment() = default;
    MazeSegment(const MazeSegment & other) = default;
    MazeSegment & operator=(const MazeSegment & other) = default;

    friend std::ostream & operator<<(std::ostream & os, const MazeSegment & s);
};

class MazeRunner {
protected:
    Maze maze;
    long bestScore = LONG_MAX;
    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

public:
    MazeRunner() = default;
    MazeRunner(const MazeRunner & other) = default;
    MazeRunner & operator=(const MazeRunner & other) = default;
    MazeRunner(const Maze & map);

    // long Run(std::function<bool(long long,const std::vector<MazeSegment> &)> cb);
    long Run();
    MazeRunnerPosition Walk(const MazeRunnerPosition & r) const;
    MazeRunnerPosition Step(const MazeRunnerPosition & r, Compass d) const;
    MazeSegment JumpBack(std::vector<MazeSegment> & ss);
    bool DetectLoop(std::vector<MazeSegment> & ss, const MazeSegment & s);
    

    // long Run3(std::function<bool(long long,const std::vector<MazeSegment> &)> cb);        // (std::function<void(std::vector<MazeSegment>,long)> exitCallback)
    long Run3();

    void ProgressBar(long long n, std::chrono::steady_clock::time_point start, long numSegments, long maxSegments, long score, long bestScore, int w = 100);

protected:

    void Reset();
};

#endif // __DAY16_MAZERUNNER_H__
