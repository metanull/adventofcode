#ifndef ARGV2_EQUALS
    #define ARGV2_EQUALS(x) (argc == 2 && 0 == strcmp(argv[1], x))
#endif

#include <functional>
#include <map>

#include "charmap.h"
#include "maze.h"

int main(int argc, char ** argv, char ** envp) {
    if(argc != 2) {
        std::cerr << "Usage: test_charmap <test>" << std::endl;
        return 1;
    }
    std::string test_name = argv[1];
    std::map<std::string, std::function<bool()>> tests = {
        {"dummy", []() -> bool {
            std::cerr << "Expected result: true - for expression true" << std::endl;
            return true;
        }},
        {"is_tile_free-free_char", []() -> bool {
            std::cerr << "Expected result: true - for expression is_tile_free({{'S','#'},{'.','E'}},{0,1},{1,1})" << std::endl;
            return maze_node::is_tile_free({{'S','#'},{'.','E'}},{0,1},{1,1});
        }},
        {"is_tile_free-exit", []() -> bool {
            std::cerr << "Expected result: true - for expression is_tile_free({{'S','#'},{'.','E'}},{1,1},{1,1})" << std::endl;
            return maze_node::is_tile_free({{'S','#'},{'.','E'}},{0,1},{1,1});
        }},
        {"is_tile_free-other_char", []() -> bool {
            std::cerr << "Expected result: false - for expression is_tile_free({{'S','#'},{'.','E'}},{1,0},{1,1})" << std::endl;
            return maze_node::is_tile_free({{'S','#'},{'.','E'}},{0,1},{1,1});
        }},
        {"is_tile_free-start", []() -> bool {
            std::cerr << "Expected result: false - for expression is_tile_free({{'S','#'},{'.','E'}},{0,0},{1,1})" << std::endl;
            return maze_node::is_tile_free({{'S','#'},{'.','E'}},{0,1},{1,1});
        }},
        {"is_tile_free-out-of-range", []() -> bool {
            std::cerr << "Expected result: exception std::out-of-range - for expression is_tile_free({{'S','#'},{'.','E'}},{2,2},{1,1})" << std::endl;
            try {
                maze_node::is_tile_free({{'S','#'},{'.','E'}},{2,2},{1,1});
            } catch(std::out_of_range & e) {
                return true;
            }
            return false;
        }},
        {"init-empty", []() -> bool {
            std::cerr << "Expected result: empty vector - for expression init({}, {0,0}, {1,1}, EAST, is_tile_free)" << std::endl;
            return maze_node::init({}, {0,0}, {1,1}, metanull::charmap::EAST, maze_node::is_tile_free).size() == 0;
        }},
        {"init-no-options", []() -> bool {
            std::cerr << "Expected result: exception std::runtime_error - for expression init({{'S','#'},{'.','E'}}, {0,0}, {1,1}, EAST, is_tile_free)" << std::endl;
            try {
                maze_node::init({{'S','#'},{'.','E'}}, {0,0}, {1,1}, metanull::charmap::EAST, maze_node::is_tile_free);
            } catch(std::runtime_error & e) {
                return true;
            }
            return false;
        }},
        {"init-one-option", []() -> bool {
            std::cerr << "Expected result: vector with one element - for expression init({{'S','#'},{'.','E'}}, {0,0}, {1,1}, EAST, is_tile_free)" << std::endl;
            auto nodes = maze_node::init({{'S','#'},{'.','E'}}, {0,0}, {1,1}, metanull::charmap::EAST, maze_node::is_tile_free);
            return nodes.size() == 1 && nodes[0].start == metanull::charmap::position{0,0} && nodes[0].start_direction == metanull::charmap::EAST && nodes[0].end == metanull::charmap::position{0,1} && nodes[0].end_direction == metanull::charmap::SOUTH && nodes[0].score == 1001 && nodes[0].visited.size() == 2 && nodes[0].visited[0] == metanull::charmap::position{0,0} && nodes[0].visited[1] == metanull::charmap::position{0,1};
        }},
        /*
        
        {"construct", []() -> bool {
            maze m;
            std::cerr << "Expected result: new maze object {0,0},EAST,{0,0},EAST,[],[],0 - for expression maze(m) == n" << std::endl;
            return m.start == metanull::charmap::position{0,0} && m.end == metanull::charmap::position{0,0} && m.start_direction == metanull::charmap::EAST && m.open_nodes.size() == 0 && m.m.size() == 0;
        }},
        {"construct-copy", []() -> bool {
            maze n = maze(metanull::charmap::map{
                {'S','.','#'},
                {'#','.','#'},
                {'#','.','E'}
            }, metanull::charmap::position{0,0}, metanull::charmap::position{2,2}, metanull::charmap::EAST);
            std::cerr << "Expected result: new maze object equivalent to n - for expression maze(n)" << std::endl;
            auto m(n);
            return m.start == metanull::charmap::position{0,0} && m.end == metanull::charmap::position{2,2} && m.start_direction == metanull::charmap::EAST && m.open_nodes.size() == 0 
                && m.m.size() == 3 && m.m[0].size() == 3 && m.m[1].size() == 3 && m.m[2].size() == 3 
                && m.m[0][0] == 'S' && m.m[0][1] == '.' && m.m[0][2] == '#' 
                && m.m[1][0] == '#' && m.m[1][1] == '.' && m.m[1][2] == '#'
                && m.m[2][0] == '#' && m.m[2][1] == '.' && m.m[2][2] == 'E';
        }},
        {"assign", []() -> bool {
            maze n = maze(metanull::charmap::map{
                {'S','.','#'},
                {'#','.','#'},
                {'#','.','E'}
            }, metanull::charmap::position{0,0}, metanull::charmap::position{2,2}, metanull::charmap::EAST);
            std::cerr << "Expected result: new maze object equivalent to n - for expression maze(n)" << std::endl;
            auto m = n;
            return m.start == metanull::charmap::position{0,0} && m.end == metanull::charmap::position{2,2} && m.start_direction == metanull::charmap::EAST && m.open_nodes.size() == 0 
                && m.m.size() == 3 && m.m[0].size() == 3 && m.m[1].size() == 3 && m.m[2].size() == 3 
                && m.m[0][0] == 'S' && m.m[0][1] == '.' && m.m[0][2] == '#' 
                && m.m[1][0] == '#' && m.m[1][1] == '.' && m.m[1][2] == '#'
                && m.m[2][0] == '#' && m.m[2][1] == '.' && m.m[2][2] == 'E';
        }},
        {"construct-position", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'S','.','#'},
                {'#','.','#'},
                {'#','.','E'}
            }, metanull::charmap::position{0,0}, metanull::charmap::position{2,2}, metanull::charmap::EAST);
            std::cerr << "Expected result: new maze object {0,0},EAST,{2,2},EAST,[],[],0 - for expression maze(metanull::charmap::position{0,0},metanull::charmap::position{2,2})" << std::endl;
            return m.start == metanull::charmap::position{0,0} && m.end == metanull::charmap::position{2,2} && m.start_direction == metanull::charmap::EAST && m.open_nodes.size() == 0 
                && m.m.size() == 3 && m.m[0].size() == 3 && m.m[1].size() == 3 && m.m[2].size() == 3 
                && m.m[0][0] == 'S' && m.m[0][1] == '.' && m.m[0][2] == '#' 
                && m.m[1][0] == '#' && m.m[1][1] == '.' && m.m[1][2] == '#'
                && m.m[2][0] == '#' && m.m[2][1] == '.' && m.m[2][2] == 'E';
        }},
        {"construct-char", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'S','.','#'},
                {'#','.','#'},
                {'#','.','E'}
            }, 'S', 'E', metanull::charmap::EAST);
            std::cerr << "Expected result: new maze object {0,0},EAST,{2,2},EAST,[],[],0 - for expression maze(metanull::charmap::position{0,0},metanull::charmap::position{2,2})" << std::endl;
            return m.start == metanull::charmap::position{0,0} && m.end == metanull::charmap::position{2,2} && m.start_direction == metanull::charmap::EAST && m.open_nodes.size() == 0 
                && m.m.size() == 3 && m.m[0].size() == 3 && m.m[1].size() == 3 && m.m[2].size() == 3 
                && m.m[0][0] == 'S' && m.m[0][1] == '.' && m.m[0][2] == '#' 
                && m.m[1][0] == '#' && m.m[1][1] == '.' && m.m[1][2] == '#'
                && m.m[2][0] == '#' && m.m[2][1] == '.' && m.m[2][2] == 'E';
        }},
        {"init-3x3-1-option", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'S','.','#'},
                {'#','.','#'},
                {'#','.','E'}
            }, 'S', 'E', metanull::charmap::EAST);
            auto nodes = m.init();
            std::cerr << "Expected result: 1 node with start {0,0},EAST,end {1,0},EAST,score 1,visited {0,0},turns [],closed false - for expression maze.init()" << std::endl;
            std::cerr << "nodes.size(): " << nodes.size() << std::endl;
            if (!nodes.empty()) {
                for(auto nn : nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return nodes.size() == 1 
                && nodes[0].start == metanull::charmap::position{0,0} && nodes[0].start_direction == metanull::charmap::EAST 
                && nodes[0].end == metanull::charmap::position{1,0} && nodes[0].end_direction == metanull::charmap::EAST 
                && nodes[0].score == 1 
                && nodes[0].visited.size() == 2 
                && nodes[0].visited.front() == metanull::charmap::position{0,0} 
                && nodes[0].visited.back() == metanull::charmap::position{1,0} 
                && nodes[0].turns.size() == 0 
                && !nodes[0].closed;
        }},
        {"init-3x3-2-options", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'S','.','#'},
                {'.','#','#'},
                {'.','.','E'}
            }, 'S', 'E', metanull::charmap::EAST);
            auto nodes = m.init();
            std::cerr << "Expected result: 2 nodes with start {0,0},EAST,end {1,0},EAST,score 1,visited {0,0},turns [],closed false and start {0,0},EAST,end {0,1},SOUTH,score 1001,visited {0,0},turns {0,0},closed false - for expression maze.init()" << std::endl;
            std::cerr << "nodes.size(): " << nodes.size() << std::endl;
            if (!nodes.empty()) {
                for(auto nn : nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return nodes.size() == 2 
                && nodes[0].start == metanull::charmap::position{0,0} && nodes[0].start_direction == metanull::charmap::EAST 
                && nodes[0].end == metanull::charmap::position{1,0} && nodes[0].end_direction == metanull::charmap::EAST 
                && nodes[0].score == 1 
                && nodes[0].visited.size() == 2
                && nodes[0].visited.front() == metanull::charmap::position{0,0} 
                && nodes[0].visited.back() == metanull::charmap::position{1,0} 
                && nodes[0].turns.size() == 0 
                && !nodes[0].closed
                && nodes[1].start == metanull::charmap::position{0,0} && nodes[1].start_direction == metanull::charmap::EAST 
                && nodes[1].end == metanull::charmap::position{0,1} && nodes[1].end_direction == metanull::charmap::SOUTH 
                && nodes[1].score == 1001 
                && nodes[1].visited.size() == 2
                && nodes[1].visited.front() == metanull::charmap::position{0,0} 
                && nodes[1].visited.back() == metanull::charmap::position{0,1} 
                && nodes[1].turns.size() == 1 
                && nodes[1].turns.front() == metanull::charmap::position{0,0} 
                && !nodes[1].closed;
        }},
        {"init-3x3-1-option-bkwd", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'E','.','#'},
                {'#','.','#'},
                {'#','.','S'}
            }, 'S', 'E', metanull::charmap::EAST);
            auto nodes = m.init();
            std::cerr << "Expected result: 1 node with start {2,2},EAST,end {1,2},WEST,score 1001,visited {2,2},turns {2,2},closed false - for expression maze.init()" << std::endl;
            std::cerr << "nodes.size(): " << nodes.size() << std::endl;
            if (!nodes.empty()) {
                for(auto nn : nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return nodes.size() == 1 
                && nodes[0].start == metanull::charmap::position{2,2} && nodes[0].start_direction == metanull::charmap::EAST 
                && nodes[0].end == metanull::charmap::position{1,2} && nodes[0].end_direction == metanull::charmap::WEST 
                && nodes[0].score == 1001 
                && nodes[0].visited.size() == 2
                && nodes[0].visited.front() == metanull::charmap::position{2,2} 
                && nodes[0].visited.back() == metanull::charmap::position{1,2} 
                && nodes[0].turns.size() == 1 
                && nodes[0].turns.front() == metanull::charmap::position{2,2} 
                && !nodes[0].closed;
        }},
        {"init-3x3-2-options-bkwd", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'E','.','.'},
                {'#','#','.'},
                {'#','.','S'}
            }, 'S', 'E', metanull::charmap::EAST);
            auto nodes = m.init();
            std::cerr << "Expected result: 2 nodes with start {2,2},EAST,end {1,2},WEST,score 1001,visited {2,2},turns {2,2},closed false and start {2,2},EAST,end {2,1},SOUTH,score 1001,visited {2,2},turns {2,2},closed false - for expression maze.init()" << std::endl;
            std::cerr << "nodes.size(): " << nodes.size() << std::endl;
            if (!nodes.empty()) {
                for(auto nn : nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return nodes.size() == 2 
                && nodes[0].start == metanull::charmap::position{2,2}
                && nodes[0].start_direction == metanull::charmap::EAST 
                && nodes[0].end == metanull::charmap::position{2,1} 
                && nodes[0].end_direction == metanull::charmap::NORTH 
                && nodes[0].score == 1001 
                && nodes[0].visited.size() == 2 
                && nodes[0].visited.front() == metanull::charmap::position{2,2} 
                && nodes[0].visited.back() == metanull::charmap::position{2,1} 
                && nodes[0].turns.size() == 1 
                && nodes[0].turns.front() == metanull::charmap::position{2,2} 
                && !nodes[0].closed
                && nodes[1].start == metanull::charmap::position{2,2} 
                && nodes[1].start_direction == metanull::charmap::EAST 
                && nodes[1].end == metanull::charmap::position{1,2} 
                && nodes[1].end_direction == metanull::charmap::WEST 
                && nodes[1].score == 1001 
                && nodes[1].visited.size() == 2
                && nodes[1].visited.front() == metanull::charmap::position{2,2} 
                && nodes[1].visited.back() == metanull::charmap::position{1,2} 
                && nodes[1].turns.size() == 1 
                && nodes[1].turns.front() == metanull::charmap::position{2,2} 
                && !nodes[1].closed;
        }},
        {"init-3x3-3-options", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'.','.','#'},
                {'.','S','#'},
                {'.','.','E'}
            }, 'S', 'E', metanull::charmap::EAST);
            auto nodes = m.init();
            std::cerr << "Expected result: 3 nodes with start {1,1},EAST,end {2,1},EAST,score 1,visited {1,1},turns [],closed false and start {1,1},EAST,end {1,2},SOUTH,score 1001,visited {1,1},turns {1,1},closed false and start {1,1},EAST,end {0,1},NORTH,score 1001,visited {1,1},turns {1,1},closed false - for expression maze.init()" << std::endl;
            std::cerr << "nodes.size(): " << nodes.size() << std::endl;
            if (!nodes.empty()) {
                for(auto nn : nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return nodes.size() == 3 
                && nodes[0].start == metanull::charmap::position{1,1} 
                && nodes[0].start_direction == metanull::charmap::EAST 
                && nodes[0].end == metanull::charmap::position{1,0}
                && nodes[0].end_direction == metanull::charmap::NORTH 
                && nodes[0].score == 1001 
                && nodes[0].visited.size() == 2
                && nodes[0].visited.front() == metanull::charmap::position{1,1} 
                && nodes[0].visited.back() == metanull::charmap::position{1,0} 
                && nodes[0].turns.size() == 1 
                && nodes[0].turns.front() == metanull::charmap::position{1,1} 
                && !nodes[0].closed
                && nodes[1].start == metanull::charmap::position{1,1}
                && nodes[1].start_direction == metanull::charmap::EAST 
                && nodes[1].end == metanull::charmap::position{0,1}
                && nodes[1].end_direction == metanull::charmap::WEST 
                && nodes[1].score == 1001 
                && nodes[1].visited.size() == 2
                && nodes[1].visited.front() == metanull::charmap::position{1,1} 
                && nodes[1].visited.back() == metanull::charmap::position{0,1} 
                && nodes[1].turns.size() == 1 
                && nodes[1].turns.front() == metanull::charmap::position{1,1} 
                && !nodes[1].closed
                && nodes[2].start == metanull::charmap::position{1,1}
                && nodes[2].start_direction == metanull::charmap::EAST 
                && nodes[2].end == metanull::charmap::position{1,2}
                && nodes[2].end_direction == metanull::charmap::SOUTH 
                && nodes[2].score == 1001 
                && nodes[2].visited.size() == 2
                && nodes[2].visited.front() == metanull::charmap::position{1,1} 
                && nodes[2].visited.back() == metanull::charmap::position{1,2} 
                && nodes[2].turns.size() == 1 
                && nodes[2].turns.front() == metanull::charmap::position{1,1} 
                && !nodes[2].closed;
        }},
        {"init-3x3-4-options", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'.','.','#'},
                {'.','S','.'},
                {'.','.','E'}
            }, 'S', 'E', metanull::charmap::EAST);
            auto nodes = m.init();
            std::cerr << "Expected result: 4 nodes with start {1,1},EAST,end {2,1},EAST,score 1,visited {1,1},turns [],closed false and start {1,1},EAST,end {1,2},SOUTH,score 1001,visited {1,1},turns {1,1},closed false and start {1,1},EAST,end {0,1},NORTH,score 1001,visited {1,1},turns {1,1},closed false and start {1,1},EAST,end {1,0},NORTH,score 1001,visited {1,1},turns {1,1},closed false - for expression maze.init()" << std::endl;
            std::cerr << "nodes.size(): " << nodes.size() << std::endl;
            if (!nodes.empty()) {
                for(auto nn : nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return nodes.size() == 4 
                && nodes[0].start == metanull::charmap::position{1,1} 
                && nodes[0].start_direction == metanull::charmap::EAST 
                && nodes[0].end == metanull::charmap::position{2,1} 
                && nodes[0].end_direction == metanull::charmap::EAST 
                && nodes[0].score == 1 
                && nodes[0].visited.size() == 2
                && nodes[0].visited.front() == metanull::charmap::position{1,1} 
                && nodes[0].visited.back() == metanull::charmap::position{2,1} 
                && nodes[0].turns.size() == 0 
                && !nodes[0].closed
                && nodes[1].start == metanull::charmap::position{1,1} 
                && nodes[1].start_direction == metanull::charmap::EAST 
                && nodes[1].end == metanull::charmap::position{1,0} 
                && nodes[1].end_direction == metanull::charmap::NORTH 
                && nodes[1].score == 1001 
                && nodes[1].visited.size() == 2
                && nodes[1].visited.front() == metanull::charmap::position{1,1} 
                && nodes[1].visited.back() == metanull::charmap::position{1,0} 
                && nodes[1].turns.size() == 1 
                && nodes[1].turns.front() == metanull::charmap::position{1,1} 
                && !nodes[1].closed
                && nodes[2].start == metanull::charmap::position{1,1} 
                && nodes[2].start_direction == metanull::charmap::EAST 
                && nodes[2].end == metanull::charmap::position{0,1} 
                && nodes[2].end_direction == metanull::charmap::WEST 
                && nodes[2].score == 1001 
                && nodes[2].visited.size() == 2
                && nodes[2].visited.front() == metanull::charmap::position{1,1} 
                && nodes[2].visited.back() == metanull::charmap::position{0,1} 
                && nodes[2].turns.size() == 1 
                && nodes[2].turns.front() == metanull::charmap::position{1,1} 
                && !nodes[2].closed
                && nodes[3].start == metanull::charmap::position{1,1} 
                && nodes[3].start_direction == metanull::charmap::EAST 
                && nodes[3].end == metanull::charmap::position{1,2} 
                && nodes[3].end_direction == metanull::charmap::SOUTH 
                && nodes[3].score == 1001 
                && nodes[3].visited.size() == 2
                && nodes[3].visited.front() == metanull::charmap::position{1,1} 
                && nodes[3].visited.back() == metanull::charmap::position{1,2} 
                && nodes[3].turns.size() == 1 
                && nodes[3].turns.front() == metanull::charmap::position{1,1} 
                && !nodes[3].closed;
        }},
        {"next_nodes-simple", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'S','.','.','.','E'},
                {'#','#','.','#','#'},
                {'#','#','.','#','#'},
            }, 'S', 'E', metanull::charmap::EAST);
            maze_node current_node;
            current_node.start = metanull::charmap::position{0,0};
            current_node.start_direction = metanull::charmap::EAST;
            current_node.end = metanull::charmap::position{1,0};
            current_node.end_direction = metanull::charmap::EAST;
            current_node.score = 1;
            current_node.visited = {{0,0},{1,0}};
            current_node.closed = false;
            
            auto next_nodes = m.next_nodes_from(current_node);
            std::cerr << "Expected result: 2 open node {0,0}->{3,0},score=3; {0,0}->{2,1},score=1003" << std::endl;
            std::cerr << "next_nodes.size(): " << next_nodes.size() << std::endl;
            if (!next_nodes.empty()) {
                for(auto nn : next_nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return next_nodes.size() == 2 
                && next_nodes[0].start == metanull::charmap::position{0,0} 
                && next_nodes[0].start_direction == metanull::charmap::EAST 
                && next_nodes[0].end == metanull::charmap::position{3,0} 
                && next_nodes[0].end_direction == metanull::charmap::EAST 
                && next_nodes[0].score == 3
                && next_nodes[0].visited.size() == 4 
                && next_nodes[0].visited.front() == metanull::charmap::position{0,0} 
                && next_nodes[0].visited.back() == metanull::charmap::position{3,0} 
                && next_nodes[0].turns.size() == 0 
                && !next_nodes[0].closed
                && next_nodes[1].start == metanull::charmap::position{0,0} 
                && next_nodes[1].start_direction == metanull::charmap::EAST 
                && next_nodes[1].end == metanull::charmap::position{2,1} 
                && next_nodes[1].end_direction == metanull::charmap::SOUTH
                && next_nodes[1].score == 1003
                && next_nodes[1].visited.size() == 4 
                && next_nodes[1].visited.front() == metanull::charmap::position{0,0} 
                && next_nodes[1].visited.back() == metanull::charmap::position{2,1} 
                && next_nodes[1].turns.size() == 1 
                && next_nodes[1].turns.front() == metanull::charmap::position{2,0} 
                && !next_nodes[1].closed;
        }},
        {"next_nodes-simple-exit", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'S','.','.','.','E'},
                {'#','#','.','#','#'},
                {'#','#','.','#','#'},
            }, 'S', 'E', metanull::charmap::EAST);
            maze_node current_node;
            current_node.start = metanull::charmap::position{0,0};
            current_node.start_direction = metanull::charmap::EAST;
            current_node.end = metanull::charmap::position{3,0};
            current_node.end_direction = metanull::charmap::EAST;
            current_node.score = 3;
            current_node.visited = {{0,0},{1,0},{2,0},{3,0}};
            current_node.closed = false;
            
            auto next_nodes = m.next_nodes_from(current_node);
            std::cerr << "Expected result: 1 closed node {0,0}->{4,0},score=5" << std::endl;
            std::cerr << "next_nodes.size(): " << next_nodes.size() << std::endl;
            if (!next_nodes.empty()) {
                for(auto nn : next_nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return next_nodes.size() == 1 
                && next_nodes[0].start == metanull::charmap::position{0,0} 
                && next_nodes[0].start_direction == metanull::charmap::EAST 
                && next_nodes[0].end == metanull::charmap::position{4,0} 
                && next_nodes[0].end_direction == metanull::charmap::EAST 
                && next_nodes[0].score == 4
                && next_nodes[0].visited.size() == 5 
                && next_nodes[0].visited.front() == metanull::charmap::position{0,0} 
                && next_nodes[0].visited.back() == metanull::charmap::position{4,0} 
                && next_nodes[0].turns.size() == 0 
                && next_nodes[0].closed;
        }},
        {"next_nodes-simple-deadend", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'S','.','.','.','E'},
                {'#','#','.','#','#'},
                {'#','#','.','#','#'},
            }, 'S', 'E', metanull::charmap::EAST);
            maze_node current_node;
            current_node.start = metanull::charmap::position{0,0};
            current_node.start_direction = metanull::charmap::EAST;
            current_node.end = metanull::charmap::position{2,1};
            current_node.end_direction = metanull::charmap::SOUTH;
            current_node.score = 1003;
            current_node.visited = {{0,0},{1,0},{2,0},{2,1}};
            current_node.turns = {{2,0}};
            current_node.closed = false;
            
            auto next_nodes = m.next_nodes_from(current_node);
            std::cerr << "Expected result: 0 nodes (because of a dead end)" << std::endl;
            std::cerr << "next_nodes.size(): " << next_nodes.size() << std::endl;
            if (!next_nodes.empty()) {
                for(auto nn : next_nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return next_nodes.size() == 0;
        }},
        {"next_nodes-simple-loop", []() -> bool {
            maze m = maze(metanull::charmap::map{
                {'S','.','.','.','E'},
                {'#','#','.','#','#'},
                {'.','.','.','#','#'},
                {'.','#','.','#','#'},
                {'.','.','.','#','#'},
            }, 'S', 'E', metanull::charmap::EAST);
            maze_node current_node;
            current_node.start = metanull::charmap::position{0,0};
            current_node.start_direction = metanull::charmap::EAST;
            current_node.end = metanull::charmap::position{2,3};
            current_node.end_direction = metanull::charmap::SOUTH;
            current_node.score = 1005;
            current_node.visited = {{0,0},{1,0},{2,0},{2,1},{2,2},{2,3}};
            current_node.turns = {{2,0}};
            current_node.closed = false;
            
            auto next_nodes = m.next_nodes_from(current_node);
            std::cerr << "Expected result: 0 nodes (because of a loop)" << std::endl;
            std::cerr << "next_nodes.size(): " << next_nodes.size() << std::endl;
            if (!next_nodes.empty()) {
                for(auto nn : next_nodes) {
                    static int counter = 0;
                    std::cerr << "Node " << counter++ << ":" << std::endl;
                    std::cerr << nn << std::endl;
                }
            }
            return next_nodes.size() == 0;
        }},
        */
    };

    auto it = tests.find(test_name);
    if (it != tests.end()) {
        if(!it->second()) {
            std::cerr << "Test failed: " << test_name << std::endl;
            return 1;
        }
        return 0;
    } else {
        std::cerr << "Unknown test: " << test_name << ". Options are: " << std::endl;
        for (const auto& test : tests) {
            std::cout << "- " << test.first << std::endl;
        }
        return 1;
    }
}
