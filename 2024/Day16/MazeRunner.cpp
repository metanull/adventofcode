#include "MazeRunner.h"


MazeRunner::MazeRunner(const Maze & map) {
    maze = map;
    start = std::chrono::high_resolution_clock::now();
}


// long MazeRunner::Run(std::function<bool(long long,const std::vector<MazeSegment> &)> cb) {
long MazeRunner::Run(std::string banner, std::function<std::vector<Compass>(const std::vector<Compass>&)> choiceOrder) {
    long long n = 0;
    std::vector<MazeSegment> segments;
    long maxSegments = 0;

    MazeRunnerPosition runner = {maze.Start(), Compass::EAST, 0};
    MazeSegment currentSegment = {runner, runner, std::vector<MazeRunnerPosition>()};
    
    bool depleted = false;
    try {
        // while(cb(n,segments)) {
        while(true) {
            n++;
            
            // Are we just starting?
            bool isFirstMove = segments.size() == 0 && currentSegment.moves.empty();

            // Is it a regular tile or a special tile (START,END,CROSS-POINT,DEAD-END)
            bool isRegularTile = !(
                maze.IsCrossPoint(runner.position,runner.direction)
                || maze.IsExit(runner.position) 
                || maze.IsStart(runner.position) 
                || maze.IsDeadEnd(runner.position,runner.direction)
            );

            if(!isRegularTile && !isFirstMove) {
                // close the current segment
                currentSegment.end = runner;
                segments.push_back(currentSegment);
                currentSegment = {runner,runner,std::vector<MazeRunnerPosition>()};

                bool shouldJumpBack = false;

                // Handle "exit reached"
                if(maze.IsExit(segments.back().end.position)) {
                    bool isBest = false;
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        if(segments.back().end.score < bestScore) {
                            bestPath = segments;
                            bestScore = segments.back().end.score;
                            isBest = true;
                        }
                    }
                    shouldJumpBack = true;

                    if(segments.size() > maxSegments) {
                        maxSegments = (long)segments.size();
                    }
                    ProgressBar(banner, n, start, (long)segments.size(), (long)maxSegments, segments.back().end.score, bestScore, 100, isBest);
                } else {
                    // Avoid loops; dead ends; return to start
                    if( maze.IsStart(segments.back().end.position)) {
                        shouldJumpBack = true;
                    } else
                    if( maze.IsDeadEnd(segments.back().end.position, segments.back().end.direction)) {
                        shouldJumpBack = true;

                        CondemnLastSegment(segments);

                        /*// Fill the dead end with a wall
                        for(auto d = segments.back().moves.begin() + 1; d < segments.back().moves.end(); d++) {
                            maze.SetTile(d->position,maze.WALL_CHAR);
                        }*/
                    } else {
                        shouldJumpBack = DetectLoop(segments, segments.back());
                    }

                    // Avoid score overflow
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        if(segments.back().end.score > bestScore) {
                            shouldJumpBack = true;
                        }
                    }
                }

                // Jump back or Walk ahead
                if(shouldJumpBack) {
                    currentSegment = JumpBack(segments,choiceOrder);
                    runner = currentSegment.moves.front();
                } else {
                    runner = Walk(runner,choiceOrder);
                    currentSegment.moves.push_back(runner);
                }
            } else {
                runner = Walk(runner,choiceOrder);
                currentSegment.moves.push_back(runner);
            }
        }
    } catch( std::runtime_error & e) {
        std::cerr << e.what() << std::endl;
    }

    return bestScore;
}

MazeRunnerPosition MazeRunner::Step(const MazeRunnerPosition & r, Compass d) const {
    MazeRunnerPosition current(r);
    if(d != current.direction) {
        current.score += SCORE_TURN;
        current.direction = d;
    }
    // Move the runner one step ahead in the chosen direction
    current.position = maze.Move(current.position, current.direction);
    current.score += SCORE_STEP;
    return current;
}
void MazeRunner::CondemnLastSegment(std::vector<MazeSegment> & segments) {
    for(auto d = segments.back().moves.begin() + 1; d < segments.back().moves.end(); d++) {
        maze.SetTile(d->position,maze.WALL_CHAR);
    }
}
MazeRunnerPosition MazeRunner::Walk(const MazeRunnerPosition & r, std::function<std::vector<Compass>(const std::vector<Compass>&)> choiceOrder) {
    MazeRunnerPosition current(r);
    // Update the orientation
    auto options = maze.Options(current.position, current.direction);
    if(options.empty()) {
        // Impossible to Walk, presumably, this path is being closed as a "dead-end" by another thread.
        // Do nothing; it "Run" method shall pick-up the situation and jump back to a previous segment

        std::cerr << "Walk(): Walked into a dead end (another thread is probably closing the path)" << std::endl;
        return current;

        // throw std::runtime_error("BANG! Walked into a dead end - this is unexpected");
    }
    // Apply the callback to modify the order of the options if needed
    options = choiceOrder(options);
    return Step(current, options.front());
}

