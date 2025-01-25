#include "maze.h"

maze_node::maze_node(metanull::charmap::position pos, metanull::charmap::direction dir, size_t score) : score(score), closed(false), start(pos), start_direction(dir), end(pos), end_direction(dir), visited({}), turns({})
{
}

const maze_node::tile_test_function maze_node::is_tile_free = 
    [](const metanull::charmap::map &m, const metanull::charmap::position &p, const metanull::charmap::position &end) -> bool { 

        return p == end || metanull::charmap::access(m, p) == '.';

    };

std::vector<maze_node> maze_node::init(const metanull::charmap::map & m, const metanull::charmap::position & start, const metanull::charmap::position & end, const metanull::charmap::direction & start_direction, const tile_test_function & is_tile_free) {
    std::vector<maze_node> next_nodes;
    maze_node start_node(start, start_direction, 0);
    auto options = metanull::charmap::neighbours_if(m, start, [m,end,is_tile_free](metanull::charmap::position p, char c, metanull::charmap::direction d)
                                                    { return is_tile_free(m,p,end) 
                                                        && (d == metanull::charmap::SOUTH || d == metanull::charmap::NORTH || d == metanull::charmap::EAST || d == metanull::charmap::WEST); 
                                                    });
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
            n.score += maze_node::score_turn;
        }
        n.visited.push_back(n.start);
        n.visited.push_back(n.end);
        n.score += maze_node::score_step;
        next_nodes.push_back(n);
    }
    return next_nodes;
}

