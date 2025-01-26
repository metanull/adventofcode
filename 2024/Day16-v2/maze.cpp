#include "maze.h"

maze_node::maze_node(metanull::charmap::position pos, metanull::charmap::direction dir, size_t score) : score(score), closed(false), start(pos), start_direction(dir), end(pos), end_direction(dir), visited({}), turns({})
{
}

const maze_node::tile_test_function maze_node::is_tile_free =
    [](const metanull::charmap::map &m, const metanull::charmap::position &p, const metanull::charmap::position &end) -> bool
{
    return p == end || metanull::charmap::access(m, p) == '.';
};

std::vector<maze_node> maze_node::init(const metanull::charmap::map &m, const metanull::charmap::position &start, const metanull::charmap::position &end, const metanull::charmap::direction &start_direction, const tile_test_function &is_tile_free)
{
    std::vector<maze_node> next_nodes;
    maze_node start_node(start, start_direction, 0);
    auto options = metanull::charmap::neighbours_if(m, start, [m, end, is_tile_free](metanull::charmap::position p, char c, metanull::charmap::direction d)
                                                    { return is_tile_free(m, p, end) && (d == metanull::charmap::SOUTH || d == metanull::charmap::NORTH || d == metanull::charmap::EAST || d == metanull::charmap::WEST); });
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

std::vector<maze_node> maze_node::next_nodes_from(const metanull::charmap::map &m, maze_node &origin, const metanull::charmap::position &end, const tile_test_function &is_tile_free)
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
        auto options = metanull::charmap::neighbours_if(m, current.end, [current, m, end, is_tile_free](metanull::charmap::position p, char c, metanull::charmap::direction d)
                                                        { return is_tile_free(m, p, end) && (d != metanull::charmap::inverse(current.end_direction)) && (d == metanull::charmap::SOUTH || d == metanull::charmap::NORTH || d == metanull::charmap::EAST || d == metanull::charmap::WEST); });
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
            if (options.size() != 1)
            {
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

std::ostream &operator<<(std::ostream &os, const maze_node &n)
{
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
    if (!n.visited.empty())
    {
        os << " - .front: (" << metanull::charmap::abscissa(n.visited.front()) << ", " << metanull::charmap::ordinate(n.visited.front()) << ")";
        std::hash<std::string> hasher;
        std::string visited_str;
        for (const auto &pos : n.visited)
        {
            visited_str += std::to_string(metanull::charmap::abscissa(pos)) + "," + std::to_string(metanull::charmap::ordinate(pos)) + ";";
        }
        os << " - hash: " << hasher(visited_str);
        os << " - .back: (" << metanull::charmap::abscissa(n.visited.back()) << ", " << metanull::charmap::ordinate(n.visited.back()) << ")";
    }
    os << std::endl;
    os << "- turns:   - .size(): " << n.turns.size();
    if (!n.turns.empty())
    {
        os << " - .front: (" << metanull::charmap::abscissa(n.turns.front()) << ", " << metanull::charmap::ordinate(n.turns.front()) << ")";
        std::hash<std::string> hasher;
        std::string turns_str;
        for (const auto &pos : n.turns)
        {
            turns_str += std::to_string(metanull::charmap::abscissa(pos)) + "," + std::to_string(metanull::charmap::ordinate(pos)) + ";";
        }
        os << " - hash: " << hasher(turns_str);
        os << " - .back: (" << metanull::charmap::abscissa(n.turns.back()) << ", " << metanull::charmap::ordinate(n.turns.back()) << ")";
    }
    os << std::endl;

    return os;
}

std::vector<maze_node> maze_node::find_first_best_path(const metanull::charmap::map &m, const metanull::charmap::position &start, const metanull::charmap::position &end, const metanull::charmap::direction &start_direction, const tile_test_function &is_tile_free)
{
    std::vector<maze_node> log;
    std::vector<maze_node> closed_nodes;
    std::vector<maze_node> open_nodes = init(m, start, end, start_direction, is_tile_free);
    while (!open_nodes.empty())
    {
        // Store closed nodes in a separate list, and remove them from open_nodes
        std::copy_if(open_nodes.begin(), open_nodes.end(), std::back_inserter(closed_nodes), [](const maze_node &n)
                     { return n.closed; });
        open_nodes.erase(std::remove_if(open_nodes.begin(), open_nodes.end(), [](const maze_node &n)
                                        { return n.closed; }),
                         open_nodes.end());

        // Break as soon as a node gets closed, as they are sorted by score the first one is the best
        if (!closed_nodes.empty())
        {
            break;
        }

        // Sort open_nodes by score, to process the best nodes first
        std::sort(open_nodes.begin(), open_nodes.end(), [](const auto &lhs, const auto &rhs)
                  { return lhs.score < rhs.score; });

        // Discover the next nodes starting from the current best one
        auto next_nodes = next_nodes_from(m, open_nodes.front(), end, is_tile_free);
        // Remove the current node, as it is replaced by the next nodes (if any)
        open_nodes.erase(open_nodes.begin());

        // Speed up by trimming the next nodes that are already in the log with the same end, end_direction, and a score that is lower or equal
        next_nodes.erase(std::remove_if(next_nodes.begin(), next_nodes.end(), [&log](const maze_node &n)
                                        { return std::find_if(log.begin(), log.end(), [&n](const maze_node &ln)
                                                              { return ln.end == n.end && ln.end_direction == n.end_direction && ln.score <= n.score; }) != log.end(); }),
                         next_nodes.end());
        // Remove from the log the items that are in next_nodes with the same end, end_direction
        log.erase(std::remove_if(log.begin(), log.end(), [&next_nodes](const maze_node &ln)
                                 { return std::find_if(next_nodes.begin(), next_nodes.end(), [&ln](const maze_node &n)
                                                       { return ln.end == n.end && ln.end_direction == n.end_direction; }) != next_nodes.end(); }),
                  log.end());
        // Then add all the items from next_nodes to the log
        std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(log));

        // Add the remaining next nodes to the list of open_nodes
        std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(open_nodes));

        continue;
    }

    return closed_nodes;
}

