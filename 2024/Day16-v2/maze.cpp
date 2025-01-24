#include "maze.h"

maze_node::maze_node(metanull::charmap::position pos, metanull::charmap::direction dir, size_t score) : score(score), closed(false), start(pos), start_direction(dir), end(pos), end_direction(dir), visited({}), turns({})
{
}

std::ostream & operator<<(std::ostream & os, const maze_node & n) {
    os << "Node:" << std::endl;
    os << "- start: (" << metanull::charmap::abscissa(n.start) << ", " << metanull::charmap::ordinate(n.start) << ")" << std::endl;
    os << "- start_direction: " << metanull::charmap::direction_as_string(n.start_direction) << std::endl;
    os << "- end: (" << metanull::charmap::abscissa(n.end) << ", " << metanull::charmap::ordinate(n.end) << ")" << std::endl;
    os << "- end_direction: " << metanull::charmap::direction_as_string(n.end_direction) << std::endl;
    os << "- score: " << n.score << std::endl;
    os << "- visited.size(): " << n.visited.size() << std::endl;
    if (!n.visited.empty()) {
        os << "- visited.front: (" << metanull::charmap::abscissa(n.visited.front()) << ", " << metanull::charmap::ordinate(n.visited.front()) << ")" << std::endl;
        os << "- visited.back: (" << metanull::charmap::abscissa(n.visited.back()) << ", " << metanull::charmap::ordinate(n.visited.back()) << ")" << std::endl;
    }
    os << "- turns.size(): " << n.turns.size() << std::endl;
    if (!n.turns.empty()) {
        os << "- turns.front: (" << metanull::charmap::abscissa(n.turns.front()) << ", " << metanull::charmap::ordinate(n.turns.front()) << ")" << std::endl;
        os << "- turns.back: (" << metanull::charmap::abscissa(n.turns.back()) << ", " << metanull::charmap::ordinate(n.turns.back()) << ")" << std::endl;
    }
    os << "- closed: " << n.closed << std::endl;
    return os;
}

bool maze::is_free(const metanull::charmap::position &p) const
{
    return p == end || is_free(metanull::charmap::access(m, p));
}
bool maze::is_free(char c) const
{
    return c == '.';
}

maze::maze(const metanull::charmap::map &map, const metanull::charmap::position &start, const metanull::charmap::position &end, const metanull::charmap::direction &start_dir) : m(map), start(start), end(end), start_direction(start_dir)
{
}
maze::maze(const metanull::charmap::map &map, char char_start, char char_end, const metanull::charmap::direction &start_dir) : m(map), start_direction(start_dir)
{
    auto start_list = metanull::charmap::char_find(map, char_start);
    if (start_list.empty() || start_list.size() > 1)
    {
        throw std::invalid_argument("Invalid start character (not found or multiple instances)");
    }
    start = start_list.front();
    auto end_list = metanull::charmap::char_find(map, char_end);
    if (end_list.empty() || end_list.size() > 1)
    {
        throw std::invalid_argument("Invalid end character (not found or multiple instances)");
    }
    end = end_list.front();
}

std::vector<maze_node> maze::init() const
{
    std::vector<maze_node> next_nodes;
    maze_node start_node(start, start_direction, 0);
    auto options = metanull::charmap::neighbours_if(m, start, [this](metanull::charmap::position p, char c, metanull::charmap::direction d)
                                                    { return is_free(p) && (d == metanull::charmap::SOUTH || d == metanull::charmap::NORTH || d == metanull::charmap::EAST || d == metanull::charmap::WEST); });
    if (options.empty())
    {
        throw std::runtime_error("Invalid maze");
    }
    for (auto o : options)
    {
        maze_node n = start_node;
        n.end = metanull::charmap::translate(n.start, o.first);
        n.end_direction = o.first;
        if (n.end_direction != n.start_direction)
        {
            n.turns.push_back(n.start);
            n.score += SCORE_TURN;
        }
        n.visited.push_back(n.start);
        n.visited.push_back(n.end);
        n.score += SCORE_STEP;
        next_nodes.push_back(n);
    }
    return next_nodes;
}

std::vector<maze_node> maze::next_nodes_from(maze_node &origin) const
{
    if (origin.end == end)
    {
        origin.closed = true;
        origin.visited.push_back(origin.end);
        return {origin};
    }

    auto current = origin;
    while (true)
    {
        auto options = metanull::charmap::neighbours_if(m, current.end, [this, current](metanull::charmap::position p, char c, metanull::charmap::direction d)
                                                        { return is_free(p) && (d != metanull::charmap::inverse(current.end_direction)) && (d == metanull::charmap::SOUTH || d == metanull::charmap::NORTH || d == metanull::charmap::EAST || d == metanull::charmap::WEST); });
        if (options.empty())
        {
            return {};
        }
        std::vector<maze_node> next_nodes;
        auto current_backup = current;
        for (auto o : options)
        {
            auto next = metanull::charmap::translate(current.end, o.first);

            if (std::find(current.visited.begin(), current.visited.end(), next) != current.visited.end())
            {
                // This node was already visited
                return {};
            }

            current.visited.push_back(next);
            if (current.end_direction != o.first)
            {
                current.turns.push_back(current.end);
                current.score += SCORE_TURN;
            }
            current.score += SCORE_STEP;
            current.end = next;
            current.end_direction = o.first;

            if (next == end)
            {
                current.closed = true;
                return {current};
            }

            next_nodes.push_back(current);
            if (options.size() != 1) {
                // Multiple options, restore current
                current = current_backup;
            }
        }
        if (options.size() == 1)
        {
            // There was only one option, continue browsing that path
            continue;
        }
        // There were multiple options, interrupt browsing, return them all
        return next_nodes;
    }
}