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
#include <shared_mutex>

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
    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
    long bestScore = UPPERLIMIT;
    std::vector<MazeSegment> bestPath;
    std::mutex mtx; // Mutex for synchronizing access to bestScore

public:
    MazeRunner() = default;
    MazeRunner(const MazeRunner & other) = default;
    MazeRunner & operator=(const MazeRunner & other) = default;
    MazeRunner(const Maze & map);

    // long Run(std::function<bool(long long,const std::vector<MazeSegment> &)> cb);
    long Run(std::string banner, std::function<std::vector<Compass>(const std::vector<Compass>&)>);
    MazeRunnerPosition Walk(const MazeRunnerPosition & r, std::function<std::vector<Compass>(const std::vector<Compass>&)>);
    MazeRunnerPosition Step(const MazeRunnerPosition & r, Compass d) const;
    MazeSegment JumpBack(std::vector<MazeSegment> & ss, std::function<std::vector<Compass>(const std::vector<Compass>&)>);
    bool DetectLoop(std::vector<MazeSegment> & ss, const MazeSegment & s);
    void CondemnLastSegment(std::vector<MazeSegment> & ss);
    

    // long __Run_Backup(std::function<bool(long long,const std::vector<MazeSegment> &)> cb);        // (std::function<void(std::vector<MazeSegment>,long)> exitCallback)
    long __Run_Backup();

    void ProgressBar(std::string & banner, long long n, std::chrono::steady_clock::time_point start, long numSegments, long maxSegments, long score, long bestScore, int w = 100, bool isBest = false);
};

#endif // __DAY16_MAZERUNNER_H__
