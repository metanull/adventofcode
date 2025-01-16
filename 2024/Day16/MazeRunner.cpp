#include "MazeRunner.h"

long MazeRunner::Run3(std::function<void(std::vector<MazeSegment>,long)> exitCallback) {
    std::vector<MazeSegment> segments;
    std::vector<MazeRunnerCrosspoint> crosspoints;
    MazeRunnerPosition start = {maze.Start(), Compass::EAST, 0};
    MazeRunnerPosition current = start;
    MazeSegment currentSegment = {current, current, std::vector<MazeRunnerPosition>()};
    long bestScore = LONG_MAX;
    std::vector<MazeSegment> bestPath;


    while(true) {

        // Is the current segment closed?
        bool closed = (currentSegment.origin.position != currentSegment.end.position);

        // Is it a regular tile or a special tile (START,END,CROSS-POINT,DEAD-END)
        bool regular = !(
               maze.IsCrossPoint(current.position,current.direction)
            || maze.IsExit(current.position) 
            || maze.IsStart(current.position) 
            || maze.IsDeadEnd(current.position,current.direction)
        );

        // Are we just starting?
        bool first = segments.size() == 0 && currentSegment.moves.empty();
        
        if(!regular && !first) {
            // close the segment
            currentSegment.end = current;
            segments.push_back(currentSegment);
            currentSegment = {current,current,std::vector<MazeRunnerPosition>()};

            // process the segment
            bool shouldPop = false; // If true, after processing, pop/return to the previous segment

            // Found the exit
            if(maze.IsExit(currentSegment.end.position)) {
                exitCallback(segments,current.score);

                if(current.score < bestScore) {
                    bestPath = segments;
                    bestScore = current.score;
                }
                shouldPop = true;
            } else {
                // Anti loop
                if(maze.IsStart(currentSegment.end.position)) {
                    // Segment ends on the start tile. This is a loop
                    shouldPop = true;
                } else {
                    auto sff = std::find_if(segments.begin(), segments.end(), [currentSegment](const MazeSegment & s) {
                        // Look for identical segment
                        return currentSegment.origin.position == s.origin.position && currentSegment.end.position == s.end.position;
                    });
                    if(sff != segments.end() && sff < segments.end() - 1) {
                        // Found an identical segment. This is a loop
                        shouldPop = true;
                    } else {
                        auto srw = std::find_if(segments.begin(), segments.end(), [currentSegment](const MazeSegment & s) {
                            // Look for identical segment (in reverse direction)
                            return currentSegment.origin.position == s.end.position && currentSegment.end.position == s.origin.position;
                        });
                        if(srw != segments.end() && sff < segments.end() - 1) {
                            // Found an identical segment (inverted). This is a loop
                            shouldPop = true;
                        }
                    }
                }
                // Speed enhancement (could be even better placed on top, but beware of the "closed segments" logic)
                if(current.score > bestScore) {
                    // This path is too costly, no need to keep on
                    shouldPop = true;
                }
            }

            if(!shouldPop) {
                // Nothing to pop, just move ahead
                // Update the runner's orientation
                auto options = maze.Options(current.position,current.direction);
                if(options.empty()) {
                    throw std::runtime_error("Unexpected state, this shouldn't be a dead end");
                }
                if(options.front() != current.direction) {
                    current.score += 1000;
                    current.direction = options.front();
                }

                // Move the runner one step ahead in the chosen direction
                current.position = maze.Move(current.position,current.direction);
                current.score += 1;
                currentSegment.moves.push_back(current);

            } else {
                bool exhausted = false;
                while(shouldPop && !exhausted) {        
                    // There is a need to abandon the current segment and to return to the one before
                    shouldPop = false;

                    // Skip all previously abandonne segments (not containing any moves)
                    while(!segments.empty() && segments.back().moves.empty()) {
                        segments.pop_back();
                    }
                    if(segments.empty()) {
                        // Nothing left, we are done
                        exhausted = true;
                        break;
                    }

                    // What direction should we take, knowing that it is evaluated in the order: Forward, Clockwise, CounterClockwise?
                    // -> Compare the direction we were going at the end of segment before last (atEndPrevLast)
                    //    with the direction we were going at the beginning of last segment (atBeginLast)
                    // Capture their respective values
                    auto atEndPrevLast = start;
                    auto atBeginLast = segments.back().origin;
                    auto atBeginLastDirection = segments.back().moves.front().direction;    // This is the direction we had chosen after the last segment
                    if(segments.size()>1) {     // If there are 2 or more  segments, then it was the end position of the segments before (as the beginning position of the current segment is already oriented possibly in a different direction)
                        auto i = segments.end() - 2;
                        atEndPrevLast = i->end;
                    } else 
                    if(segments.size() == 1){   // If there is only one segment, then take the "start" 
                        atEndPrevLast = start;
                    }
                    // Drop the last segment
                    segments.pop_back();

                    // Chose the "next" direction option
                    auto options = maze.Options(atEndPrevLast.position,atEndPrevLast.direction);
                    if(options.empty()) {
                        throw std::runtime_error("Unexpected state, this shouldn't be a dead end");
                    }
                    Compass choice;
                    for(auto c = options.begin(); c < options.end(); c++ ) {
                        if(*c == atBeginLastDirection /*atBeginLast.direction*/) {
                            // *c is the option we took initially,  last time; take the next one (if any)
                            if(c < options.end() - 1) {
                                choice = *(c + 1);
                            }
                            break;
                        }
                    }
                    if(choice == Compass::UNKNOWN) {
                        // No suitable option, we should pop again, immediatelly
                        shouldPop = true;
                        continue;
                    }

                    // Move the runner at the segment's end
                    current = atEndPrevLast;

                    // Update its orientation
                    if(current.direction != choice) {
                        current.score += 1000;
                        current.direction = choice;
                    }

                    // Move the runner one step ahead in the chosen direction
                    current.position = maze.Move(current.position,current.direction);
                    current.score += 1;
                    currentSegment.moves.push_back(current);
                }
                if(exhausted) {
                    // All options/segments exhausted; leave
                    break;
                }
            }
        } else {
            // Just a regular tile; move ahead

            // Update the orientation
            auto options = maze.Options(current.position,current.direction);
            if(options.empty()) {
                throw std::runtime_error("Unexpected state, this shouldn't be a dead end");
            }
            if(options.front() != current.direction) {
                current.score += 1000;
                current.direction = options.front();
            }

            // Move the runner one step ahead in the chosen direction
            current.position = maze.Move(current.position,current.direction);
            current.score += 1;
            currentSegment.moves.push_back(current);
        }

    }

    return bestScore;
}