MazeSegment MazeRunner::JumpBack(std::vector<MazeSegment> & segments, std::function<std::vector<Compass>(const std::vector<Compass>&)> choiceOrder) {
    while(!segments.empty() && segments.back().moves.empty()) {
        segments.pop_back();
    }
    if(segments.empty()) {
        throw std::runtime_error("Depleted");
    }

    // Abandon last segment, jump to where we were right before this one
    MazeRunnerPosition newOrigin = {maze.Start(), Compass::EAST, 0};                     // Unless if there are segments in the list, take start as the new origin
    auto oldDirection = segments.back().moves.front().direction;    // This is the direction we had chosen after the last segment
    if(segments.size()>1) {                     // If there are 2 or more segments, then take the "end" of the segment before last as the new origin
        auto i = segments.end() - 2;            // Note we can't use the last segment's start as a new origin, as a rotation may have occured between the end of end-2 and the begin of n-1
        newOrigin = i->end;
    }
    segments.pop_back();

    // Chose the "next" direction option
    auto options = maze.Options(newOrigin.position,newOrigin.direction);
    if(options.empty()) {                       // This test is not strictly needed, as we are returning to a know point that was already tested
        throw std::runtime_error("Unexpected state; the new origin should not be a dead end.");
    }
    // Apply the callback to modify the order of the options if needed
    options = choiceOrder(options);
    Compass choice;
    for(auto c = options.begin(); c < options.end(); c++ ) {
        if(*c == oldDirection) {
            if(c < options.end() - 1) { // "*c" is the option that was chosen last time...
                choice = *(c + 1);      // ... so, select the next option (if any)
            }
            break;
        }
    }
    if(choice == Compass::UNKNOWN) {
        // No suitable option, jumb further back
        return JumpBack(segments,choiceOrder);
    }
    // Update the orientation as per the choice

    MazeSegment s = {newOrigin,newOrigin,std::vector<MazeRunnerPosition>()};
    s.moves.push_back(Step(newOrigin,choice));
    return s;
}

bool MazeRunner::DetectLoop(std::vector<MazeSegment> & segments, const MazeSegment & needle) {
    auto sff = std::find_if(segments.begin(), segments.end(), [this,needle](const MazeSegment & s) {
        // Look for identical segment, if one is found, this is a loop
        return needle.origin.position == s.origin.position && needle.end.position == s.end.position;
    });
    if(sff != segments.end() && sff < segments.end() - 1) {
        return true;
    }
    auto srw = std::find_if(segments.begin(), segments.end(), [this,needle](const MazeSegment & s) {
        // Look for identical segment (in reverse direction), if one is found, this is a loop
        return needle.origin.position == s.end.position && needle.end.position == s.origin.position;
    });
    if(srw != segments.end() && srw < segments.end() - 1) {
        return true;
    }
    return false;
}

void MazeRunner::ProgressBar(std::string & banner, long long n, std::chrono::steady_clock::time_point start, long numSegments, long maxSegments, long score, long bestScore, int w, bool isBest) {

    int pc = (int)(w * ((float)numSegments / (float)maxSegments));
    int wpc  = std::min((w-1),pc);
    int wlft = std::max((1),(w - pc));
    
    std::stringstream ss;
    // ss << "\033[1F"
    ss << std::setfill(' ')
        << std::setw(12) << std::this_thread::get_id() 
        << " - "
        << std::setw(12) << banner
        << " - "
        << " "
        << std::setw(9) << (n++)
        << " "
        << "\033[34;1m" << std::setw(5) << std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start).count() << " min."
        << " ";
    if(isBest) {
        ss << "\033[42m" << std::setw(wpc) << ' ' << "\033[0m"
            << "\033[41m" << std::setw(wlft) << ' ' << "\033[0m"
            << " "
            << "\033[27;33;1m"  << std::setw(6) << score << "\033[0m";
    } else {
        ss << "\033[44m" << std::setw(wpc) << ' ' << "\033[0m"
            << "\033[43m" << std::setw(wlft) << ' ' << "\033[0m"
            << " "
            << "\033[27;34m"  << std::setw(6) << score << "\033[0m";
    }
    ss << " - "
        << "\033[7;1;32m" << std::setw(6) << bestScore << "\033[0m"
        << ", "
        << " " << std::setw(3) << numSegments
        << " - "
        << "\033[27;35m"  << std::setw(3) << maxSegments << "\033[0m"
        << std::endl;
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << ss.str();
    }
}