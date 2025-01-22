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
        {"subset_matches-top_left", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{0,0},{
                {'+','^'},
                {'<','.'}}
            );
            return n;
        }},
        {"subset_matches-top_right", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,0},{
                {'^','+'},
                {'.','>'}}
            );
            return n;
        }},
        {"subset_matches-bottom_left", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{0,2},{
                {'<','.'},
                {'+','v'}}
            );
            return n;
        }},
        {"subset_matches-bottom_right", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{2,2},{
                {'.','>'},
                {'v','+'}}
            );
            return n;
        }},
        {"subset_matches-out_of_bounds", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{3,3},{
                {'.','>'},
                {'v','+'}}
            );
            return !n;
        }},
        {"subset_matches-out_of_bounds-truncate", [m]() -> bool {
            auto n = metanull::charmap::subset_matches(m,{3,3},{
                {'.','>'},
                {'v','+'}}
            ,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return !n;
        }},
        {"subset_matches-out_of_bounds-truncate-empty", [m]() -> bool {
            auto o = metanull::charmap::subset(m,{6,6},3);
            auto n = metanull::charmap::subset_matches(m,{6,6},{
                {'.','>'},
                {'v','+'}}
            ,metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS);
            return !n;
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