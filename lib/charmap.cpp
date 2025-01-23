#include "charmap.h"

namespace metanull
{
    namespace charmap
    {
        direction rotate_clockwise(const direction &d)
        {
            return {-d.second, d.first};
        }

        direction rotate_counter_clockwise(const direction &d)
        {
            return {d.second, -d.first};
        }

        direction inverse(const direction &d)
        {
            return {-d.first, -d.second};
        }

        map subset(const map &m, index origin, index end, int flags)
        {
            if (end.second > m.size() || end.first > m[0].size())
            {
                if (flags & metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS)
                {
                    end.second = std::min(m.size(), end.second);
                    end.first = std::min(m[0].size(), end.first);
                }
                else
                {
                    // throw std::out_of_range("Index out of range");
                    return map();
                }
            }
            map s;
            for (auto r = origin.second; r < m.size() && r < end.second; r++)
            {
                s.emplace_back(m[r].begin() + origin.first, m[r].begin() + end.first);
            }
            return s;
        }
        map subset(const map &m, index origin, size_t w, int flags)
        {
            return subset(m, origin, std::make_pair(origin.first + w, origin.second + w), flags);
        }

        bool subset_quick_matches(const map &m, index origin, const map &needle)
        {
            index end = {origin.first + needle[0].size(), origin.second + needle.size()};
            test_in_bounds(m, translate(end, {-1, -1}));
            
            for (auto r = m.begin() + origin.second; r < m.begin() + origin.second + needle.size(); r ++)
            {
                if (std::mismatch(r->begin() + origin.first, r->begin() + origin.first + needle[std::distance(m.begin() + origin.second, r)].size(), needle[std::distance(m.begin() + origin.second, r)].begin()).first != r->begin() + origin.first + needle[std::distance(m.begin() + origin.second, r)].size())
                {
                    return false;
                }
            }
            return true;
        }

