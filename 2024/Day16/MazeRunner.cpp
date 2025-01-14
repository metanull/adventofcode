#include "MazeRunner.h"

long MazeRunner::Run2(std::function<void(std::stack<MazeSegment>,long)> callback) {
    Reset();

    long localbestscore = LONG_MAX;
    while(true) {
        if(IsExit()) {
            if(current.score < localbestscore) {
                localbestscore = current.score;
            }
            if(!OnExit(callback)) {
                break;
            }
            continue;
        }
        if(IsStart()) {
            if(!OnStart()) {
                break;
            }
            //continue;
        }
        if(IsDeadEnd()) {
            if(!OnDeadEnd()) {
                break;
            }
            continue;
        }
        
        Compass choice = Compass::UNKNOWN;
        if(IsCrosspoint()) {
            if(!OnCrosspoint()) {
                break;
            }
            
            choice = UseCrosspoint();
            if(choice == Compass::UNKNOWN) {
                if(!OnCrosspoint()) {
                    break;
                }
                continue;
            }
        } else {
            choice = Options().front();
        }

        Move2(choice);
    }
    return localbestscore;
}

long MazeRunner::Move2(Compass choice) {
    if(choice == Compass::UNKNOWN) {
        throw std::runtime_error("Invalid choice");
    }
    long localscore = (choice == current.direction) ? 1 : 1001;
    current.score += localscore;
    current.position = maze.Move(current.position, current.direction);
    currentSegment.moves.push_back(current);
    return localscore;
}

Compass MazeRunner::UseCrosspoint() {
    if(!CrosspointExists()) {
        throw std::runtime_error("No crosspoints to use");
    }
    return crosspoints.back().EraseOption();
}

void MazeRunner::OnError(std::runtime_error & e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
}

bool MazeRunner::OnCrosspoint() {
    try {
        if(CrosspointExists()) {
            PopSegment();
            PopCrosspoint();
        } else {
            PushSegment();
            PushCrosspoint();
        }
    } catch(std::runtime_error & e) {
        return false;
    }
    return true;
}
bool MazeRunner::OnExit(std::function<void(std::stack<MazeSegment>,long)> callback) {
    PushSegment();
    callback(segments, current.score);
    PopSegment();

    try {
        PopSegment();
        PopCrosspoint();
    } catch(std::runtime_error & e) {
        return false;
    }
    return true;
}
bool MazeRunner::OnDeadEnd() {
    try {
        PopSegment();
        PopCrosspoint();
    } catch(std::runtime_error & e) {
        return false;
    }
    return true;
}
bool MazeRunner::OnStart() {
    if(IsCrosspoint()) {
        if(CrosspointExists()) {
            try {
                PopSegment();
                PopCrosspoint();
            } catch(std::runtime_error & e) {
                return false;
            }
        } else {
            PushSegment();
            PushCrosspoint();
        }
    } else {
        PushSegment();
    }
    return true;
}

bool MazeRunner::CrosspointExists() const {
    return std::find_if(crosspoints.begin(), crosspoints.end(), [this](const MazeRunnerCrosspoint & c) {
        return c.origin.position == current.position;
    }) != crosspoints.end();
}
void MazeRunner::PushCrosspoint(){
    crosspoints.push_back({current,Options()});
}
MazeRunnerCrosspoint MazeRunner::PopCrosspoint() {
    if(crosspoints.empty()) {
        throw std::runtime_error("No crosspoints to pop");
    }
    MazeRunnerCrosspoint cp = crosspoints.back();
    crosspoints.pop_back();
    current = cp.origin;
    return cp;
}

void MazeRunner::PushSegment() {
    if(segments.empty()) {
        MazeSegment segment = {current, current, std::vector<MazeRunnerPosition>()};
        segments.push(segment);
    } else {
        MazeSegment segment = currentSegment;
        segment.end = current;
        segments.push(segment);
    }
    currentSegment = {current, current, std::vector<MazeRunnerPosition>()};
}
MazeSegment MazeRunner::PopSegment() {
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

/*
std::ostream & MazeRunnerCrosspoint::operator<<(std::ostream & os) const {
    os << "\033[7;1mCROSSPOINT\033[27;22m" << std::endl;
    os << "\033[1m(" << std::setw(5) << origin.position.first << ", " << std::setw(5) << origin.position.second << ") \033[22m";
    os << "[" << origin.direction << "]";
    os << "[" << std::setw(10) << origin.score << "]";
    os << "[";
    if(!options.empty()) {
        for(auto m: options) {
            os << "\033[7;1m" << m << "\033[27;22m ";
        }
    }
    os << "]";
    return os;
}

std::ostream & MazeRunnerPosition::operator<<(std::ostream & os) const {
    os << "\033[7;1mPOSITION\033[27;22m" << std::endl;
    os << "\033[1m(" << std::setw(5) << position.first << ", " << std::setw(5) << position.second << ") \033[22m";
    os << "[" << direction << "]";
    os << "[" << std::setw(10) << score << "]";
    return os;
}

std::ostream & MazeSegment::operator<<(std::ostream & os) const {
    os << "\033[7;1mSEGMENT\033[27;22m" << std::endl;
    os << "\033[7m" << std::endl;
    os << "FROM  : ";
    os << "\033[1m(" << std::setw(5) << origin.position.first << ", " << std::setw(5) << origin.position.second << ") \033[22m";
    os << "[" << origin.direction << "]";
    os << "[" << std::setw(10) << origin.score << "]";
    os << "\033[27m" << std::endl;
    if(!moves.empty()) {
        for(auto m: moves) {
            os << "\t";
            os << "\033[1m(" << std::setw(5) << m.position.first << ", " << std::setw(5) << m.position.second << ") \033[22m";
            os << "[" << m.direction << "]";
            os << "[" << std::setw(10) << m.score << "]";
            os << std::endl;
        }
    }
    os << "\033[7m";
    os << "TO    : ";
    os << "\033[1m(" << std::setw(5) << end.position.first << ", " << std::setw(5) << end.position.second << ") \033[22m";
    os << "[" << end.direction << "]";
    os << "[" << std::setw(10) << end.score << "]";
    os << "\033[27m";
    return os;
}
*/