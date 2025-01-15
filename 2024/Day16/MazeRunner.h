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

struct MazeRunnerCrosspoint {
    MazeRunnerPosition origin;          // Position of the crosspoint & direction
    std::vector<Compass> options;       // List available/remaining options at the cross point

    MazeRunnerCrosspoint() = default;
    MazeRunnerCrosspoint(const MazeRunnerCrosspoint & other) = default;
    MazeRunnerCrosspoint & operator=(const MazeRunnerCrosspoint & other) = default;

    /**
     * @brief Get the next option to try
     */
    Compass GetOption();
    /**
     * @brief Erase the next option to try (and return it)
     */
    Compass EraseOption();

    friend std::ostream & operator<<(std::ostream & os, const MazeRunnerCrosspoint & cp);
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
    std::vector<MazeRunnerCrosspoint> crosspoints;
    std::stack<MazeSegment> segments;
    std::stack<MazeSegment> bestPath;
    MazeRunnerPosition current;
    MazeSegment currentSegment;
    long bestScore = LONG_MAX;
    bool justRestored = false;

public:
    MazeRunner() = default;
    MazeRunner(const MazeRunner & other) = default;
    MazeRunner & operator=(const MazeRunner & other) = default;
    MazeRunner(const Maze & map);

    long Run(std::function<void(std::stack<MazeSegment>,long)> callback);
    long MazeRunner::Run2(std::function<void(std::stack<MazeSegment>,long)> exitCallback);

protected:
    bool OnCrosspointReached();
    bool OnTileReached(MazeRunnerPosition & cur, const MazeRunnerPosition & prev);
    bool OnExitReached(std::function<void(std::stack<MazeSegment>,long)> onexit);
    bool OnDeadEndReached();
    bool OnLoopDetected();

    void PushSegment();
    MazeSegment PopSegment();
    size_t CountSegment();

    bool CrosspointExists() const;
    void PushCrosspoint();
    MazeRunnerCrosspoint PopCrosspoint();

    MazeRunnerPosition GetPosition() const;

    void Reset();

    // iterator => iterates over all segments
    // iterator => iterates over all moves
    // iterator => iterates over all crosspoints

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

public: 
/*//template<typename C, typename T = typename C::value_type>
    template<typename C, typename T = std::decay_t<decltype(*begin(std::declval<C>()))>>
    static void DumpSegment(C const & c, std::string ansi = "\033[0m");
    */
    static void DumpSegment(const std::stack<MazeSegment> & segments, std::string ansi = "\033[0m");
    static void DumpSegment(const MazeSegment & segment, std::string ansi = "\033[0m");

    static void DumpCrosspoint(const std::vector<MazeRunnerCrosspoint> & crosspoint, std::string ansi = "\033[0m");
    static void DumpCrosspoint(const MazeRunnerCrosspoint & crosspoint, std::string ansi = "\033[0m");

};

#endif // __DAY16_MAZERUNNER_H__
