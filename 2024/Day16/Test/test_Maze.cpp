#include "../Maze.h"

int main(int argc, char ** argv, char ** envp) {

    if(argc == 1) {
        std::cerr << "Usage: test_Maze <param>" << std::endl;
        return 1;
    }

    // Assignment tests
    if(argc == 2 && 0 == strcmp(argv[1], "assign")) {
        std::vector<std::vector<char>> map = {
            {'S', '.', '.', '#'},
            {'.', '#', '.', 'E'},
            {'.', '.', '.', '#'}
        };
        Maze maze1(map);
        Maze maze2 = maze1;
        if(maze1.GetMap() != maze2.GetMap()) {
            std::cerr << "Assignment failed" << std::endl;
            return 1;
        }
        std::cerr << "Assignment OK" << std::endl;
        return 0;
    }

    // Conversion tests
    if(argc == 2 && 0 == strcmp(argv[1], "convert")) {
        std::vector<std::vector<char>> map = {
            {'S', '.', '.', '#'},
            {'.', '#', '.', 'E'},
            {'.', '.', '.', '#'}
        };
        Maze maze(map);
        std::vector<std::vector<char>> map2 = maze;
        if(map != map2) {
            std::cerr << "Conversion failed" << std::endl;
            return 1;
        }
        std::cerr << "Conversion OK" << std::endl;
        return 0;
    }

    // Accessors tests
    if(argc == 2 && 0 == strcmp(argv[1], "accessors")) {
        std::vector<std::vector<char>> map = {
            {'S', '.', '.', '#'},
            {'.', '#', '.', 'E'},
            {'.', '.', '.', '#'}
        };
        Maze maze(map);
        if(maze.Origin() != std::make_pair(0, 0)) {
            std::cerr << "Origin accessor failed" << std::endl;
            return 1;
        }
        if(maze.Start() != std::make_pair(0, 0)) {
            std::cerr << "Start accessor failed" << std::endl;
            return 1;
        }
        if(maze.Exit() != std::make_pair(3, 1)) {
            std::cerr << "Exit accessor failed" << std::endl;
            return 1;
        }
        if(maze.TopRight() != std::make_pair(4, 3)) {
            std::cerr << "TopRight accessor failed" << std::endl;
            return 1;
        }
        if(maze.Width() != 4) {
            std::cerr << "Width accessor failed" << std::endl;
            return 1;
        }
        if(maze.Height() != 3) {
            std::cerr << "Height accessor failed" << std::endl;
            return 1;
        }
        std::cerr << "Accessors OK" << std::endl;
        return 0;
    }

    // Getters tests
    if(argc == 2 && 0 == strcmp(argv[1], "getters")) {
        std::vector<std::vector<char>> map = {
            {'.', '^', '.'},
            {'<', '#', '>'},
            {'.', 'v', '.'}
        };
        Maze maze(map);
        if(maze.GetMap() != map) {
            std::cerr << "GetMap failed" << std::endl;
            return 1;
        }
        if(maze.GetTile(std::make_pair(1, 1)) != '#') {
            std::cerr << "GetTile failed" << std::endl;
            return 1;
        }
        if(maze.GetTileForward(std::make_pair(1, 1), Compass::EAST) != '>') {
            std::cerr << "GetTileForward failed" << std::endl;
            return 1;
        }
        if(maze.GetTileClockwise(std::make_pair(1, 1), Compass::EAST) != 'v') {
            std::cerr << "GetTileClockwise failed" << std::endl;
            return 1;
        }
        if(maze.GetTileCounterClockwise(std::make_pair(1, 1), Compass::EAST) != '^') {
            std::cerr << "GetTileCounterClockwise failed" << std::endl;
            return 1;
        }
        if(maze.GetTileReverse(std::make_pair(1, 1), Compass::EAST) != '<') {
            std::cerr << "GetTileReverse failed" << std::endl;
            return 1;
        }
        std::cerr << "Getters OK" << std::endl;
        return 0;
    }

    // Setters tests
    if(argc == 2 && 0 == strcmp(argv[1], "setters")) {
        std::vector<std::vector<char>> map = {
            {'.', '^', '.'},
            {'<', '#', '>'},
            {'.', 'v', '.'}
        };
        Maze maze(map);
        maze.SetTile(std::make_pair(1,1), 'X');
        if(maze.GetTile(std::make_pair(1,1)) != 'X') {
            std::cerr << "SetTile failed" << std::endl;
            return 1;
        }
        maze.SetTileForward(std::make_pair(1,1), Compass::EAST, 'Y');
        if(maze.GetTile(std::make_pair(2, 1)) != 'Y') {
            std::cerr << "SetTileForward failed" << std::endl;
            return 1;
        }
        maze.SetTileClockwise(std::make_pair(1,1), Compass::EAST, 'Z');
        if(maze.GetTile(std::make_pair(1, 2)) != 'Z') {
            std::cerr << "SetTileClockwise failed" << std::endl;
            return 1;
        }
        maze.SetTileCounterClockwise(std::make_pair(1,1), Compass::EAST, 'W');
        if(maze.GetTile(std::make_pair(1, 0)) != 'W') {
            std::cerr << "SetTileCounterClockwise failed" << std::endl;
            return 1;
        }
        maze.SetTileReverse(std::make_pair(1,1), Compass::EAST, 'V');
        if(maze.GetTile(std::make_pair(0,1)) != 'V') {
            std::cerr << "SetTileReverse failed" << std::endl;
            return 1;
        }
        std::cerr << "Setters OK" << std::endl;
        return 0;
    }

    // Remarkable value tests
    if(argc == 2 && 0 == strcmp(argv[1], "remarkable")) {
        std::vector<std::vector<char>> map = {
            {'S', '.', '.', '#'},
            {'.', '#', '.', 'E'},
            {'.', '.', '.', '#'}
        };
        Maze maze(map);
        if(!maze.IsStart(std::make_pair(0, 0))) {
            std::cerr << "IsStart failed" << std::endl;
            return 1;
        }
        if(!maze.IsExit(std::make_pair(3, 1))) {
            std::cerr << "IsExit failed" << std::endl;
            return 1;
        }
        if(!maze.IsWall(std::make_pair(3, 0))) {
            std::cerr << "IsWall failed" << std::endl;
            return 1;
        }
        if(!maze.IsFree(std::make_pair(1, 0))) {
            std::cerr << "IsFree failed" << std::endl;
            return 1;
        }
        std::cerr << "Remarkable values OK" << std::endl;
        return 0;
    }

    std::cerr << "Usage: test_Maze <param (assign|convert|accessors|getters|setters|remarkable)>" << std::endl;
    return 1;
}
