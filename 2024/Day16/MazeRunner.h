#ifndef __DAY16_MAZERUNNER_H__
#define __DAY16_MAZERUNNER_H__

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <iomanip>

#include "Compass.h"
#include "Maze.h"

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

    std::vector<MazeSegment> segments;
    MazeRunnerPosition start;
    long bestScore = LONG_MAX;

public:
    MazeRunner() = default;
    MazeRunner(const MazeRunner & other) = default;
    MazeRunner & operator=(const MazeRunner & other) = default;
    MazeRunner(const Maze & map);

    long Run();
    MazeRunnerPosition Walk(const MazeRunnerPosition & r) const;
    MazeRunnerPosition Step(const MazeRunnerPosition & r, Compass d) const;
    MazeRunnerPosition JumpBack();
    bool DetectLoop(const MazeSegment & s);
    

    long Run3(std::function<void(std::vector<MazeSegment>,long)> exitCallback);

protected:

    void Reset();
};

#endif // __DAY16_MAZERUNNER_H__