std::vector<maze_node> maze_node::next_nodes_from(const metanull::charmap::map & m, maze_node & origin, const metanull::charmap::position & end, const tile_test_function & is_tile_free) {
    if (origin.end == end)
    {
        origin.closed = true;
        origin.visited.push_back(origin.end);
        return {origin};
    }

    auto current = origin;
    while (true)
    {
        auto options = metanull::charmap::neighbours_if(m, current.end, [current,m,end,is_tile_free](metanull::charmap::position p, char c, metanull::charmap::direction d)
                                                        { return is_tile_free(m,p,end) 
                                                            && (d != metanull::charmap::inverse(current.end_direction)) 
                                                            && (d == metanull::charmap::SOUTH || d == metanull::charmap::NORTH || d == metanull::charmap::EAST || d == metanull::charmap::WEST); 
                                                        });
        if (options.empty())
        {
            // No options, we reached a dead end, discard the node
            return {};
        }

        std::vector<maze_node> next_nodes;
        auto current_backup = current;
        for (auto o : options)
        {
            auto next = metanull::charmap::translate(current.end, o.first);

            if (std::find(current.visited.begin(), current.visited.end(), next) != current.visited.end())
            {
                // This node was already visited, we ran into a loop, discard the node
                return {};
            }

            // Move current to the next position, following the current option
            current.visited.push_back(next);
            if (current.end_direction != o.first)
            {
                current.turns.push_back(current.end);
                current.score += maze_node::score_turn;
            }
            current.score += maze_node::score_step;
            current.end = next;
            current.end_direction = o.first;

            // Close the node if we reached the end, and return it
            if (next == end)
            {
                current.closed = true;
                return {current};
            }

            next_nodes.push_back(current);
            if (options.size() != 1) {
                // If there were multiple options, we need to apply the same processing to each; so restore the current node to its previous state, and process the next option
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

std::vector<maze_node> maze_node::find_best_path(const metanull::charmap::map & m, const metanull::charmap::position & start, const metanull::charmap::position & end, const metanull::charmap::direction & start_direction, const tile_test_function & is_tile_free) {
    std::vector<maze_node> log;
    std::vector<maze_node> closed_nodes;
    std::vector<maze_node> open_nodes = init(m, start, end, start_direction, is_tile_free);
    while(!open_nodes.empty())
    {
        // Store closed nodes in a separate list, and remove them from open_nodes
        std::copy_if(open_nodes.begin(), open_nodes.end(), std::back_inserter(closed_nodes), [](const maze_node &n)
            { 
                return n.closed; 
            }
        );
        open_nodes.erase(std::remove_if(open_nodes.begin(), open_nodes.end(), [](const maze_node &n)
                {
                    return n.closed; 
                }
            ),
            open_nodes.end()
        );

        // Break as soon as a node gets closed, as they are sorted by score the first one is the best
        if (!closed_nodes.empty())
        {
            break;
        }

        // Sort open_nodes by score, to process the best nodes first
        std::sort(open_nodes.begin(), open_nodes.end(), [](const auto &lhs, const auto &rhs)
            { 
                return lhs.score < rhs.score; 
            }
        );

        // Discover the next nodes starting from the current best one
        auto next_nodes = next_nodes_from(m, open_nodes.front(), end, is_tile_free);
        // Remove the current node, as it is replaced by the next nodes (if any)
        open_nodes.erase(open_nodes.begin());

        // Speed up by trimming the next nodes that are already in the log with the same end, end_direction, and a score that is lower or equal
        next_nodes.erase(std::remove_if(next_nodes.begin(), next_nodes.end(), [&log](const maze_node &n)
                { 
                    return std::find_if(log.begin(), log.end(), [&n](const maze_node &ln)
                        { 
                            return ln.end == n.end && ln.end_direction == n.end_direction && ln.score <= n.score; 
                        }
                    ) != log.end(); 
                }
            ), next_nodes.end()
        );
        // Remove from the log the items that are in next_nodes with the same end, end_direction
        log.erase(std::remove_if(log.begin(), log.end(), [&next_nodes](const maze_node &ln)
                { 
                    return std::find_if(next_nodes.begin(), next_nodes.end(), [&ln](const maze_node &n)
                        { 
                            return ln.end == n.end && ln.end_direction == n.end_direction; 
                        }
                    ) != next_nodes.end(); 
                }
            ), log.end()
        );
        // Then add all the items from next_nodes to the log
        std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(log));
        
        // Add the remaining next nodes to the list of open_nodes
        std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(open_nodes));
            
        continue;
    }

    return closed_nodes;
    /*
    std::sort(closed_nodes.begin(), closed_nodes.end(), [](const maze_node &lhs, const maze_node &rhs)
        { 
            return lhs.score < rhs.score; 
        }
    );
    */
    
}


std::ostream & operator<<(std::ostream & os, const maze_node & n) {
    os << "(" << metanull::charmap::abscissa(n.start) << ", " << metanull::charmap::ordinate(n.start) << ")";
    os << " -> ";
    os << "(" << metanull::charmap::abscissa(n.end) << ", " << metanull::charmap::ordinate(n.end) << ")" << std::endl;
    os << "- closed: " << n.closed << std::endl;
    os << "- score:  " << n.score << std::endl;
    os << "- start:  (" << metanull::charmap::abscissa(n.start) << ", " << metanull::charmap::ordinate(n.start) << ")";
    os << " - " << metanull::charmap::direction_as_string(n.start_direction) << std::endl;
    os << "- end:    (" << metanull::charmap::abscissa(n.end) << ", " << metanull::charmap::ordinate(n.end) << ")";
    os << " - " << metanull::charmap::direction_as_string(n.end_direction) << std::endl;
    os << "- visited: - .size(): " << n.visited.size();
    if (!n.visited.empty()) {
        os << " - .front: (" << metanull::charmap::abscissa(n.visited.front()) << ", " << metanull::charmap::ordinate(n.visited.front()) << ")";
        os << " - .back: (" << metanull::charmap::abscissa(n.visited.back()) << ", " << metanull::charmap::ordinate(n.visited.back()) << ")";
    }
    os << std::endl;
    os << "- turns:   - .size(): " << n.turns.size();
    if (!n.turns.empty()) {
        os << " - .front: (" << metanull::charmap::abscissa(n.turns.front()) << ", " << metanull::charmap::ordinate(n.turns.front()) << ")";
        os << " - .back: (" << metanull::charmap::abscissa(n.turns.back()) << ", " << metanull::charmap::ordinate(n.turns.back()) << ")";
    }
    os << std::endl;
    
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