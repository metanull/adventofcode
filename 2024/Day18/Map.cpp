#include "Map.h"

namespace metanull {
    namespace charmap {
        map subset(const map & m, index origin, index end, int flags) {
            if(end.second > m.size() || end.first > m[0].size()) {
                if( flags & metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS ) {
                    end.second = std::min(m.size(),end.second);
                    end.first = std::min(m[0].size(),end.first);
                } else {
                    // throw std::runtime_error("Out of bouds");
                    return map();
                }
            }
            map s;
            for(auto r = origin.second; r < m.size() && r < end.second; r ++) {
                row t;
                for(auto c = origin.first; c < m[r].size() && c < end.first; c ++) {
                    t.emplace_back(m[r][c]);
                }
                s.push_back(t);
            }
            return s;
        }
        map subset(const map & m, index origin, size_t w, int flags) {
            return subset(m,origin,std::make_pair(origin.first+w,origin.second+w),flags);
        }

        bool subset_matches(const map & m, index origin, const map & needle, int flags){
            index end = {origin.first + needle[0].size(), origin.second + needle.size()};
            if(end.second > m.size() || end.first > m[0].size()) {
                if( flags & metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS ) {
                    end.second = std::min(m.size(),end.second);
                    end.first = std::min(m[0].size(),end.first);
                } else {
                    // throw std::runtime_error("Out of bouds");
                    return false;
                }
            }
            for(auto r = origin.second; r < m.size() && r < end.second; r ++) {
                for(auto c = origin.first; c < m[r].size() && c < end.first; c ++) {
                    if(flags & SUBSET_MATCH_NULL_AS_WILDCHARACTER) {
                        if(!(needle[r-origin.second][c-origin.first] == 0 || m[r][c] == needle[r-origin.second][c-origin.first])) {
                            return false;
                        }
                    } else {
                        if(!(m[r][c] == needle[r-origin.second][c-origin.first])) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        std::vector<index> subset_find(const map & m, const map & needle, int flags) {
            std::vector<index> matches;
            index origin = {0,0};
            index end = {m[0].size() - needle[0].size() + 1,m.size() - needle.size() + 1};
            for(auto r = origin.second; r < m.size() && r < end.second; r ++) {
                for(auto c = origin.first; c < m[r].size() && c < end.first; c ++) {
                    if(subset_matches(m, {c,r}, needle, flags)) {
                        matches.emplace_back(c,r);
                    }
                }
            }
            return matches;
        }

        size_t subset_replace(map & m, index origin, const map & replace, int flags) {
            index end = {origin.first + replace[0].size(), origin.second + replace.size()};
            if(end.second > m.size() || end.first > m[0].size()) {
                if( flags & metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS ) {
                    end.second = std::min(m.size(),end.second);
                    end.first = std::min(m[0].size(),end.first);
                } else {
                    // throw std::runtime_error("Out of bouds");
                    return SIZE_MAX;
                }
            }
            size_t replaced = 0;
            for(auto r = origin.second; r < m.size() && r < end.second; r ++) {
                for(auto c = origin.first; c < m[r].size() && c < end.first; c ++) {
                    if(flags & SUBSET_MATCH_NULL_AS_WILDCHARACTER) {
                        if(replace[r-origin.second][c-origin.first] != 0) {
                            replaced += (m[r][c] == replace[r-origin.second][c-origin.first]);
                            m[r][c] = replace[r-origin.second][c-origin.first];
                        }
                    } else {
                        replaced += (m[r][c] == replace[r-origin.second][c-origin.first]);
                        m[r][c] = replace[r-origin.second][c-origin.first];
                    }
                }
            }
            return replaced;
        }

        std::vector<index> char_find(const map & m, char c) {
            std::vector<index> occurrences;
            for(auto r = 0; r < m.size(); r ++) {
                auto it = m[r].begin();
                while((it = std::find(it,m[r].end(), c)) != m[r].end()) {
                    occurrences.emplace_back(r,std::distance(m[r].begin(),it));
                    it++;
                }
            }
            return occurrences;
        }

    } // namespace map
} // namespace metanull


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

*/