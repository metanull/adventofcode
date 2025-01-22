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
        {"subset-top_left", [m]() -> bool {
            auto n = metanull::charmap::subset(m,{0,0},3);
            return n.size() == 3 && n[0][0] == '+' && n[0][2] == '^' && n[2][0] == '<' && n[2][2] == 'o';
        }},
        {"subset-top_right", [m]() -> bool {
            auto n = metanull::charmap::subset(m,{2,0},3);
            return n.size() == 3 && n[0][0] == '^' && n[0][2] == '+' && n[2][0] == 'o' && n[2][2] == '>';
        }},
        {"subset-bottom_left", [m]() -> bool {
            auto n = metanull::charmap::subset(m,{0,2},3);
            return n.size() == 3 && n[0][0] == '<' && n[0][2] == 'o' && n[2][0] == '+' && n[2][2] == 'v';
        }},
        {"subset-bottom_right", [m]() -> bool {
            auto n = metanull::charmap::subset(m,{2,2},3);
            return n.size() == 3 && n[0][0] == 'o' && n[0][2] == '>' && n[2][0] == 'v' && n[2][2] == '+';
        }},
        {"subset-out_of_bounds", [m]() -> bool {
            auto n = metanull::charmap::subset(m,{3,3},3);
            return n.size() == 0;
        }},
        {"subset-out_of_bounds-truncate", [m]() -> bool {
            auto n = metanull::charmap::subset(m,{3,3},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return n.size() == 2 && n[0].size() == 2 && n[0][0] == '.' && n[0][1] == '>' && n[1][0] == 'v' && n[1][1] == '+';
        }},
        {"subset-out_of_bounds-truncate-empty", [m]() -> bool {
            auto o = metanull::charmap::subset(m,{6,6},3);
            auto n = metanull::charmap::subset(m,{6,6},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return n.size() == o.size() == 0;
        }},
        {"subset_matches-one", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,2},{
                {'o'}}
            );
            return !n;
        }},
        {"subset_matches-one-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,2},{
                {'\0'}}
            );
            return !n;
        }},
        {"subset_matches-top_left-3-2", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{0,0},{
                {'+','^','^'},
                {'<','.','.'}}
            );
            return n;
        }},
        {"subset_matches-top_left-3-2-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{0,0},{
                {'+','^','^'},
                {'<','\0',0}}
            );
            return n;
        }},
        {"subset_matches-top_right-2-3", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,0},{
                {'^','+'},
                {'.','>'},
                {'.','>'}}
            );
            return n;
        }},
        {"subset_matches-top_right-2-3-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,0},{
                {'^','+'},
                {'\0','>'},
                {0,'>'}}
            );
            return n;
        }},
        {"subset_matches-bottom_left-2-3", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{0,2},{
                {'<','.'},
                {'<','.'},
                {'+','v'}}
            );
            return n;
        }},
        {"subset_matches-bottom_left-2-3-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{0,2},{
                {'<',0},
                {'<','\0'},
                {'+','v'}}
            );
            return n;
        }},
        {"subset_matches-bottom_right-3-2", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,3},{
                {'.','.','>'},
                {'v','v','+'}}
            );
            return n;
        }},
        {"subset_matches-bottom_middle-3-3", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,3},{
                {'.','.','.'},
                {'.','o','.'},
                {'.','.','.'}}
            );
            return n;
        }},
        {"subset_matches-bottom_middle-3-3-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,3},{
                {0,0,0},
                {'\0','o','\0'},
                {'\0','\0','\0'}}
            );
            return n;
        }},
        {"subset_matches-out_of_bounds", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{4,4},{
                {'.'}}
            );
            return !n;
        }},
        {"subset_matches-out_of_bounds-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{4,4},{
                {'\0'}}
            );
            return !n;
        }},
        {"subset_matches-out_of_bounds-truncate", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{4,4},{
                {'.'}}
            ,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return n;
        }},
        {"subset_matches-out_of_bounds-truncate-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{4,4},{
                {'\0'}}
            ,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return n;
        }},
        
        {"subset_find-one", [m]() -> bool {
            auto n = metanull::charmap::subset_find(m,{
                {'o'}}
            );
            return n.size() == 1 && std::find_if(n.begin(), n.end(), [](metanull::charmap::index i) { return i == metanull::charmap::index{2,2}; }) != n.end();
        }},
        {"subset_find-one-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_find(m,{
                {'\0'}}
            );
            return n.size() == 25 && std::find_if(n.begin(), n.end(), [](metanull::charmap::index i) { return i == metanull::charmap::index{2,2}; }) != n.end();
        }},
        {"subset_find-horizontal", [m]() -> bool {
            auto n = metanull::charmap::subset_find(m,{
                {'<','.','.'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'v','v'}}
            );
            return n.size() == 2 && o.size() == 2;
        }},
        {"subset_find-horizontal-wild_character", [m]() -> bool {
            auto n = metanull::charmap::subset_find(m,{
                {'<','.','\0'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'v','\0'}}
            );
            return n.size() == 3 && o.size() == 3;
        }},
        {"subset_find-vertical", [m]() -> bool {
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
            auto n = metanull::charmap::subset_find(m,{
                {'^'},
                {'.'},
                {'\0'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'<'},
                {'\0'}}
            );
            return n.size() == 3 && o.size() == 3;
        }},
        {"subset_find-rect", [m]() -> bool {
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
            auto n = metanull::charmap::subset_find(m,{
                {'^','^'},
                {'.','.'},
                {0,'\0'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'<','.',0},
                {'<','.','\0'}}
            );
            auto p = metanull::charmap::subset_find(m,{
                {0,'\0'},
                {'.','.'},
                {'v','v'}}
            );
            auto q = metanull::charmap::subset_find(m,{
                {0,'.','>'},
                {'\0','.','>'}}
            );
            return n.size() == 2 && o.size() == 2 && p.size() == 2 && q.size() == 2;
        }},
        {"subset_find-whole", [m]() -> bool {
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
            auto n = metanull::charmap::subset_find(m,{
                {'\0','^','^','^','\0'},
                {'<','\0','.','\0','>'},
                {'<','.','o','.','>'},
                {'<','\0','.','\0','>'},
                {'\0','v','v','v','\0'}}
            );
            auto o = metanull::charmap::subset_find(m,{
                {'\0','^','^','^','\0'},
                {'<','\0','.','\0','>'},
                {'<','.','x','.','>'},
                {'<','\0','.','\0','>'},
                {'\0','v','v','v','\0'}}
            );
            return n.size() == 1 && o.size() == 0;
        }},

        {"subset_replace", [m]() -> bool {
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{0,0},{
                {'H','E','L','L','O'},
                {'W','O','R','L','D'}}
            );
            return std::string(mc[0].begin(),mc[0].end()) == "HELLO" && std::string(mc[1].begin(),mc[1].end()) == "WORLD" && n == 10;
        }},
        {"subset_replace-wild_character", [m]() -> bool {
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{0,0},{
                {'H','E','L','L','\0'},
                {'W','\0','R','L','D'}}
            );
            return std::string(mc[0].begin(),mc[0].end()) == "HELL+" && std::string(mc[1].begin(),mc[1].end()) == "W.RLD" && n == 8;
        }},
        {"subset_replace-out_of_bounds", [m]() -> bool {
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{1,0},{
                {'H','E','L','L','O'},
                {'W','O','R','L','D'}}
            );
            return std::string(mc[0].begin(),mc[0].end()) == "+^^^+" && std::string(mc[1].begin(),mc[1].end()) == "<...>" && n == 0;
        }},
        {"subset_replace-out_of_bounds-truncate", [m]() -> bool {
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{1,0},{
                {'H','E','L','L','O'},
                {'W','O','R','L','D'}}
            );
            return std::string(mc[0].begin(),mc[0].end()) == "+HELL" && std::string(mc[1].begin(),mc[1].end()) == "<WORL" && n == 8;
        }},
        {"subset_replace-out_of_bounds-truncate-wild_character", [m]() -> bool {
            auto mc = m;
            auto n = metanull::charmap::subset_replace(mc,{1,0},{
                {'H','E','L','L','\0'},
                {'W','\0','R','L','D'}}
            );
            return std::string(mc[0].begin(),mc[0].end()) == "+HELL" && std::string(mc[1].begin(),mc[1].end()) == "<W.RL" && n == 7;
        }},

        {"access-const", [m]() -> bool {
            return metanull::charmap::access(m,{0,0}) == '+' && metanull::charmap::access(m,{2,2}) == 'o' && metanull::charmap::access(m,{4,4}) == '+';
        }},
        {"access-const-exception", [m]() -> bool {
            try{
                metanull::charmap::access(m,{9,9});
            } catch (const std::out_of_range & e) {
                return true;
            }
            return false;
        }},
        {"access-reference", [m]() -> bool {
            auto mc = m;
            metanull::charmap::access(mc,{0,2}) = 'H';
            metanull::charmap::access(mc,{1,2}) = 'E';
            metanull::charmap::access(mc,{2,2}) = 'L';
            metanull::charmap::access(mc,{3,2}) = 'L';
            metanull::charmap::access(mc,{4,2}) = 'O';
            return std::string(mc[0].begin(),mc[2].end()) == "HELLO";
        }},

        {"row_as_string", [m]() -> bool {
            return metanull::charmap::row_as_string(m,{2,2}) == "o.>";
        }},
        {"column_as_string", [m]() -> bool {
            return metanull::charmap::column_as_string(m,{2,2}) == "o.v";
        }},
        {"transpose", [m]() -> bool {
            /*
             * Original:        Transposed:
                + ^ ^ ^ +       + < < < +
                < . . . >       ^ . . . v
                < . o . >       ^ . o . v
                < . . . >       ^ . . . v
                + v v v +       + > > > +
             */
            auto n = metanull::charmap::transpose(m);
            return n.size() == 5 && n[0].size() == 5 
                && n[0][0] == '+' && n[0][4] == '+' && n[4][0] == '+' && n[4][4] == '+'
                && n[2][2] == 'o' 
                && n[1][1] == '.' && n[3][1] == '.' && n[1][3] == '.' && n[3][3] == '.'
                && n[2][0] == '<' && n[2][4] == '>' && n[0][2] == '^' && n[4][2] == 'v'
                ;
        }},
        {"neighbours_if", [m]() -> bool {
            auto n = metanull::charmap::neighbours_if(m,{2,2}, [](char c, metanull::charmap::direction d) { return true; });
            return n.size() == 8;
        }},
        {"neighbours_if-only-NSEW", [m]() -> bool {
            auto n = metanull::charmap::neighbours_if(m,{2,2}, [](char c, metanull::charmap::direction d) { return d.first == 0 || d.second == 0; });
            return n.size() == 4;
        }},
        {"neighbours_if-char_val_is", [m]() -> bool {
            auto n = metanull::charmap::neighbours_if(m,{3,3}, [](char c, metanull::charmap::direction d) { return c == '.'; });
            return n.size() == 2;
        }},
        {"neighbours_if-char_val_is-no_match", [m]() -> bool {
            auto n = metanull::charmap::neighbours_if(m,{3,3}, [](char c, metanull::charmap::direction d) { return c == '$'; });
            return n.size() == 0;
        }}
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

