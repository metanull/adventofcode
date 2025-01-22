#include "charmap.h"

namespace metanull {
    namespace charmap {
        map subset(const map & m, index origin, index end, int flags) {
            if(end.second > m.size() || end.first > m[0].size()) {
                if( flags & metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS ) {
                    end.second = std::min(m.size(),end.second);
                    end.first = std::min(m[0].size(),end.first);
                } else {
                    // throw std::out_of_range("Index out of range");
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
                    // throw std::out_of_range("Index out of range");
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
                    // throw std::out_of_range("Index out of range");
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

        index translate(const index & o, std::pair<int,int> d) {
            return index(o.first + d.first, o.second + d.second);
        }
        void test_in_bounds(const map & m, const index & p) {
            if(p.second >= m.size() || p.first >= m[0].size()) {
                throw std::out_of_range("Index out of range");
            }
        }

        char & access(map & m, const index & p) {
            test_in_bounds(m,p);
            return m[p.second][p.first];
        }
        const char & access(const map & m, const index & p) {
            test_in_bounds(m,p);
            return m[p.second][p.first];
        }

        std::vector<std::pair<std::pair<int,int>,char>> neighbours_if(const map & m, const index & p, std::function<bool(char,std::pair<int,int>)> t) {
            std::vector<std::pair<std::pair<int,int>,char>> results;
            std::vector<std::pair<int,int>> directions = {
                NORTHWEST, NORTH, NORTHEAST,
                WEST,                  EAST,
                SOUTHWEST, SOUTH, SOUTHEAST
            };
            for(const auto & d : directions) {
                try {
                    char c = access(m,translate(p,d));
                    if(t(c,d)) {
                        results.emplace_back(d,c);
                    }
                } catch(std::out_of_range e) {
                    // out of range, skip
                }
            }
            return results;
        }

        std::string column_as_string(const map & m, const index & p) {
            test_in_bounds(m,p);
            std::string c;
            for(auto r = p.second; r < m.size(); r ++) {
                c.push_back(m[r][p.first]);
            }
            return c;
        }
        std::string row_as_string(const map & m, const index & p) {
            test_in_bounds(m,p);
            return std::string(m[p.second].begin() + p.first,m[p.second].end());
        }
        map transpose(const map & m) {
            map n;
            for(auto c = 0; c < m[0].size(); c ++) {
                row r;
                for(auto rw = 0; rw < m.size(); rw ++) {
                    r.push_back(m[rw][c]);
                }
                n.push_back(r);
            }
            return n;
        }

        /*
        std::optional<std::pair<index,char>> select_neighbour(const std::vector<std::pair<std::pair<int,int>,char>> & neighbours, const std::pair<int,int> & d) {
            auto it = std::find_if(neighbours.begin(), neighbours.end(), [std::pair<int,int>](neighbour n) { return n.d == std::pair<int,int>; })
            if(it != neighbours.end()) {
                return *it;
            } else {
                return std::nullopt;
            }
        }
        auto neighbor = metanull::map::findNeighborByDirection(results, std::pair<int,int>);

        if (neighbor) {
            std::cout << "Neighbor at std::pair<int,int> (" << std::pair<int,int>.first << ", " << std::pair<int,int>.second << "): " << neighbor->second << '\n';
        } else {
            std::cout << "No neighbor found at std::pair<int,int> (" << std::pair<int,int>.first << ", " << std::pair<int,int>.second << ")\n";
        }
        */

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
    auto r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::std::pair<int,int> d){return true;} );
    p = {2,2};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::std::pair<int,int> d){return true;} );
    p = {4,4};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::std::pair<int,int> d){return true;} );
    
    p = {0,0};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::std::pair<int,int> d){return '.' == c;} );
    p = {2,2};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::std::pair<int,int> d){return '.' == c;} );
    p = {4,4};
    r = metanull::charmap::neighbours_if(m,p, [](char c,metanull::charmap::std::pair<int,int> d){return '.' == c;} );

*/