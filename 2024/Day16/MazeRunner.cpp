#include "MazeRunner.h"

MazeRunner::MazeRunner(const Maze & map) {
    maze = map;
    current = {maze.Start(), Compass::EAST, 0};
    crosspoints.clear();
    segments = std::stack<MazeSegment>();
    currentSegment = {current, current, std::vector<MazeRunnerPosition>()};
    bestScore = LONG_MAX;
    bestPath = std::stack<MazeSegment>();
}

long MazeRunner::Run(std::function<void(std::stack<MazeSegment>,long)> callback) {
    current = {maze.Start(), Compass::EAST, 0};
    crosspoints.clear();
    segments = std::stack<MazeSegment>();
    currentSegment = {current, current, std::vector<MazeRunnerPosition>()};
    bestScore = LONG_MAX;
    bestPath = std::stack<MazeSegment>();

    while(true) {
        while(true) {
            if(IsExit()) {
                std::cout << "\033[31m;[---> EXIT EXIT EXIT EXIT EXIT EXIT EXIT]\033[0m;" << std::endl;

                std::cout << "(" << current.position.first << ", " << current.position.second << ") EXIT REACHED." << std::endl;
                // Exit reached
                // Close and push the last segment (it is popped immediatelly after, at the end of this block)
                currentSegment.end = current;
                segments.push(currentSegment);

                // Update the best path and counter
                if(current.score < bestScore) {
                    std::cout << "\033[34m;[---> BEST BEST BEST BEST BEST BEST BEST]\033[0m;" << std::endl;
                    std::cout << "(" << current.position.first << ", " << current.position.second << ") BEST SCORE FOUND." << std::endl;
                    bestPath = segments;
                    bestScore = current.score;
                }

                // DEBUG (in real it should be called at the exit only)
                callback(segments, current.score);

                // Pop the last segment
                segments.pop();

                break;
            }

            if(IsStart() && !currentSegment.moves.empty()) {
                std::cout << "\033[32m;[---> START START START START START START START]\033[0m;]" << std::endl;
                std::cout << "(" << current.position.first << ", " << current.position.second << ") STARTING POINT (LOOP)." << std::endl;
                // We have returned to the starting point, go back to the last crosspoint, and try another path (by removing the first path that we had initially tried)                
                if(!RestoreFromLastCrosspoint()) {
                    break;
                } else {
                    continue;
                }
            }

            if(IsDeadEnd()) {
                std::cout << "\033[31m;[---> DEAD DEAD DEAD DEAD DEAD DEAD DEAD]\033[0m;" << std::endl;
                std::cout << "(" << current.position.first << ", " << current.position.second << ") DEAD END." << std::endl;
                // Dead end reached, go back to the last crosspoint, and try another path (by removing the first path that we had initially tried)
                if(!RestoreFromLastCrosspoint()) {
                    break;
                } else {
                    continue;
                }
            }
            
            if(IsCrosspoint()) {
                std::cout << "\033[33m;[---> X-POINT X-POINT X-POINT X-POINT X-POINT X-POINT]\033[0m;" << std::endl;
                if(IsStart()) {
                    std::cout << "(" << current.position.first << ", " << current.position.second << ") STARTING POINT." << std::endl;
                    // Cross point at the starting point, we should only add it once
                    if(crosspoints.empty()) {
                        SaveCrosspoint();
                    }
                } else {
                    // Regular Crosspoint reached, it should be saved
                    std::cout << "(" << current.position.first << ", " << current.position.second << ") CROSS POINT." << std::endl;

                    // Find the crosspoint in the list of crosspoints...
                    auto cp = std::find_if(crosspoints.begin(), crosspoints.end(), [&](const MazeRunnerCrosspoint & cp) {
                        return cp.origin.position == current.position;
                    });
                    if(cp == crosspoints.end()) {
                        // New crosspoint
                        SaveCrosspoint();
                    } else {
                        // Already known crosspoint (we came to a loop), go back to the last crosspoint, and try another path (by removing the first path we had tried)
                        // if that other path is also exhausted, go back to the previous crosspoint in a loop
                        if(!RestoreFromLastCrosspoint()) {
                            break;
                        }
                    }
                }
            }
            
            // Move one step forward and update the score
            Compass choice = Compass::UNKNOWN;
            if(IsCrosspoint()) {
                auto cp = crosspoints.end() - 1;
                choice = cp->options.front();

                // Remove the option from the crosspoint
                std::cout << "\033[32m;[USE & ERASE OPTION " << choice << "]\033[0m;" << std::endl;
                cp->options.erase(cp->options.begin());
            } else {
                std::vector<Compass> options = Options();
                choice = options.front();
            }
            if(choice != current.direction) {
                // There is a rotation
                current.direction = choice;
                current.score += 1001;
            } else {
                // No rotation
                current.score += 1;
            }
            current.position = maze.Move(current.position, current.direction);
            std::cout << "\033[32m;[---> CURRENT SEGMENT += " << current.position.first << ", " << current.position.second << "]\033[0m;" << std::endl;
            currentSegment.moves.push_back(current);
        }
        
        // Done processing a whole path... Are there other for us to explore?.
        if(crosspoints.empty()) {
            std::cout << "\033[31m;[---> LEAVE LEAVE LEAVE LEAVE LEAVE LEAVE LEAVE]\033[0m;";
            std::cout << "(" << current.position.first << ", " << current.position.second << ") ALL CROSSPOINTS HAVE BEEN TRAVERSED." << std::endl;
            // No, let's leave
            break;
        }

        // Yes, restore position at the previous cross point
        std::cout << "\033[31m;[>>>>> EXPLORE OTHER PATH\033[0m;" << std::endl;
        
        /*current = crosspoints.back().origin;
        currentSegment = segments.top();
        segments.pop();*/
        if(!RestoreFromLastCrosspoint()) {
            std::cout << "\033[31m;[---> DONE RESUME DONE RESUME DONE RESUME DONE RESUME]\033[0m;" << std::endl;
            std::cout << "(" << current.position.first << ", " << current.position.second << ") EXPLORE MORE CROSSPOINTS." << std::endl;
            break;
        }
        if(crosspoints.begin() == crosspoints.end() - 1) {
            // We returned to the initial cross point, wipe the current segment (as the segment is only added the moment the end of it is discovered)
            std::cout << "\033[32m;[---> CURRENT SEGMENT = [] #############################\033[0m;" << std::endl;
            currentSegment.moves.clear();
        }
        
    }

    callback(bestPath, bestScore);
    return bestScore;
}