        bool subset_matches(const map &m, index origin, const map &needle, int flags)
        {
            index end = {origin.first + needle[0].size(), origin.second + needle.size()};
            if (end.second > m.size() || end.first > m[0].size())
            {
                if (flags & metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS)
                {
                    end.second = std::min(m.size(), end.second);
                    end.first = std::min(m[0].size(), end.first);
                }
                else
                {
                    // throw std::out_of_range("Index out of range");
                    return false;
                }
            }
            for (auto r = origin.second; r < m.size() && r < end.second; r++)
            {
                for (auto c = origin.first; c < m[r].size() && c < end.first; c++)
                {
                    if (flags & SUBSET_MATCH_NULL_AS_WILDCHARACTER)
                    {
                        if (!(needle[r - origin.second][c - origin.first] == 0 || m[r][c] == needle[r - origin.second][c - origin.first]))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (!(m[r][c] == needle[r - origin.second][c - origin.first]))
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        std::vector<index> subset_find(const map &m, const map &needle, int flags)
        {
            std::vector<index> matches;
            index origin = {0, 0};
            index end = {m[0].size() - needle[0].size() + 1, m.size() - needle.size() + 1};
            for (auto r = origin.second; r < m.size() && r < end.second; r++)
            {
                for (auto c = origin.first; c < m[r].size() && c < end.first; c++)
                {
                    if (subset_matches(m, {c, r}, needle, flags))
                    {
                        matches.emplace_back(c, r);
                    }
                }
            }
            return matches;
        }

        void subset_quick_replace(map &m, index origin, const map &replace)
        {
            index end = {origin.first + replace[0].size(), origin.second + replace.size()};
            test_in_bounds(m, translate(end, {-1, -1}));
            
            for (auto r = replace.begin(); r < replace.end(); r ++)
            {
                std::copy(r->begin(), r->end(), (m.begin() + origin.second + std::distance(replace.begin(), r))->begin() + origin.first);
            }
        }

        size_t subset_replace(map &m, index origin, const map &replace, int flags)
        {
            index end = {origin.first + replace[0].size(), origin.second + replace.size()};
            try
            {
                test_in_bounds(m, translate(end, {-1, -1}));
            }
            catch (std::out_of_range e)
            {
                if (flags & metanull::charmap::SUBSET_TRUNCATE_ON_OUT_OF_BOUNDS)
                {
                    end.second = std::min(m.size(), end.second);
                    end.first = std::min(m[0].size(), end.first);
                }
                else
                {
                    return 0;
                }
            }

            size_t replaced = 0;
            for (auto r = origin.second; r < m.size() && r < end.second; r++)
            {
                for (auto c = origin.first; c < m[r].size() && c < end.first; c++)
                {
                    if (flags & SUBSET_MATCH_NULL_AS_WILDCHARACTER)
                    {
                        if (replace[r - origin.second][c - origin.first] != 0)
                        {
                            replaced += (m[r][c] != replace[r - origin.second][c - origin.first]);
                            m[r][c] = replace[r - origin.second][c - origin.first];
                        }
                    }
                    else
                    {
                        replaced += (m[r][c] != replace[r - origin.second][c - origin.first]);
                        m[r][c] = replace[r - origin.second][c - origin.first];
                    }
                }
            }
            return replaced;
        }

        size_t diff_count(const map & a, const map & b, int flags) {
            if(a.size() != b.size() || a[0].size() != b[0].size()) {
                throw std::out_of_range("Both maps must have the same dimensions");
            }
            size_t diff = 0;
            for(auto r = 0; r < a.size(); r++) {
                for(auto c = 0; c < a[r].size(); c++) {
                    if(flags & SUBSET_MATCH_NULL_AS_WILDCHARACTER) {
                        if(a[r][c] != 0 && b[r][c] != 0 && a[r][c] != b[r][c]) {
                            diff++;
                        }
                    } else {
                        if(a[r][c] != b[r][c]) {
                            diff++;
                        }
                    }
                }
            }
            return diff;
        }

        std::vector<index> char_find(const map &m, char c)
        {
            std::vector<index> occurrences;
            for (auto r = 0; r < m.size(); r++)
            {
                auto it = m[r].begin();
                while ((it = std::find(it, m[r].end(), c)) != m[r].end())
                {
                    occurrences.emplace_back(std::distance(m[r].begin(), it), r);
                    it++;
                }
            }
            return occurrences;
        }

        index translate(const index &o, std::pair<int, int> d)
        {
            return index(o.first + d.first, o.second + d.second);
        }
        void test_in_bounds(const map &m, const index &p)
        {
            if (p.second >= m.size() || p.first >= m[0].size())
            {
                throw std::out_of_range("Index out of range");
            }
        }

        char &access(map &m, const index &p)
        {
            test_in_bounds(m, p);
            return m[p.second][p.first];
        }
        const char &access(const map &m, const index &p)
        {
            test_in_bounds(m, p);
            return m[p.second][p.first];
        }

        std::vector<std::pair<std::pair<int, int>, char>> neighbours_if(const map &m, const index &p, std::function<bool(char, std::pair<int, int>)> t)
        {
            std::vector<std::pair<std::pair<int, int>, char>> results;
            std::vector<std::pair<int, int>> directions = {
                NORTHWEST, NORTH, NORTHEAST,
                WEST, EAST,
                SOUTHWEST, SOUTH, SOUTHEAST};
            for (const auto &d : directions)
            {
                try
                {
                    char c = access(m, translate(p, d));
                    if (t(c, d))
                    {
                        results.emplace_back(d, c);
                    }
                }
                catch (std::out_of_range e)
                {
                    // out of range, skip
                }
            }
            return results;
        }

        std::string column_as_string(const map &m, const index &p)
        {
            test_in_bounds(m, p);
            std::string c;
            for (auto r = p.second; r < m.size(); r++)
            {
                c.push_back(m[r][p.first]);
            }
            return c;
        }
        std::string row_as_string(const map &m, const index &p)
        {
            test_in_bounds(m, p);
            return std::string(m[p.second].begin() + p.first, m[p.second].end());
        }
        map transpose(const map &m)
        {
            map n;
            for (auto c = 0; c < m[0].size(); c++)
            {
                row r;
                for (auto rw = 0; rw < m.size(); rw++)
                {
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

std::istream &operator>>(std::istream &is, metanull::charmap::map &m)
{
    m.clear();
    std::string line;
    while (std::getline(is, line))
    {
        m.push_back(metanull::charmap::row(line.begin(), line.end()));
    }
    return is;
}

std::ostream &operator<<(std::ostream &os, const metanull::charmap::map &m)
{
    for (const auto &r : m)
    {
        for (const auto &c : r)
        {
            os << c;
        }
        os << std::endl;
    }
    return os;
}