long MazeRunner::Run2(std::function<void(std::stack<MazeSegment>,long)> exitCallback) {
    Reset();
    PushSegment();

    while(true) {
        std::cout << "\033[7;1m;[---> LOOP (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;

        if(IsExit()) {
            std::cout << "\033[7;1;33m;[---> IS EXIT\033[0m." << std::endl;
            if(!OnExitReached(exitCallback)) {
                break;
            }
            continue;
        }
        if(IsStart() && CountSegment() > 1) {
            std::cout << "\033[7;1;32m;[---> IS START\033[0m." << std::endl;
            if(!OnLoopDetected()) {
                break;
            }
            continue;
        }
        if(IsDeadEnd()) {
            std::cout << "\033[7;1;32m;[---> IS DEAD END\033[0m." << std::endl;
            if(!OnDeadEndReached()) {
                break;
            }
            continue;
        }
        
        Compass choice = Compass::UNKNOWN;
        if(IsCrosspoint()) {
            std::cout << "\033[7;1;31m;[---> IS CROSS-POINT\033[0m." << std::endl;
            if(!OnCrosspointReached()) {
                break;
            }
/*
            if(!justRestored) {
            }
*/            
            // Take (and remove) the first available option
            choice = crosspoints.back().EraseOption();
        } else {
            // Take the only available option
            choice = Options().front();
        }

        justRestored = false;

        // Progress in the maze & update the score
        MazeRunnerPosition oldpos = current;
        MazeRunnerPosition newpos = {maze.Move(oldpos.position, choice), choice, current.score};
        if(!OnTileReached(newpos, oldpos)) {
            break;
        }
        continue;
    }
    return bestScore;
}