std::vector<maze_node> maze_node::find_all_best_path(const metanull::charmap::map &m, const metanull::charmap::position &start, const metanull::charmap::position &end, const metanull::charmap::direction &start_direction, const tile_test_function &is_tile_free)
{
    std::vector<maze_node> log;
    std::vector<maze_node> closed_nodes;
    std::vector<maze_node> open_nodes = init(m, start, end, start_direction, is_tile_free);
    size_t lowest_score = SIZE_MAX;

    // Process all nodes until we a node with a score greater than the best one (so that we get the list of all best nodes)
    while (!open_nodes.empty())
    {
        // Capture the best score as soon as available
        if (lowest_score == SIZE_MAX && !closed_nodes.empty())
        {
            lowest_score = closed_nodes.front().score;
        }

        // Store closed nodes in a separate list, and remove them from open_nodes
        std::copy_if(open_nodes.begin(), open_nodes.end(), std::back_inserter(closed_nodes), [](const maze_node &n)
                     { return n.closed; });
        // ... and remove them from open_nodes
        open_nodes.erase(std::remove_if(open_nodes.begin(), open_nodes.end(), [](const maze_node &n)
                                        { return n.closed; }),
                         open_nodes.end());

        // Break only when we find a node that has a score greater than the best one
        bool found = false;
        for (auto &node : closed_nodes)
        {
            if (lowest_score != SIZE_MAX && node.score != lowest_score)
            {
                found = true;
                break;
            }
        }
        if (found)
        {
            // make sure that only closed nodes with the actual best_Score are kept
            closed_nodes.erase(std::remove_if(closed_nodes.begin(), closed_nodes.end(), [lowest_score](const maze_node &n)
                                              { return n.score != lowest_score; }),
                               closed_nodes.end());
            // Leave the loop
            break;
        }

        // Sort open_nodes by score, to process the best nodes first
        std::sort(open_nodes.begin(), open_nodes.end(), [](const auto &lhs, const auto &rhs)
                  { return lhs.score < rhs.score; });

        // Discover the next nodes starting from the current best one
        auto next_nodes = next_nodes_from(m, open_nodes.front(), end, is_tile_free);
        // Remove the current node, as it is replaced by the next nodes (if any)
        open_nodes.erase(open_nodes.begin());

        // Trim all nodes with a score higher than lowest_score
        if (lowest_score != SIZE_MAX)
        {
            next_nodes.erase(std::remove_if(next_nodes.begin(), next_nodes.end(), [lowest_score](const maze_node &n)
                                            { return n.score > lowest_score; }),
                             next_nodes.end());
        }

        // Trim all nodes that are already found in the log with the same end, end_drection, and a score that is (1, if only the first best path is needed) lower or equal or (2, if all "best path" are needed) strictly lower
        next_nodes.erase(std::remove_if(next_nodes.begin(), next_nodes.end(), [&log](const maze_node &n)
                                        { return std::find_if(log.begin(), log.end(), [&n](const maze_node &ln)
                                                              {
                                                                  // <  if we are interrested in all path having the best score:
                                                                  return ln.score < n.score && ln.end == n.end && ln.end_direction == n.end_direction;
                                                              }) != log.end(); }),
                         next_nodes.end());

        // Trim open_nodes when they contain a node similar to one in next_nodes but with a greater score
        open_nodes.erase(std::remove_if(open_nodes.begin(), open_nodes.end(), [&next_nodes](const maze_node &n)
                                        { return std::find_if(next_nodes.begin(), next_nodes.end(), [&n](const maze_node &nn)
                                                              { return nn.score < n.score && nn.end == n.end && nn.end_direction == n.end_direction; }) != next_nodes.end(); }),
                         open_nodes.end());

        // Add remaining discovered nodes to the list of open_nodes pending processing
        if (!next_nodes.empty())
        {
            // Update the log by adding or replacing entries from next_nodes
            log.erase(std::remove_if(log.begin(), log.end(), [&next_nodes](const maze_node &n)
                                     { return std::find_if(next_nodes.begin(), next_nodes.end(), [&n](const maze_node &nn)
                                                           { return nn.end == n.end && nn.end_direction == n.end_direction; }) != next_nodes.end(); }),
                      log.end());
            std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(log));

            // Add the remaining next nodes to the list
            std::copy(next_nodes.begin(), next_nodes.end(), std::back_inserter(open_nodes));

            // Keep processing open_nodes
            continue;
        }
    }

    // Sort closed_nodes by score
    std::sort(closed_nodes.begin(), closed_nodes.end(), [](const maze_node &lhs, const maze_node &rhs)
              { return lhs.score < rhs.score; });

    return closed_nodes;
}