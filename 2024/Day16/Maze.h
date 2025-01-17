#ifndef __DAY16_MAZE_H__
#define __DAY16_MAZE_H__

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "Compass.h"

class Maze {
protected:
    std::pair<int,int> O = {0,0};   // The Origin
    std::pair<int,int> S = {0,0};   // The Start
    std::pair<int,int> E = {0,0};   // The Exit
    std::pair<int,int> Z = {0,0};   // The Top Right Corner
    
    std::vector<std::vector<char>> M; // The Maze

public:
    const static char START_CHAR = 'S';
    const static char EXIT_CHAR = 'E';
    const static char WALL_CHAR = '#';
    const static char EMPTY_CHAR = '.';

    Maze() = default;
    Maze(const Maze & other) = default;
    Maze(const std::vector<std::vector<char>> & map);
    Maze & operator=(const Maze & other) = default;
    Maze & operator=(const std::vector<std::vector<char>> & map);
    Maze & LoadMazeFromMap(const std::vector<std::vector<char>> & map);

    long Width() const;
    long Height() const;

    std::pair<int,int> Origin() const;
    std::pair<int,int> Start() const;
    std::pair<int,int> Exit() const;
    std::pair<int,int> TopRight() const;

    char GetTile(std::pair<int,int> p) const;
    char SetTile(std::pair<int,int> p, char c);
    
    std::pair<int,int> Move(std::pair<int,int> c, Compass d) const;

    bool IsWall(std::pair<int,int> p) const;
    bool IsFree(std::pair<int,int> p) const;
    bool IsExit(std::pair<int,int> p) const;
    bool IsStart(std::pair<int,int> p) const;

    bool IsDeadEnd(std::pair<int,int> p, Compass d) const;
    bool IsCrossPoint(std::pair<int,int> p, Compass d) const;

    std::vector<Compass>Options(std::pair<int,int> p, Compass d) const;

    char GetTileForward(std::pair<int,int> p, Compass d) const;
    char GetTileClockwise(std::pair<int,int> p, Compass d) const;
    char GetTileCounterClockwise(std::pair<int,int> p, Compass d) const;
    char GetTileReverse(std::pair<int,int> p, Compass d) const;
    char SetTileForward(std::pair<int,int> p, Compass d, char c);
    char SetTileClockwise(std::pair<int,int> p, Compass d, char c);
    char SetTileCounterClockwise(std::pair<int,int> p, Compass d, char c);
    char SetTileReverse(std::pair<int,int> p, Compass d, char c);

    const std::vector<std::vector<char>> & GetMap() const;

    operator std::vector<std::vector<char>>() const;

    friend std::ostream & operator<<(std::ostream & os, const Maze & maze);

    bool operator==(const Maze & other) const;
    bool operator==(const std::vector<std::vector<char>> & map) const;
    bool operator!=(const Maze & other) const;
    bool operator!=(const std::vector<std::vector<char>> & map) const;

};

#endif // __DAY16_MAZE_H__