/*

     // TESTS

    metanull::charmap::map m = {
        {'+','^','^','^','+'},
        {'<','.','.','.','>'},
        {'<','.','o','.','>'},
        {'<','.','.','.','>'},
        {'+','v','v','v','+'}

        + < < < +
        ^ . . . v
        ^ . o . v
        ^ . . . v
        + > > > +

    };

    auto n = metanull::charmap::char_find(m,'+');  
    n = metanull::charmap::char_find(m,'o');  
    n = metanull::charmap::char_find(m,'.');  
    n = metanull::charmap::char_find(m,'\0');  
    auto o = metanull::charmap::subset(m,{0,0},3); 
    o = metanull::charmap::subset(m,{2,0},3); 
    o = metanull::charmap::subset(m,{0,2},3); 
    o = metanull::charmap::subset(m,{2,2},3); 
    o = metanull::charmap::subset(m,{3,3},3); 
    o = metanull::charmap::subset(m,{4,4},3); 
    o = metanull::charmap::subset(m,{9,9},3); 
    o = metanull::charmap::subset(m,{3,3},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    o = metanull::charmap::subset(m,{4,4},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    o = metanull::charmap::subset(m,{9,9},3,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    o = metanull::charmap::subset(m,{0,0},{4,4}); 
    o = metanull::charmap::subset(m,{2,0},{4,4}); 
    o = metanull::charmap::subset(m,{0,2},{4,4}); 
    o = metanull::charmap::subset(m,{2,2},{4,4}); 
    o = metanull::charmap::subset(m,{3,3},{4,4}); 
    o = metanull::charmap::subset(m,{3,3},{5,5}); 
    o = metanull::charmap::subset(m,{3,3},{6,6}); 
    o = metanull::charmap::subset(m,{4,4},{6,6}); 
    o = metanull::charmap::subset(m,{3,3},{4,4},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    o = metanull::charmap::subset(m,{3,3},{5,5},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    o = metanull::charmap::subset(m,{3,3},{6,6},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    o = metanull::charmap::subset(m,{4,4},{6,6},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    o = metanull::charmap::subset(m,{3,3},{2,2}); 
    o = metanull::charmap::subset(m,{3,3},{3,3}); 
    o = metanull::charmap::subset(m,{9,9},{9,9}); 
    o = metanull::charmap::subset(m,{9,9},{9,9},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    auto p = metanull::charmap::subset_matches(m,{0,0},{{'+','^'},{'<','.'}}); 
    p = metanull::charmap::subset_matches(m,{0,0},{{'+','^'},{0,0}}); 
    p = metanull::charmap::subset_matches(m,{0,0},{{'+','^'},{0,0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    p = metanull::charmap::subset_matches(m,{3,0},{{'^','+'},{'.','>'}}); 
    p = metanull::charmap::subset_matches(m,{3,0},{{'^','+'},{0,0}}); 
    p = metanull::charmap::subset_matches(m,{3,0},{{'^','+'},{0,0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    p = metanull::charmap::subset_matches(m,{0,3},{{'<','.'},{'+','v'}}); 
    p = metanull::charmap::subset_matches(m,{0,3},{{'<','.'},{0,0}}); 
    p = metanull::charmap::subset_matches(m,{0,3},{{'<','.'},{0,0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    p = metanull::charmap::subset_matches(m,{3,3},{{'.','>'},{'v','+'}}); 
    p = metanull::charmap::subset_matches(m,{3,3},{{'.','>'},{0,0}}); 
    p = metanull::charmap::subset_matches(m,{3,3},{{'.','>'},{0,0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    p = metanull::charmap::subset_matches(m,{1,1},{{'.','.','.'},{'.','o','.'},{'.','.','.'}}); 
    p = metanull::charmap::subset_matches(m,{1,1},{{'.','.','.'},{'.',0,'.'},{'.','.','.'}}); 
    p = metanull::charmap::subset_matches(m,{1,1},{{'.','.','.'},{'.',0,'.'},{'.','.','.'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    p = metanull::charmap::subset_matches(m,{3,3},{{'.','>',0},{0,0,0}},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    p = metanull::charmap::subset_matches(m,{3,3},{{'.','>',0},{0,0,0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER | metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    p = metanull::charmap::subset_matches(m,{3,3},{{'.',0},{'v',0},{0,0,0}},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    p = metanull::charmap::subset_matches(m,{3,3},{{'.',0},{'v',0},{0,0,0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER | metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS); 
    auto q = metanull::charmap::subset_find(m,{{'<','.','.'}}); 
    q = metanull::charmap::subset_find(m,{{'<','.',0},{'<','.',0}}); 
    q = metanull::charmap::subset_find(m,{{'<','.',0},{'<','.',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    q = metanull::charmap::subset_find(m,{{'.','.','>'}}); 
    q = metanull::charmap::subset_find(m,{{'\0','.','>'},{'\0','.','>'}}); 
    q = metanull::charmap::subset_find(m,{{'\0','.','>'},{'\0','.','>'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    q = metanull::charmap::subset_find(m,{{'^'},{'.'},{'.'}}); 
    q = metanull::charmap::subset_find(m,{{'^'},{'.'},{'\0'}}); 
    q = metanull::charmap::subset_find(m,{{'^'},{'.'},{'\0'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    q = metanull::charmap::subset_find(m,{{'.'},{'.'},{'v'}}); 
    q = metanull::charmap::subset_find(m,{{'\0'},{'.'},{'v'}}); 
    q = metanull::charmap::subset_find(m,{{'\0'},{'.'},{'v'}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    q = metanull::charmap::subset_find(m,{
        {'+','^','^','^','+'},
        {'<','.','.','.','>'},
        {'<','.','o','.','>'},
        {'<','.','.','.','>'},
        {'+','v','v','v','+'}
    }); 
    q = metanull::charmap::subset_find(m,{
        {0,0,'^',0,0},
        {0,0,0,0,0},
        {'<',0,'o',0,'>'},
        {0,0,0,0,0},
        {0,0,'v',0,0}
    }); 
    q = metanull::charmap::subset_find(m,{
        {0,0,'^',0,0},
        {0,0,0,0,0},
        {'<',0,'o',0,'>'},
        {0,0,0,0,0},
        {0,0,'v',0,0}
    },metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER); 
    auto r = metanull::charmap::subset_replace(m,{0,0},{{'a','#','.'},{'.','a','.'}});
    r = metanull::charmap::subset_replace(m,{1,1},{{'b','#','.'},{'.','b','.'}});
    r = metanull::charmap::subset_replace(m,{2,2},{{'c','#','.'},{'.','c','.'}});
    r = metanull::charmap::subset_replace(m,{3,3},{{'d','#','.'},{'.','d','.'}});
    r = metanull::charmap::subset_replace(m,{4,4},{{'e','#','.'},{'.','e','.'}});
    r = metanull::charmap::subset_replace(m,{3,3},{{'D','#','.'},{'.','D','.'}},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
    r = metanull::charmap::subset_replace(m,{4,4},{{'E','#','.'},{'.','E','.'}},metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
    r = metanull::charmap::subset_replace(m,{0,0},{{'\0','*','f'},{0,'f',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
    r = metanull::charmap::subset_replace(m,{1,1},{{'\0','*','g'},{0,'g',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
    r = metanull::charmap::subset_replace(m,{2,2},{{'\0','*','h'},{0,'h',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
    r = metanull::charmap::subset_replace(m,{3,3},{{'\0','*','i'},{0,'i',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
    r = metanull::charmap::subset_replace(m,{4,4},{{'\0','*','j'},{0,'j',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER);
    r = metanull::charmap::subset_replace(m,{3,3},{{'\0','*','I'},{0,'I',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER | metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
    r = metanull::charmap::subset_replace(m,{4,4},{{'J','*','J'},{0,'I',0}},metanull::charmap::SUBSET_MATCH_NULL_AS_WILDCHARACTER | metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);

    metanull::charmap::index p = {2,2};
    auto n = metanull::charmap::access(m,p);
    metanull::charmap::access(m,p) = 'Z';
    n = metanull::charmap::access(m,p);
    p = {0,0};
    n = metanull::charmap::access(m,p);
    p = {4,4};
    n = metanull::charmap::access(m,p);
    try {
        p = {5,5};
        n = metanull::charmap::access(m,p);
    } catch(std::out_of_range e) {
        // Success
    }
    p = {0,0};
    auto r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::direction d){return true;} );
    p = {2,2};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::direction d){return true;} );
    p = {4,4};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::direction d){return true;} );
    
    p = {0,0};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::direction d){return '.' == c;} );
    p = {2,2};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::direction d){return '.' == c;} );
    p = {4,4};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::direction d){return '.' == c;} );

*/