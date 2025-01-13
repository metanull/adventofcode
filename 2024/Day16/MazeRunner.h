#ifndef __DAY16_MAZERUNNER_H__
#define __DAY16_MAZERUNNER_H__

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>

#include "Compass.h"
#include "Maze.h"

struct MazeRunnerPosition {
    std::pair<int,int> position = std::make_pair(0,0);
    Compass direction = Compass::UNKNOWN;
    long score = 0;

    MazeRunnerPosition() = default;
    MazeRunnerPosition(const MazeRunnerPosition & other) = default;
    MazeRunnerPosition & operator=(const MazeRunnerPosition & other) = default;
};

struct MazeRunnerCrosspoint {
    MazeRunnerPosition origin;          // Position of the crosspoint & direction
    std::vector<Compass> options;       // List available/remaining options at the cross point

    MazeRunnerCrosspoint() = default;
    MazeRunnerCrosspoint(const MazeRunnerCrosspoint & other) = default;
    MazeRunnerCrosspoint & operator=(const MazeRunnerCrosspoint & other) = default;
};

struct MazeSegment {
    MazeRunnerPosition origin;
    MazeRunnerPosition end;
    std::vector<MazeRunnerPosition> moves;

    MazeSegment() = default;
    MazeSegment(const MazeSegment & other) = default;
    MazeSegment & operator=(const MazeSegment & other) = default;
};

class MazeRunner {
protected:
    Maze maze;
    std::vector<MazeRunnerCrosspoint> crosspoints;
    std::stack<MazeSegment> segments;
    std::stack<MazeSegment> bestPath;
    MazeRunnerPosition current;
    MazeSegment currentSegment;
    long bestScore = LONG_MAX;

public:
    MazeRunner() = default;
    MazeRunner(const MazeRunner & other) = default;
    MazeRunner & operator=(const MazeRunner & other) = default;
    MazeRunner(const Maze & map);

    long Run(std::function<void(std::stack<MazeSegment>,long)> callback);

protected:
    /**
     * @brief Save the current position as a crosspoint
     */
    void SaveCrosspoint();
    /**
     * @brief Interrupt processing, and restore the runner to the last valid crosspoint
     */
    bool RestoreFromLastCrosspoint();
    /**
     * @brief Get the options available to the runner
     */
    int Options(bool & forward, bool & clockwise, bool & counter_clockwise, bool & reverse) const;
    /**
     * @brief Get the options available to the runner as a list
    */
    std::vector<Compass> Options() const;
    /**
     * @brief Make a choice among the available options
     */
    Compass Option(int choice, bool & rotation) const;
    /**
    * @brief Check if the current position is a crosspoint
    */
    bool IsCrosspoint() const;
    /**
     * @brief Check if the current position is the exit
     */
    bool IsExit() const;
    /**
     * @brief Check if the current position is a dead end
     */
    bool IsDeadEnd() const;
    /**
     * @brief Check if the current position is the start
     */
    bool IsStart() const;

};

#endif // __DAY16_MAZERUNNER_H__