void MazeRunner::SaveCrosspoint() {
    std::cout << "\033[32m;[---> PUSH PUSH PUSH PUSH PUSH PUSH PUSH PUSH]\033[0m;" << std::endl;
    std::cout << "PUSH CROSSPOINT " << current.position.first << ", " << current.position.second << " (" << current.direction << ", " << current.score << ") Options: ";
    for(auto opt : Options()) {
        std::cout << opt << " ";
    }
    std::cout << std::endl;

    // New crosspoint
    crosspoints.push_back({current,Options()});

    if(currentSegment.end.position == currentSegment.origin.position) {
        // The current segment is empty, we should not add it to the stack
        std::cout << "\033[32m;[---> CURRENT SEGMENT : Origin == End]\033[0m;" << std::endl;
    }
    if(segments.empty()) {
        // The first segment is empty, we should not add it to the stack
        std::cout << "\033[32m;[---> CURRENT SEGMENT : SEGMENTS[] is EMPTY]\033[0m;" << std::endl;
    }

    // Between two crosspoints, that is a segment, we want to keep track of it
    std::cout << "\033[32m;[---> CURRENT SEGMENT : CLOSE]\033[0m;" << std::endl;
    currentSegment.end = current;
    segments.push(currentSegment);
    std::cout << "\033[32m;[---> CURRENT SEGMENT : NEW]\033[0m;" << std::endl;
    currentSegment = {current, MazeRunnerPosition(), std::vector<MazeRunnerPosition>()};

/*
##### IL Y A DES SEGMENTS EN DOUBLE!!!
*/
}

bool MazeRunner::RestoreFromLastCrosspoint() {
    std::cout << "\033[32m;[---> POP POP POP POP POP POP POP POP POP]\033[0m;" << std::endl;
    if(crosspoints.empty()) {
        std::cout << "POP CROSSPOINT > No crosspoints in the stack" << std::endl;
        return false;
    }
    auto lastCp = crosspoints.back();
    
    std::cout << "POPPING > " << lastCp.origin.position.first << ", " << lastCp.origin.position.second << " (" << lastCp.origin.direction << ", " << lastCp.origin.score << ", {";
    for(auto opt : lastCp.options) {
        std::cout << opt << " ";
    }
    std::cout << "})" << std::endl;
    
    // Does the last crosspoint have any options left?
    if(lastCp.options.empty()) {
        std::cout << "POPPED > CROSSPOINT HAD NO OPTIONS LEFT" << std::endl;

        current = crosspoints.back().origin;
        currentSegment = segments.top();
        crosspoints.pop_back();
        segments.pop();

        std::cout << "\033[32m;[---> CURRENT SEGMENT = [";
        for(auto move : currentSegment.moves) {
            std::cout << move.position.first << ", " << move.position.second << " ";
        }
        std::cout << "]\033[0m;" << std::endl;
        
        return RestoreFromLastCrosspoint();
    }

    // Update the current position, remove the last option from the crosspoint
    current = crosspoints.back().origin;
    currentSegment = segments.top();

    std::cout << "\033[32m;[---> CURRENT SEGMENT = [";
    for(auto move : currentSegment.moves) {
        std::cout << move.position.first << ", " << move.position.second << " ";
    }
    std::cout << "]\033[0m;" << std::endl;
    
    std::cout << "POPPED > " << crosspoints.back().origin.position.first << ", " << crosspoints.back().origin.position.second << " Score: (" << crosspoints.back().origin.score << ", Crosspoint Options: {";
    if(crosspoints.back().options.empty()) {
        std::cout << "\033[31m;NO OPTIONS LEFT\033[0m;";
    } else {
        for(auto opt : crosspoints.back().options) {
            std::cout << "\033[33m;" << opt << "\033[0m; ";
        }
    }
    std::cout << "}" << std::endl;

    return true;
}