bool MazeRunner::OnCrosspointReached() {
    std::cout << "\033[7;1;31m;[CROSS-POINT DETECTED]\033[0m." << std::endl;
    // If it is a new crosspoint, save it
    if(!CrosspointExists()) {
        std::cout << "\033[7;1;31m;[CROSS-POINT DETECTED:: NEW]\033[0m." << std::endl;
        PushSegment();
        PushCrosspoint();
        return true;
    } 
    
    if(justRestored) {
        std::cout << "\033[7;1;31m;[CROSS-POINT:: IN-RESTORE]\033[0m." << std::endl;
        // Otherwise check if there are any options left and use it; or pop the crosspoint before
        while(CrosspointExists() && crosspoints.back().options.empty()) {
            PopSegment();
            PopCrosspoint();
        }
        return !CrosspointExists();
    } else {
        // If we are not in the middle of a restoration, it means we have reached a loop
        return OnLoopDetected();
    }
}
bool MazeRunner::OnExitReached(std::function<void(std::stack<MazeSegment>,long)> callback) {
    std::cout << "\033[7;1;31m;[EXIT DETECTED]\033[0m." << std::endl;
    // Close the last Segment, and call the callback
    PushSegment();
    callback(segments, current.score);
    PopSegment();

    // Then return to the last crosspoint to process other path
    try {
        PopSegment();
        PopCrosspoint();
    } catch(std::runtime_error) {
        return false;
    }
    return true;
}
bool MazeRunner::OnDeadEndReached() {
    std::cout << "\033[7;1;31m;[DEAD END DETECTED]\033[0m." << std::endl;
    try {
        PopSegment();
        PopCrosspoint();
    } catch(std::runtime_error) {
        return false;
    }
    return true;
}
bool MazeRunner::OnLoopDetected() {
    std::cout << "\033[7;1;31m;[LOOP DETECTED]\033[0m." << std::endl;
    try {
        PopSegment();
        PopCrosspoint();
    } catch(std::runtime_error) {
        return false;
    }
    return true;
}
bool MazeRunner::OnTileReached(MazeRunnerPosition & cur, const MazeRunnerPosition & prev) {
    // Update the score
    if(cur.position != prev.position) {
        cur.score += 1;
    }
    if(cur.direction != prev.direction) {
        std::cout << "\033[27m" << cur.direction;
        cur.score += 1000;
    }
    // Update the "current" position
    current = cur;
    currentSegment.moves.push_back(current);

    // Interrupt if the score gets too high
    return cur.score <= bestScore;
}



bool MazeRunner::CrosspointExists() const {
    return std::find_if(crosspoints.begin(), crosspoints.end(), [this](const MazeRunnerCrosspoint & c) {
        return c.origin.position == current.position;
    }) != crosspoints.end();
}
void MazeRunner::PushCrosspoint(){
    std::cout << "\033[7;1;33m;[PUSH CP]\033[0m." << std::endl;
    crosspoints.push_back({current,Options()});
}
MazeRunnerCrosspoint MazeRunner::PopCrosspoint() {
    std::cout << "\033[7;1;32m;[POP CP]\033[0m." << std::endl;
    if(crosspoints.empty()) {
        throw std::runtime_error("No crosspoints to pop");
    }
    MazeRunnerCrosspoint cp = crosspoints.back();
    crosspoints.pop_back();
    current = cp.origin;

    justRestored = true;
    return cp;
}

