#include "../charmap.h"

#ifndef ARGV2_EQUALS
    #define ARGV2_EQUALS(x) (argc == 2 && 0 == strcmp(argv[1], x))
#endif

#include <functional>
#include <map>

int main(int argc, char ** argv, char ** envp) {
    if(argc != 2) {
        std::cerr << "Usage: test_charmap <test>" << std::endl;
        return 1;
    }

    metanull::charmap::map m = {
        {'+','^','^','^','+'},
        {'<','.','.','.','>'},
        {'<','.','o','.','>'},
        {'<','.','.','.','>'},
        {'+','v','v','v','+'}
    };

    std::string test_name = argv[1];
    std::map<std::string, std::function<bool()>> tests = {
        {"char_find", [m]() -> bool {
            std::cerr << "Expected result: [4] - for expression char_find(m,'+')" << std::endl;
            std::cerr << "Expected result: [1] - for expression char_find(m,'o')" << std::endl;
            std::cerr << "Expected result: [8] - for expression char_find(m,'.')" << std::endl;
            
            auto n = metanull::charmap::char_find(m,'+');
            if(n.size() != 4) {
                return false;
            }
            n = metanull::charmap::char_find(m,'o');
            if(n.size() != 1) {
                return false;
            }
            n = metanull::charmap::char_find(m,'.');
            if(n.size() != 8) {
                return false;
            }
            return true;
        }},
        {"char_find-direction-NORTH", [m]() -> bool {
            std::cerr << "Expected result: [3] all in top most row - for expression char_find(m,'^')" << std::endl;
            auto n = metanull::charmap::char_find(m,'^');
            for(auto o : n) {
                std::cerr << o.first << ',' << o.second << std::endl;
            }
            return n.size() == 3 && std::find_if(n.begin(), n.end(), [](metanull::charmap::index i) { return i.second != 0; }) == n.end();
        }},
        {"char_find-direction-SOUTH", [m]() -> bool {
            std::cerr << "Expected result: [3] all in top most row - for expression char_find(m,'v')" << std::endl;
            auto n = metanull::charmap::char_find(m,'v');
            for(auto o : n) {
                std::cerr << o.first << ',' << o.second << std::endl;
            }
            return n.size() == 3 && std::find_if(n.begin(), n.end(), [](metanull::charmap::index i) { return i.second != 4; }) == n.end();
        }},
        {"char_find-direction-WEST", [m]() -> bool {
            std::cerr << "Expected result: [3] all in left most column - for expression char_find(m,'<')" << std::endl;
            auto n = metanull::charmap::char_find(m,'<');
            for(auto o : n) {
                std::cerr << o.first << ',' << o.second << std::endl;
            }
            return n.size() == 3 && std::find_if(n.begin(), n.end(), [](metanull::charmap::index i) { return i.first != 0; }) == n.end();
        }},
        {"char_find-direction-EAST", [m]() -> bool {
            std::cerr << "Expected result: [3] all in right most column - for expression char_find(m,'>')" << std::endl;
            auto n = metanull::charmap::char_find(m,'>');
            for(auto o : n) {
                std::cerr << o.first << ',' << o.second << std::endl;
            }
            return n.size() == 3 && std::find_if(n.begin(), n.end(), [](metanull::charmap::index i) { return i.first != 4; }) == n.end();
        }},
        {"subset-top_left", [m]() -> bool {
            std::cerr << "Expected result: 3x3 - for expression subset(m,{0,0},3)" << std::endl;
            std::cerr << "+^^" << std::endl;
            std::cerr << "<..>" << std::endl;
            std::cerr << "<.o>" << std::endl;
            
            auto n = metanull::charmap::subset(m,{0,0},3);
            return n.size() == 3 && n[0][0] == '+' && n[0][2] == '^' && n[2][0] == '<' && n[2][2] == 'o';
        }},
        {"subset-top_right", [m]() -> bool {
            std::cerr << "Expected result: 3x3 - for expression subset(m,{2,0},3)" << std::endl;
            std::cerr << "^^+" << std::endl;
            std::cerr << "..>" << std::endl;
            std::cerr << "0.>" << std::endl;
            
            auto n = metanull::charmap::subset(m,{2,0},3);
            return n.size() == 3 && n[0][0] == '^' && n[0][2] == '+' && n[2][0] == 'o' && n[2][2] == '>';
        }},
        {"subset-bottom_left", [m]() -> bool {
            std::cerr << "Expected result: 3x3 - for expression subset(m,{0,2},3)" << std::endl;
            std::cerr << "<.o" << std::endl;
            std::cerr << "<.." << std::endl;
            std::cerr << "+vv" << std::endl;
            
            auto n = metanull::charmap::subset(m,{0,2},3);
            return n.size() == 3 && n[0][0] == '<' && n[0][2] == 'o' && n[2][0] == '+' && n[2][2] == 'v';
        }},
        {"subset-bottom_right", [m]() -> bool {
            std::cerr << "Expected result: 3x3 - for expression subset(m,{2,2},3)" << std::endl;
            std::cerr << "o.>" << std::endl;
            std::cerr << "..>" << std::endl;
            std::cerr << "vv+" << std::endl;
            
            auto n = metanull::charmap::subset(m,{2,2},3);
            return n.size() == 3 && n[0][0] == 'o' && n[0][2] == '>' && n[2][0] == 'v' && n[2][2] == '+';
        }},
        {"subset-out_of_bounds", [m]() -> bool {
            std::cerr << "Expected result: [] - for expression subset(m,{3,3},3)" << std::endl;
            
            auto n = metanull::charmap::subset(m,{3,3},3);
            return n.size() == 0;
        }},
        {"subset-out_of_bounds-truncate", [m]() -> bool {
            std::cerr << "Expected result: 2x2 - for expression subset(m,{3,3},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS)" << std::endl;
            std::cerr << ".>" << std::endl;
            std::cerr << "v+" << std::endl;
            
            auto n = metanull::charmap::subset(m,{3,3},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return n.size() == 2 && n[0].size() == 2 && n[0][0] == '.' && n[0][1] == '>' && n[1][0] == 'v' && n[1][1] == '+';
        }},
        {"subset-out_of_bounds-truncate-empty", [m]() -> bool {
            std::cerr << "Expected result: [] - for expression subset(m,{6,6},3)" << std::endl;
            std::cerr << "Expected result: [] - for expression subset(m,{6,6},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS)" << std::endl;
            
            auto o = metanull::charmap::subset(m,{6,6},3);
            auto n = metanull::charmap::subset(m,{6,6},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return n.size() == 0 && o.size() == 0;
        }},
        {"subset_matches-one", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{2,2},{{'o'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{2,2},{
                {'o'}}
            );
            return n;
        }},
        {"subset_matches-not-one", [m]() -> bool {
            std::cerr << "Expected result: false - for expression subset_matches(m,{2,2},{{'x'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{2,2},{
                {'x'}}
            );
            return !n;
        }},
        {"subset_matches-one-wild_character", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{2,2},{{'\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{2,2},{
                {'\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n;
        }},
        {"subset_matches-top_left-3-2", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{0,0},{{'+','^','^'},{'<','.','.'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{0,0},{
                {'+','^','^'},
                {'<','.','.'}}
            );
            return n;
        }},
        {"subset_matches-not-top_left-3-2", [m]() -> bool {
            std::cerr << "Expected result: false - for expression subset_matches(m,{0,0},{{'x','x','x'},{'x','x','x'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{0,0},{
                {'x','x','x'},
                {'x','x','x'}}
            );
            return !n;
        }},
        {"subset_matches-top_left-3-2-wild_character", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{0,0},{{'+','^','^'},{'<','\\0',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{0,0},{
                {'+','^','^'},
                {'<','\0',0}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n;
        }},
        {"subset_matches-top_right-2-3", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{3,0},{{'^','+'},{'.','>'},{'.','>'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{3,0},{
                {'^','+'},
                {'.','>'},
                {'.','>'}}
            );
            return n;
        }},
        {"subset_matches-top_right-2-3-wild_character", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{3,0},{{'^','+'},{'\0','>'},{0,'>'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{3,0},{
                {'^','+'},
                {'\0','>'},
                {0,'>'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n;
        }},
        {"subset_matches-bottom_left-2-3", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{0,2},{{'<','.'},{'<','.'},{'v','+'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{0,2},{
                {'<','.'},
                {'<','.'},
                {'+','v'}}
            );
            return n;
        }},
        {"subset_matches-bottom_left-2-3-wild_character", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{0,2},{{'<',0},{'<','\\0'},{'v','+'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{0,2},{
                {'<',0},
                {'<','\0'},
                {'+','v'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n;
        }},
        {"subset_matches-bottom_right-3-2", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{2,3},{{'.','.','>'},{'v','v','+'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{2,3},{
                {'.','.','>'},
                {'v','v','+'}}
            );
            return n;
        }},
        {"subset_matches-bottom_right-3-2-wild_character", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{2,3},{{'\\0','\\0','>'},{'v','v','+'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{2,3},{
                {'\0','\0','>'},
                {'v','v','+'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n;
        }},
        {"subset_matches-bottom_middle-3-3", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{1,1},{{'.','.','.'},{'.','o','.'},{'.','.','.'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{1,1},{
                {'.','.','.'},
                {'.','o','.'},
                {'.','.','.'}}
            );
            return n;
        }},
        {"subset_matches-not-bottom_middle-3-3", [m]() -> bool {
            std::cerr << "Expected result: false - for expression subset_matches(m,{1,1},{{'.','.','.'},{'.','x','.'},{'.','.','.'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{1,1},{
                {'.','.','.'},
                {'.','x','.'},
                {'.','.','.'}}
            );
            return !n;
        }},
        {"subset_matches-bottom_middle-3-3-wild_character", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{1,1},{{0,0,0},{'\\0','o','\\0'},{'\\0','\\0','\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{1,1},{
                {0,0,0},
                {'\0','o','\0'},
                {'\0','\0','\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n;
        }},
        {"subset_matches-not-bottom_middle-3-3-wild_character", [m]() -> bool {
            std::cerr << "Expected result: false - for expression subset_matches(m,{1,1},{{0,0,0},{'\\0','x','\\0'},{'\\0','\\0','\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{1,1},{
                {0,0,0},
                {'\0','x','\0'},
                {'\0','\0','\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return !n;
        }},
        {"subset_matches-out_of_bounds", [m]() -> bool {
            std::cerr << "Expected result: false - for expression subset_matches(m,{5,5},{{'.'}})" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{5,5},{
                {'.'}}
            );
            return !n;
        }},
        {"subset_matches-out_of_bounds-wild_character", [m]() -> bool {
            std::cerr << "Expected result: false - for expression subset_matches(m,{5,5},{{'\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{5,5},{
                {'\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return !n;
        }},
        {"subset_matches-out_of_bounds-truncate", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{4,4},{{'+','x'},{'x','x'}},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{4,4},{
                {'+','x'},
                {'x','x'}}
            ,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return n;
        }},
        {"subset_matches-out_of_bounds-truncate-wild_character", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{4,4},{{'\\0','x'},{'x','x'}},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS | metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_matches(m,{4,4},{
                {'\0','x'},
                {'x','x'}}
            ,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS | metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n;
        }},
        {"subset_quick_matches-one", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{2,2},{{'o'}})" << std::endl;
            
            auto n = metanull::charmap::subset_quick_matches(m,{2,2},{
                {'o'}}
            );
            return n;
        }},
        {"subset_quick_matches-not-one", [m]() -> bool {
            std::cerr << "Expected result: false - for expression subset_matches(m,{2,2},{{'x'}})" << std::endl;
            
            auto n = metanull::charmap::subset_quick_matches(m,{2,2},{
                {'x'}}
            );
            return !n;
        }},
        {"subset_quick_matches-3x3", [m]() -> bool {
            std::cerr << "Expected result: true - for expression subset_matches(m,{2,2},{{'o'}})" << std::endl;
            
            auto n = metanull::charmap::subset_quick_matches(m,{1,1},{
                {'.','.','.'},
                {'.','o','.'},
                {'.','.','.'}}
            );
            return n;
        }},
        {"subset_quick_matches-not-3x3", [m]() -> bool {
            std::cerr << "Expected result: false - for expression subset_matches(m,{2,2},{{'x'}})" << std::endl;
            
            auto n = metanull::charmap::subset_quick_matches(m,{1,1},{
                {'.','.','.'},
                {'.','x','.'},
                {'.','.','.'}}
            );
            return !n;
        }},
        {"subset_quick_matches-out_of_bounds", [m]() -> bool {
            std::cerr << "Expected result: throws out_of_range - for expression subset_matches(m,{5,5},{{'.'}})" << std::endl;
            
            try {
                auto n = metanull::charmap::subset_quick_matches(m,{3,3},{
                    {'.','.','.'},
                    {'.','o','.'},
                    {'.','.','.'}}
                );
            } catch(std::out_of_range e) {
                return true;
            }
            return false;
        }},
        
        {"subset_find-one", [m]() -> bool {
            std::cerr << "Expected result: [{2,2}] - for expression subset_find(m,{{'o'}})" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'o'}}
            );
            return n.size() == 1 && std::find_if(n.begin(), n.end(), [](metanull::charmap::index i) { return i == metanull::charmap::index{2,2}; }) != n.end();
        }},
        {"subset_find-one-wild_character", [m]() -> bool {
            std::cerr << "Expected result: [{0,0},{1,0},{2,0},...{4,4}] - for expression subset_find(m,{{'\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n.size() == 25 && std::find_if(n.begin(), n.end(), [](metanull::charmap::index i) { return i == metanull::charmap::index{2,2}; }) != n.end();
        }},
        {"subset_find-horizontal", [m]() -> bool {
            std::cerr << "Expected result: [{0,1},{1,1}] - for expression subset_find(m,{{'<','.','.'}})" << std::endl;
            std::cerr << "Expected result: [{1,4},{2,4}] - for expression subset_find(m,{{'v','v'}})" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'<','.','.'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'v','v'}}
            );
            return n.size() == 2 && o.size() == 2;
        }},
        {"subset_find-horizontal-wild_character", [m]() -> bool {
            std::cerr << "Expected result: [{0,1},{1,1},{1,2}] - for expression subset_find(m,{{'<','.','\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            std::cerr << "Expected result: [{1,4},{2,4},{3,4}] - for expression subset_find(m,{{'v','\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'<','.','\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            auto o = metanull::charmap::subset_find(m,{
                {'v','\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n.size() == 3 && o.size() == 3;
        }},
        {"subset_find-vertical", [m]() -> bool {
            std::cerr << "Expected result: [{1,0},{3,0}] - for expression subset_find(m,{{'^'},{'.'},{'.'}})" << std::endl;
            std::cerr << "Expected result: [{0,1},{0,2}] - for expression subset_find(m,{{'<'},{'<'}})" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'^'},
                {'.'},
                {'.'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'<'},
                {'<'}}
            );
            return n.size() == 2 && o.size() == 2;
        }},
        {"subset_find-vertical-wild_character", [m]() -> bool {
            std::cerr << "Expected result: [{1,0},{2,0},{3,0}] - for expression subset_find(m,{{'^'},{'.'},{'\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            std::cerr << "Expected result: [{0,1},{0,2},{0,3}] - for expression subset_find(m,{{'<'},{'\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'^'},
                {'.'},
                {'\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            auto o = metanull::charmap::subset_find(m,{
                {'<'},
                {'\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n.size() == 3 && o.size() == 3;
        }},
        {"subset_find-rect", [m]() -> bool {
            std::cerr << "Expected result: [{0,1},{0,2}] - for expression subset_find(m,{{'^','^'},{'.','.'}})" << std::endl;
            std::cerr << "Expected result: [{0,0},{1,0}] - for expression subset_find(m,{{'<','.'},{'<','.'}})" << std::endl;
            std::cerr << "Expected result: [{1,3},{2,3}] - for expression subset_find(m,{{'.','.'},{'v','v'}})" << std::endl;
            std::cerr << "Expected result: [{3,1},{3,2}] - for expression subset_find(m,{{'.','>'},{'.','>'}})" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'^','^'},
                {'.','.'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'<','.'},
                {'<','.'}}
            );
            auto p = metanull::charmap::subset_find(m,{
                {'.','.'},
                {'v','v'}}
            );
            auto q = metanull::charmap::subset_find(m,{
                {'.','>'},
                {'.','>'}}
            );
            return n.size() == 2 && o.size() == 2 && p.size() == 2 && q.size() == 2;
        }},
        {"subset_find-rect-wild_character", [m]() -> bool {
            std::cerr << "Expected result: [{0,1},{0,2}] - for expression subset_find(m,{{'^','^'},{'.','.'},{0,'\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            std::cerr << "Expected result: [{1,0},{2,0}] - for expression subset_find(m,{{'<','.',0},{'<','.','\\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            std::cerr << "Expected result: [{1,3},{2,3}] - for expression subset_find(m,{{0,'\\0'},{'.','.'},{'v','v'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            std::cerr << "Expected result: [{3,1},{3,2}] - for expression subset_find(m,{{0,'.'},'\\0','>'},{'\\0','.','>'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'^','^'},
                {'.','.'},
                {0,'\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            auto o = metanull::charmap::subset_find(m,{
                {'<','.',0},
                {'<','.','\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            auto p = metanull::charmap::subset_find(m,{
                {0,'\0'},
                {'.','.'},
                {'v','v'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            auto q = metanull::charmap::subset_find(m,{
                {0,'.','>'},
                {'\0','.','>'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n.size() == 2 && o.size() == 2 && p.size() == 2 && q.size() == 2;
        }},
        {"subset_find-whole", [m]() -> bool {
            std::cerr << "Expected result: [{0,0}] - for expression subset_find(m,{{'+','^','^','^','+'},{'<','.','.','.','>'},{'<','.','o','.','>'},{...}})" << std::endl;
            std::cerr << "Expected result: [] - for expression subset_find(m,{{'+','^','^','^','+'},{'<','.','.','.','>'},{'<','.','x','.','>'},{...}})" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'+','^','^','^','+'},
                {'<','.','.','.','>'},
                {'<','.','o','.','>'},
                {'<','.','.','.','>'},
                {'+','v','v','v','+'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'+','^','^','^','+'},
                {'<','.','.','.','>'},
                {'<','.','x','.','>'},
                {'<','.','.','.','>'},
                {'+','v','v','v','+'}}
            );
            return n.size() == 1 && o.size() == 0;
        }},
        {"subset_find-whole-wild_character", [m]() -> bool {
            std::cerr << "Expected result: [{0,0}] - for expression subset_find(m,{{'\\0','^','^','^','\\0'},{'<','\\0','.','\\0','>'},{'<','.','o','.','>'},{...}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            std::cerr << "Expected result: [] - for expression subset_find(m,{{'\\0','^','^','^','\\0'},{'<','\\0','.','\\0','>'},{'<','.','x','.','>'},{...}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            auto n = metanull::charmap::subset_find(m,{
                {'\0','^','^','^','\0'},
                {'<','\0','.','\0','>'},
                {'<','.','o','.','>'},
                {'<','\0','.','\0','>'},
                {'\0','v','v','v','\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            auto o = metanull::charmap::subset_find(m,{
                {'\0','^','^','^','\0'},
                {'<','\0','.','\0','>'},
                {'<','.','x','.','>'},
                {'<','\0','.','\0','>'},
                {'\0','v','v','v','\0'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return n.size() == 1 && o.size() == 0;
        }},

        {"subset_replace", [m]() -> bool {
            std::cerr << "Expected result: 5x5 - for expression subset_replace(m,{0,0},{{'H','E','L','L','O'},{'W','O','R','L','D'}})" << std::endl;
            std::cerr << "HELLO" << std::endl;
            std::cerr << "WORLD" << std::endl;
            std::cerr << "<.o.>" << std::endl;
            std::cerr << "<...>" << std::endl;
            std::cerr << "+vvv+" << std::endl;
            
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{0,0},{
                {'H','E','L','L','O'},
                {'W','O','R','L','D'}}
            );
            return std::string(mc[0].begin(),mc[0].end()) == "HELLO" && std::string(mc[1].begin(),mc[1].end()) == "WORLD" && n == 10;
        }},
        {"subset_replace-wild_character", [m]() -> bool {
            std::cerr << "Expected result: 5x5 - for expression subset_replace(m,{0,0},{{'H','E','L','L','\\0'},{'W','\\0','R','L','D'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            std::cerr << "HELL+" << std::endl;
            std::cerr << "W.RLD" << std::endl;
            std::cerr << "<.o.>" << std::endl;
            std::cerr << "<...>" << std::endl;
            std::cerr << "+vvv+" << std::endl;
            
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{0,0},{
                {'H','E','L','L','\0'},
                {'W','\0','R','L','D'}}
            ,metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return std::string(mc[0].begin(),mc[0].end()) == "HELL+" && std::string(mc[1].begin(),mc[1].end()) == "W.RLD" && n == 8;
        }},
        {"subset_replace-out_of_bounds", [m]() -> bool {
            std::cerr << "Expected result: [No change] - for expression subset_replace(m,{1,0},{{'H','E','L','L','O'},{'W','O','R','L','D'}})" << std::endl;
            
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{1,0},{
                {'H','E','L','L','O'},
                {'W','O','R','L','D'}}
            );
            return std::string(mc[0].begin(),mc[0].end()) == "+^^^+" && std::string(mc[1].begin(),mc[1].end()) == "<...>" && n == 0;
        }},
        {"subset_replace-out_of_bounds-truncate", [m]() -> bool {
            std::cerr << "Expected result: 5x5 - for expression subset_replace(m,{1,0},{{'H','E','L','L','O'},{'W','O','R','L','D'}},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS)" << std::endl;
            std::cerr << "+HELL" << std::endl;
            std::cerr << "<WORL" << std::endl;
            std::cerr << "<.o.>" << std::endl;
            std::cerr << "<...>" << std::endl;
            std::cerr << "+vvv+" << std::endl;
            
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{1,0},{
                {'H','E','L','L','O'},
                {'W','O','R','L','D'}}
            ,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return std::string(mc[0].begin(),mc[0].end()) == "+HELL" && std::string(mc[1].begin(),mc[1].end()) == "<WORL" && n == 8;
        }},
        {"subset_replace-out_of_bounds-truncate-wild_character", [m]() -> bool {
            std::cerr << "Expected result: 5x5 - for expression subset_replace(m,{1,0},{{'H','E','L','L','\\0'},{'W','\\0','R','L','D'}},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS | metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            std::cerr << "+HELL" << std::endl;
            std::cerr << "<W.RL" << std::endl;
            std::cerr << "<.o.>" << std::endl;
            std::cerr << "<...>" << std::endl;
            std::cerr << "+vvv+" << std::endl;
            
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{1,0},{
                {'H','E','L','L','\0'},
                {'W','\0','R','L','D'}}
            ,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS | metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            return std::string(mc[0].begin(),mc[0].end()) == "+HELL" && std::string(mc[1].begin(),mc[1].end()) == "<W.RL" && n == 7;
        }},
        {"subset_quick_replace", [m]() -> bool {
            std::cerr << "Expected result: 5x5 - for expression subset_replace(m,{0,0},{{'H','E','L','L','O'},{'W','O','R','L','D'}})" << std::endl;
            std::cerr << "HELLO" << std::endl;
            std::cerr << "WORLD" << std::endl;
            std::cerr << "<.o.>" << std::endl;
            std::cerr << "<...>" << std::endl;
            std::cerr << "+vvv+" << std::endl;
            
            auto mc = m;
            metanull::charmap::subset_quick_replace(mc,{0,0},{
                {'H','E','L','L','O'},
                {'W','O','R','L','D'}}
            );
            return std::string(mc[0].begin(),mc[0].end()) == "HELLO" && std::string(mc[1].begin(),mc[1].end()) == "WORLD";
        }},
        {"subset_quick_replace-out_of_bounds", [m]() -> bool {
            std::cerr << "Expected result: throws out_of_range - for expression subset_replace(m,{1,0},{{'H','E','L','L','O'},{'W','O','R','L','D'}})" << std::endl;
            
            try {
                auto mc = m;
                metanull::charmap::subset_quick_replace(mc,{1,0},{
                    {'H','E','L','L','O'},
                    {'W','O','R','L','D'}}
                );
            } catch (const std::out_of_range & e) {
                return true;
            }
            return false;
        }},
        {"diff_count", []() -> bool {
            std::cerr << "Expected result: 0 - for expression diff_count(m, m)" << std::endl;
            std::cerr << "Expected result: 1 - for expression diff_count(m, m2)" << std::endl;
            std::cerr << "Expected result: 0 - for expression diff_count(m, m3, metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER)" << std::endl;
            
            metanull::charmap::map m = {
            {'+','^','^','^','+'},
            {'<','.','.','.','>'},
            {'<','.','o','.','>'},
            {'<','.','.','.','>'},
            {'+','v','v','v','+'}
            };
            
            metanull::charmap::map m2 = {
            {'+','^','^','^','+'},
            {'<','.','.','.','>'},
            {'<','.','x','.','>'},
            {'<','.','.','.','>'},
            {'+','v','v','v','+'}
            };
            
            metanull::charmap::map m3 = {
            {'+','^','^','^','+'},
            {'<','.','.','.','>'},
            {'<','.','\0','.','>'},
            {'<','.','.','.','>'},
            {'+','v','v','v','+'}
            };
            
            auto diff1 = metanull::charmap::diff_count(m, m);
            auto diff2 = metanull::charmap::diff_count(m, m2);
            auto diff3 = metanull::charmap::diff_count(m, m3, metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
            
            return diff1 == 0 && diff2 == 1 && diff3 == 0;
        }},

        {"access-const", [m]() -> bool {
            std::cerr << "Expected result: + - for expression access(m,{0,0})" << std::endl;
            std::cerr << "Expected result: o - for expression access(m,{2,2})" << std::endl;
            std::cerr << "Expected result: + - for expression access(m,{4,4})" << std::endl;
            return metanull::charmap::access(m,{0,0}) == '+' && metanull::charmap::access(m,{2,2}) == 'o' && metanull::charmap::access(m,{4,4}) == '+';
        }},
        {"access-const-exception", [m]() -> bool {
            std::cerr << "Expected result: throws std::out_of_range - for expression access(m,{9,9})" << std::endl;
            try{
                metanull::charmap::access(m,{9,9});
            } catch (const std::out_of_range & e) {
                return true;
            }
            return false;
        }},
        {"access-reference", [m]() -> bool {
            std::cerr << "Expected result: m[2][0] = 'H' - for expression access(m,{0,2}) = 'H'" << std::endl;
            std::cerr << "Expected result: m[2][1] = 'E' - for expression access(m,{1,2}) = 'E'" << std::endl;
            std::cerr << "Expected result: m[2][2] = 'L' - for expression access(m,{2,2}) = 'L'" << std::endl;
            std::cerr << "Expected result: m[2][3] = 'L' - for expression access(m,{3,2}) = 'L'" << std::endl;
            std::cerr << "Expected result: m[2][4] = 'O' - for expression access(m,{4,2}) = 'O'" << std::endl;
            
            auto mc = m;
            metanull::charmap::access(mc,{0,2}) = 'H';
            metanull::charmap::access(mc,{1,2}) = 'E';
            metanull::charmap::access(mc,{2,2}) = 'L';
            metanull::charmap::access(mc,{3,2}) = 'L';
            metanull::charmap::access(mc,{4,2}) = 'O';
            return std::string(mc[2].begin(),mc[2].end()) == "HELLO";
        }},

        {"row_as_string", [m]() -> bool {
            std::cerr << "Expected result: o.> - for expression row_as_string(m,{2,2})" << std::endl;
            return metanull::charmap::row_as_string(m,{2,2}) == "o.>";
        }},
        {"column_as_string", [m]() -> bool {
            std::cerr << "Expected result: o.v - for expression column_as_string(m,{2,2})" << std::endl;
            return metanull::charmap::column_as_string(m,{2,2}) == "o.v";
        }},
        {"transpose", [m]() -> bool {
            std::cerr << "Expected result: 5x5 - for expression transpose(m)" << std::endl;
            std::cerr << "    Original:   ->  Transposed:" << std::endl;
            std::cerr << "" << std::endl;
            std::cerr << "    + ^ ^ ^ +       + < < < +" << std::endl;
            std::cerr << "    < . . . >       ^ . . . v" << std::endl;
            std::cerr << "    < . o . >   ->  ^ . o . v" << std::endl;
            std::cerr << "    < . . . >       ^ . . . v" << std::endl;
            std::cerr << "    + v v v +       + > > > +" << std::endl;
            
            auto n = metanull::charmap::transpose(m);
            return n.size() == 5 && n[0].size() == 5 
                && n[0][0] == '+' && n[0][4] == '+' && n[4][0] == '+' && n[4][4] == '+'
                && n[2][2] == 'o' 
                && n[1][1] == '.' && n[3][1] == '.' && n[1][3] == '.' && n[3][3] == '.'
                && n[2][0] == '^' && n[2][4] == 'v' && n[0][2] == '<' && n[4][2] == '>'
                ;
        }},
        {"neighbours_if", [m]() -> bool {
            std::cerr << "Expected result: 8 indexes - for expression neighbours_if(m,{2,2}, [](char c, metanull::charmap::direction d) { return true; })" << std::endl;
            
            auto n = metanull::charmap::neighbours_if(m,{2,2}, [](char c, metanull::charmap::direction d) { return true; });
            return n.size() == 8;
        }},
        {"neighbours_if-only-NSEW", [m]() -> bool {
            std::cerr << "Expected result: 4 indexes - for expression neighbours_if(m,{2,2}, [](char c, metanull::charmap::direction d) { return d.first == 0 || d.second == 0; })" << std::endl;
            
            auto n = metanull::charmap::neighbours_if(m,{2,2}, [](char c, metanull::charmap::direction d) { return d.first == 0 || d.second == 0; });
            return n.size() == 4;
        }},
        {"neighbours_if-NORTH", [m]() -> bool {
            std::cerr << "Expected result: 1 indexes to the NORTH of current - for expression neighbours_if(m,{1,1}, [](char c, metanull::charmap::direction d) { return d == metanull::charmap::NORTH })" << std::endl;
            
            auto n = metanull::charmap::neighbours_if(m,{1,1}, [](char c, metanull::charmap::direction d) { return d == metanull::charmap::NORTH; });
            auto d = metanull::charmap::translate({1,1},metanull::charmap::NORTH);
            auto dm = metanull::charmap::index(1,2);
            return n.size() == 1 && metanull::charmap::translate({1,1},n[0].first) == d && metanull::charmap::translate({1,1},n[0].first) == dm;
        }},
        {"neighbours_if-SOUTH", [m]() -> bool {
            std::cerr << "Expected result: 1 indexes to the SOUTH of current - for expression neighbours_if(m,{1,1}, [](char c, metanull::charmap::direction d) { return d == metanull::charmap::SOUTH })" << std::endl;
            
            auto n = metanull::charmap::neighbours_if(m,{1,1}, [](char c, metanull::charmap::direction d) { return d == metanull::charmap::SOUTH; });
            auto d = metanull::charmap::translate({1,1},metanull::charmap::SOUTH);
            auto dm = metanull::charmap::index(1,0);
            return n.size() == 1 && metanull::charmap::translate({1,1},n[0].first) == d && metanull::charmap::translate({1,1},n[0].first) == dm;
        }},
        {"neighbours_if-EAST", [m]() -> bool {
            std::cerr << "Expected result: 1 indexes to the EAST of current - for expression neighbours_if(m,{1,1}, [](char c, metanull::charmap::direction d) { return d == metanull::charmap::EAST })" << std::endl;
            
            auto n = metanull::charmap::neighbours_if(m,{1,1}, [](char c, metanull::charmap::direction d) { return d == metanull::charmap::EAST; });
            auto d = metanull::charmap::translate({1,1},metanull::charmap::EAST);
            auto dm = metanull::charmap::index(2,1);
            return n.size() == 1 && metanull::charmap::translate({1,1},n[0].first) == d && metanull::charmap::translate({1,1},n[0].first) == dm;
        }},
        {"neighbours_if-WEST", [m]() -> bool {
            std::cerr << "Expected result: 1 indexes to the WEST of current - for expression neighbours_if(m,{1,1}, [](char c, metanull::charmap::direction d) { return d == metanull::charmap::WEST })" << std::endl;
            
            auto n = metanull::charmap::neighbours_if(m,{1,1}, [](char c, metanull::charmap::direction d) { return d == metanull::charmap::WEST; });
            auto d = metanull::charmap::translate({1,1},metanull::charmap::WEST);
            auto dm = metanull::charmap::index(0,1);
            return n.size() == 1 && metanull::charmap::translate({1,1},n[0].first) == d && metanull::charmap::translate({1,1},n[0].first) == dm;
        }},
        {"neighbours_if-char_val_is", [m]() -> bool {
            std::cerr << "Expected result: 2 indexes - for expression neighbours_if(m,{3,3}, [](char c, metanull::charmap::direction d) { return c == '.'; })" << std::endl;
            
            auto n = metanull::charmap::neighbours_if(m,{3,3}, [](char c, metanull::charmap::direction d) { return c == '.'; });
            return n.size() == 2;
        }},
        {"neighbours_if-char_val_is-no_match", [m]() -> bool {
            std::cerr << "Expected result: [] - for expression neighbours_if(m,{3,3}, [](char c, metanull::charmap::direction d) { return c == '$'; })" << std::endl;
            
            auto n = metanull::charmap::neighbours_if(m,{3,3}, [](char c, metanull::charmap::direction d) { return c == '$'; });
            return n.size() == 0;
        }},
        {"rotate_clockwise", []() -> bool {
            std::cerr << "Expected result: (1, 0) -> (0, 1)" << std::endl;
            std::cerr << "Expected result: (0, 1) -> (-1, 0)" << std::endl;
            std::cerr << "Expected result: (-1, 0) -> (0, -1)" << std::endl;
            std::cerr << "Expected result: (0, -1) -> (1, 0)" << std::endl;
            
            auto d = metanull::charmap::rotate_clockwise({1, 0});
            if (d != metanull::charmap::direction{0, 1}) return false;
            d = metanull::charmap::rotate_clockwise({0, 1});
            if (d != metanull::charmap::direction{-1, 0}) return false;
            d = metanull::charmap::rotate_clockwise({-1, 0});
            if (d != metanull::charmap::direction{0, -1}) return false;
            d = metanull::charmap::rotate_clockwise({0, -1});
            return d == metanull::charmap::direction{1, 0};
        }},
        {"rotate_counter_clockwise", []() -> bool {
            std::cerr << "Expected result: (1, 0) -> (0, -1)" << std::endl;
            std::cerr << "Expected result: (0, -1) -> (-1, 0)" << std::endl;
            std::cerr << "Expected result: (-1, 0) -> (0, 1)" << std::endl;
            std::cerr << "Expected result: (0, 1) -> (1, 0)" << std::endl;
            
            auto d = metanull::charmap::rotate_counter_clockwise({1, 0});
            if (d != metanull::charmap::direction{0, -1}) return false;
            d = metanull::charmap::rotate_counter_clockwise({0, -1});
            if (d != metanull::charmap::direction{-1, 0}) return false;
            d = metanull::charmap::rotate_counter_clockwise({-1, 0});
            if (d != metanull::charmap::direction{0, 1}) return false;
            d = metanull::charmap::rotate_counter_clockwise({0, 1});
            return d == metanull::charmap::direction{1, 0};
        }},
        {"inverse", []() -> bool {
            std::cerr << "Expected result: (1, 0) -> (-1, 0)" << std::endl;
            std::cerr << "Expected result: (0, 1) -> (0, -1)" << std::endl;
            std::cerr << "Expected result: (-1, 0) -> (1, 0)" << std::endl;
            std::cerr << "Expected result: (0, -1) -> (0, 1)" << std::endl;
            
            auto d = metanull::charmap::inverse({1, 0});
            if (d != metanull::charmap::direction{-1, 0}) return false;
            d = metanull::charmap::inverse({0, 1});
            if (d != metanull::charmap::direction{0, -1}) return false;
            d = metanull::charmap::inverse({-1, 0});
            if (d != metanull::charmap::direction{1, 0}) return false;
            d = metanull::charmap::inverse({0, -1});
            return d == metanull::charmap::direction{0, 1};
        }},
        {"operator<<", [m]() -> bool {
            std::cerr << "Expected result: +^^^+\n<...>\n<.o.>\n<...>\n+vvv+\n - for expression operator<<(std::cout, m)" << std::endl;
            std::ostringstream oss;
            oss << m;
            std::stringstream ss;
            ss << "+^^^+" << std::endl << "<...>" << std::endl << "<.o.>" << std::endl << "<...>" << std::endl << "+vvv+" << std::endl;
            return oss.str() == ss.str();
        }},
        {"operator>>", []() -> bool {
            std::cerr << "Expected result: +^^^+\n<...>\n<.o.>\n<...>\n+vvv+\n - for expression operator>>(std::istringstream, m)" << std::endl;
            std::ostringstream oss;
            oss << "+^^^+" << std::endl << "<...>" << std::endl << "<.o.>" << std::endl << "<...>" << std::endl << "+vvv+" << std::endl;
            std::istringstream iss(oss.str());
            metanull::charmap::map m;
            iss >> m;
            return m == metanull::charmap::map{
            {'+','^','^','^','+'},
            {'<','.','.','.','>'},
            {'<','.','o','.','>'},
            {'<','.','.','.','>'},
            {'+','v','v','v','+'}
            };
        }},
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