bool MazeRunner::IsDeadEnd() const {
    bool forward, clockwise, counter_clockwise, reverse;
    return Options(forward, clockwise, counter_clockwise, reverse) == 1 && reverse;
}
bool MazeRunner::IsExit() const {
    return maze.GetTile(current.position) == Maze::EXIT_CHAR;
}
bool MazeRunner::IsCrosspoint() const {
    bool forward, clockwise, counter_clockwise, reverse;
    int options = Options(forward, clockwise, counter_clockwise, reverse);
    
    // The starting point is a crosspoint if it has at least 2 options to go
    if(IsStart()) {
        return options > 1;
    }
    // A crosspoint is a point where there are more than one options to go (excluding the reverse direction - which ias assumed always present)
    return options > 2;
}
bool MazeRunner::IsStart() const {
    return maze.GetTile(current.position) == Maze::START_CHAR;
}

/**
 * @brief Get the options available to the runner
 */
int MazeRunner::Options(bool & forward, bool & clockwise, bool & counter_clockwise, bool & reverse) const {
    forward = clockwise = counter_clockwise = reverse = false;
    if(maze.GetTile(current.position) == Maze::WALL_CHAR) {
        return 0;
    }
    int options = 0;
    if(maze.GetTileForward(current.position, current.direction) != Maze::WALL_CHAR) {
        forward = true;
        options ++;
    }
    if(maze.GetTileClockwise(current.position, current.direction) != Maze::WALL_CHAR) {
        clockwise = true;
        options ++;
    }
    if(maze.GetTileCounterClockwise(current.position, current.direction) != Maze::WALL_CHAR) {
        counter_clockwise = true;
        options ++;
    }
    if(maze.GetTileReverse(current.position, current.direction) != Maze::WALL_CHAR) {
        reverse = true;
        options ++;
    }
    return options;
}
/**
 * @brief Get the options available to the runner as a list
*/
std::vector<Compass> MazeRunner::Options() const {
    bool forward, clockwise, counter_clockwise, reverse;
    int options = Options(forward, clockwise, counter_clockwise, reverse);
    std::vector<Compass> list;
    if(forward) {
        list.push_back(current.direction);
    }
    if(clockwise) {
        list.push_back(Compass::Clockwise(current.direction));
    }
    if(counter_clockwise) {
        list.push_back(Compass::CounterClockwise(current.direction));
    }
    return list;
}

/**
 * @brief Make a choice among the available options
 */
Compass MazeRunner::Option(int choice, bool & rotation) const {
    bool forward, clockwise, counter_clockwise, reverse;
    forward = clockwise = counter_clockwise = reverse = rotation = false;
    int options = Options(forward, clockwise, counter_clockwise, reverse);
    if(choice < 0 || choice >= options) {
        throw std::runtime_error("Invalid choice");
    }
    if(choice == 0) {
        if(forward) {
            return current.direction;
        }
        if(clockwise) {
            return Compass::Clockwise(current.direction);
        }
        if(counter_clockwise) {
            return Compass::CounterClockwise(current.direction);
        }
    }
    if(choice == 1) {
        if(forward) {
            if(clockwise) {
                rotation = true;
                return Compass::Clockwise(current.direction);
            }
            if(counter_clockwise) {
                rotation = true;
                return Compass::CounterClockwise(current.direction);
            }
        } else {
            if(clockwise && counter_clockwise) {
                rotation = true;
                return Compass::CounterClockwise(current.direction);
            }
        }
    }
    if(choice == 2) {
        if(forward && clockwise && counter_clockwise) {
            rotation = true;
            return Compass::CounterClockwise(current.direction);
        }
    }
    return Compass::UNKNOWN;
}