void MazeRunner::PushSegment() {
    if(segments.empty()) {
        std::cout << "\033[7;1;33m;[PUSH SEGMENT: INITIAL]\033[0m." << std::endl;
        MazeSegment segment = {current, current, std::vector<MazeRunnerPosition>()};
        segments.push(segment);
    } else {
        std::cout << "\033[7;1;33m;[PUSH SEGMENT]\033[0m." << std::endl;
        MazeSegment segment = currentSegment;
        segment.end = current;
        segments.push(segment);
    }
    currentSegment = {current, current, std::vector<MazeRunnerPosition>()};
}
MazeSegment MazeRunner::PopSegment() {
    std::cout << "\033[7;1;32m;[POP SEGMENT]\033[0m." << std::endl;
    currentSegment = segments.top();
    segments.pop();
    return currentSegment;
}
size_t MazeRunner::CountSegment() {
    return segments.size();
}
MazeRunnerPosition MazeRunner::GetPosition() const {
    return current;
}

MazeRunner::MazeRunner(const Maze & map) {
    maze = map;
    Reset();
}

long MazeRunner::Run(std::function<void(std::stack<MazeSegment>,long)> callback) {
    Reset();

    while(true) {
        while(true) {
            if(IsExit()) {
                std::cout << "\033[7;31m;[---> EXIT FOUND (" << current.position.first << ", " << current.position.second << ") \033[0m." << std::endl;
                // Exit reached
                // Close and push the last segment (it is popped immediatelly after, at the end of this block)
                currentSegment.end = current;
                segments.push(currentSegment);

                // Update the best path and counter
                if(current.score < bestScore) {
                    std::cout << "\033[7;34m;[---> BEST SCORE (" << current.position.first << ", " << current.position.second << ") \033[0m." << std::endl;
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
                std::cout << "\033[7;32m;[---> START (" << current.position.first << ", " << current.position.second << ") STARTING POINT (LOOP).\033[0m." << std::endl;
                // We have returned to the starting point, go back to the last crosspoint, and try another path (by removing the first path that we had initially tried)                
                if(!RestoreFromLastCrosspoint()) {
                    break;
                } else {
                    continue;
                }
            }

            if(IsDeadEnd()) {
                std::cout << "\033[7;31m;[---> DEAD END (" << current.position.first << ", " << current.position.second << ") \033[0m." << std::endl;
                // Dead end reached, go back to the last crosspoint, and try another path (by removing the first path that we had initially tried)
                if(!RestoreFromLastCrosspoint()) {
                    break;
                } else {
                    continue;
                }
            }
            
            if(IsCrosspoint()) {
                std::cout << "\033[7;33m[---> ON A CROSS-POINT (" << current.position.first << ", " << current.position.second << ") \033[0m." << std::endl;
                if(IsStart()) {
                    std::cout << "\033[7;31m       >> STARTING POINT << \033[0m." << std::endl;
                    // Cross point at the starting point, we should only add it once
                    if(crosspoints.empty()) {
                        SaveCrosspoint();
                    }
                } else {
                    // Regular Crosspoint reached, it should be saved

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
            std::cout << "\033[7;31m;[---> LEAVING" << std::endl;
            std::cout << "(" << current.position.first << ", " << current.position.second << ") ALL CROSSPOINTS HAVE BEEN TRAVERSED." << std::endl;
            std::cout << "\033[0m";
            // No, let's leave
            break;
        }

        // Yes, restore position at the previous cross point
        std::cout << "\033[7;31m;[>>>>> EXPLORE OTHER PATH\033[0m;" << std::endl;
        
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
    std::cout << "\033[7;32m[---> PUSH CROSSPOINT & SEGMENTT (" << current.position.first << ", " << current.position.second << " [" << current.direction << "], [" << current.score << "], [Options: ";
    for(auto opt : Options()) {
        std::cout << opt << ", ";
    }
    std::cout << "]" << "\033[0m" << std::endl;

    // New crosspoint
    crosspoints.push_back({current,Options()});
DumpCrosspoint(crosspoints.back(), "\033[43m");

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
DumpSegment(currentSegment,"\033[42m");
    segments.push(currentSegment);
    std::cout << "\033[32m;[---> CURRENT SEGMENT : NEW]\033[0m;" << std::endl;
    currentSegment = {current, MazeRunnerPosition(), std::vector<MazeRunnerPosition>()};
DumpSegment(segments);
DumpSegment(currentSegment,"\033[43m");

/*
##### IL Y A DES SEGMENTS EN DOUBLE!!!
*/
}

bool MazeRunner::RestoreFromLastCrosspoint() {
    if(crosspoints.empty()) {
        std::cout << "POP CROSSPOINT > \033[7;31mNO CROSSPOINTS!\033[0m" << std::endl;
        return false;
    }
    auto lastCp = crosspoints.back();
DumpCrosspoint(crosspoints.back(), "\033[42m");
    
    
    // Does the last crosspoint have any options left?
    if(lastCp.options.empty()) {
        std::cout << "POP CROSSPOINT > \033[7;31mCrosspoint has NO MORE OPTIONS\033[0m" << std::endl;
        std::cout << "POPPED > CROSSPOINT HAD NO OPTIONS LEFT" << std::endl;

DumpSegment(segments.top(),"\033[41m");
DumpCrosspoint(crosspoints.back(), "\033[41m");

        current = crosspoints.back().origin;
        currentSegment = segments.top();
        crosspoints.pop_back();
        segments.pop();

DumpSegment(segments);
DumpSegment(currentSegment,"\033[43m");
        
        return RestoreFromLastCrosspoint();
    }

    // Update the current position, remove the last option from the crosspoint
    current = crosspoints.back().origin;
    currentSegment = segments.top();

DumpCrosspoint(crosspoints.back(), "\033[43m");
DumpSegment(currentSegment,"\033[43m");

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
    return Options(forward, clockwise, counter_clockwise, reverse) == 1 && reverse; // If there is only one option, and it is the reverse direction, then it is a dead end
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

Compass MazeRunnerCrosspoint::GetOption() {
    if(options.empty()) {
        return Compass::UNKNOWN;
    }
    return options.front();
}
Compass MazeRunnerCrosspoint::EraseOption() {
    if(options.empty()) {
        return Compass::UNKNOWN;
    }
    Compass choice = options.front();
    options.erase(options.begin());
    return choice;
}

void MazeRunner::Reset() {
    current = {maze.Start(), Compass::EAST, 0};
    crosspoints.clear();
    segments = std::stack<MazeSegment>();
    currentSegment = {current, current, std::vector<MazeRunnerPosition>()};
    bestScore = LONG_MAX;
    bestPath = std::stack<MazeSegment>();
}

void MazeRunner::DumpSegment(const std::stack<MazeSegment> & segments, std::string ansi) {
    auto copy = segments;
    while(!copy.empty()) {
        DumpSegment(copy.top(),ansi);
        copy.pop();
    }
}

/*
//template<typename C, typename T = typename C::value_type>
template<typename C, typename T = std::decay_t<decltype(*begin(std::declval<C>()))>>
void MazeRunner::DumpSegment(C const & c, std::string ansi) {
    for(auto s : c) {
        DumpSegment(s);
    }
}
*/

void MazeRunner::DumpSegment(const MazeSegment & segment, std::string ansi) {
    std::cout << ansi;

    std::cout << "\033[7m" << std::endl;
    std::cout << "FROM  : ";
    std::cout << "\033[1m(" << std::setw(5) << segment.origin.position.first << ", " << std::setw(5) << segment.origin.position.second << ") \033[22m";
    std::cout << "[" << segment.origin.direction << "]";
    std::cout << "[" << std::setw(10) << segment.origin.score << "]";
    std::cout << "\033[27m" << std::endl;
    if(!segment.moves.empty()) {
        for(auto m: segment.moves) {
            std::cout << "\t";
            std::cout << "\033[1m(" << std::setw(5) << m.position.first << ", " << std::setw(5) << m.position.second << ") \033[22m";
            std::cout << "[" << m.direction << "]";
            std::cout << "[" << std::setw(10) << m.score << "]";
            std::cout << std::endl;
        }
    }
    std::cout << "\033[7m";
    std::cout << "TO    : ";
    std::cout << "\033[1m(" << std::setw(5) << segment.end.position.first << ", " << std::setw(5) << segment.end.position.second << ") \033[22m";
    std::cout << "[" << segment.end.direction << "]";
    std::cout << "[" << std::setw(10) << segment.end.score << "]";
    std::cout << "\033[27m" << std::endl;

    std::cout << "\033[0m" << std::endl;
}

static void DumpCrosspoint(const std::vector<MazeRunnerCrosspoint> & crosspoint, std::string ansi){
    for(auto cp = crosspoint.begin(); cp < crosspoint.end(); cp++) {
        MazeRunner::DumpCrosspoint(*cp, ansi);
    }
}

void MazeRunner::DumpCrosspoint(const MazeRunnerCrosspoint & crosspoint, std::string ansi) {
    std::cout << ansi;

    std::cout << "\033[7m" << std::endl;
    std::cout << "X-POINT:";
    std::cout << "\033[1m(" << std::setw(5) << crosspoint.origin.position.first << ", " << std::setw(5) << crosspoint.origin.position.second << ") \033[22m";
    std::cout << "[" << crosspoint.origin.direction << "]";
    std::cout << "[" << std::setw(10) << crosspoint.origin.score << "]";
    std::cout << "[";
    if(!crosspoint.options.empty()) {
        for(auto m: crosspoint.options) {
            std::cout << "\033[7;1m" << m << "\033[27;22m ";
        }
    }
    std::cout << "]";
    std::cout << "\033[0m" << std::endl;
}

std::ostream & operator<<(std::ostream & os, const MazeRunnerCrosspoint & cp) {
    os << "\033[7;1mCROSSPOINT\033[27;22m" << std::endl;
    os << "\033[1m(" << std::setw(5) << cp.origin.position.first << ", " << std::setw(5) << cp.origin.position.second << ") \033[22m";
    os << "[" << cp.origin.direction << "]";
    os << "[" << std::setw(10) << cp.origin.score << "]";
    os << "[";
    if(!cp.options.empty()) {
        for(auto m: cp.options) {
            os << "\033[7;1m" << m << "\033[27;22m ";
        }
    }
    os << "]";
    return os;
}

std::ostream & operator<<(std::ostream & os, const MazeRunnerPosition & p) {
    os << "\033[7;1mPOSITION\033[27;22m" << std::endl;
    os << "\033[1m(" << std::setw(5) << p.position.first << ", " << std::setw(5) << p.position.second << ") \033[22m";
    os << "[" << p.direction << "]";
    os << "[" << std::setw(10) << p.score << "]";
    return os;
}

std::ostream & operator<<(std::ostream & os, const MazeSegment & s){
    os << "\033[7;1mSEGMENT\033[27;22m" << std::endl;
    os << "\033[7m" << std::endl;
    os << "FROM  : ";
    os << "\033[1m(" << std::setw(5) << s.origin.position.first << ", " << std::setw(5) << s.origin.position.second << ") \033[22m";
    os << "[" << s.origin.direction << "]";
    os << "[" << std::setw(10) << s.origin.score << "]";
    os << "\033[27m" << std::endl;
    if(!s.moves.empty()) {
        for(auto m: s.moves) {
            os << "\t";
            os << "\033[1m(" << std::setw(5) << m.position.first << ", " << std::setw(5) << m.position.second << ") \033[22m";
            os << "[" << m.direction << "]";
            os << "[" << std::setw(10) << m.score << "]";
            os << std::endl;
        }
    }
    os << "\033[7m";
    os << "TO    : ";
    os << "\033[1m(" << std::setw(5) << s.end.position.first << ", " << std::setw(5) << s.end.position.second << ") \033[22m";
    os << "[" << s.end.direction << "]";
    os << "[" << std::setw(10) << s.end.score << "]";
    os << "\033[27m";
    return os;
}
