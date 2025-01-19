#include "MazeRunner.h"


MazeRunner::MazeRunner(const Maze & map) {
    maze = map;
    long bestScore = UPPERLIMIT;
}


// long MazeRunner::Run(std::function<bool(long long,const std::vector<MazeSegment> &)> cb) {
long MazeRunner::Run() {
    long long n = 0;
    std::vector<MazeSegment> segments;
    long bestScore = UPPERLIMIT;
    long maxSegments = 0;
    start = std::chrono::high_resolution_clock::now();

    MazeRunnerPosition runner = {maze.Start(), Compass::EAST, 0};
    MazeSegment currentSegment = {runner, runner, std::vector<MazeRunnerPosition>()};
    std::vector<MazeSegment> bestPath;
    
    try {
    // while(cb(n,segments)) {
    while(true) {
        n++;
        
        #if defined(DEBUG) && DEBUG != 0
            if(segments.size() > maxSegments) {
                maxSegments = (long)segments.size();
            }
            ProgressBar(n, start, (long)segments.size(), (long)maxSegments, runner.score, bestScore, 100);
        #endif
        
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
                if(segments.back().end.score < bestScore) {
                    bestPath = segments;
                    bestScore = segments.back().end.score;
                }
                shouldJumpBack = true;

                #if !defined(DEBUG) || DEBUG == 0
                    if(segments.size() > maxSegments) {
                        maxSegments = (long)segments.size();
                    }
                    ProgressBar(n, start, (long)segments.size(), (long)maxSegments, segments.back().end.score, bestScore, 100);
                #endif
            } else {
                // Avoid loops; dead ends; return to start
                if( maze.IsStart(segments.back().end.position)) {
                    shouldJumpBack = true;
                } else
                if( maze.IsDeadEnd(segments.back().end.position, segments.back().end.direction)) {
                    shouldJumpBack = true;

                    // Fill the dead end with a wall
                    for(auto d = segments.back().moves.begin() + 1; d < segments.back().moves.end(); d++) {
                        maze.SetTile(d->position,maze.WALL_CHAR);
                    }
                } else {
                    shouldJumpBack = DetectLoop(segments, segments.back());
                }

                // Avoid score overflow
                if( segments.back().end.score > bestScore) {
                    shouldJumpBack = true;
                }
            }

            // Jump back or Walk ahead
            if(shouldJumpBack) {
                currentSegment = JumpBack(segments);
                runner = currentSegment.moves.front();
                /*
                runner = JumpBack(segments);
                currentSegment.origin = runner;
                currentSegment.moves.push_back(runner);
                */
            } else {
                runner = Walk(runner);
                currentSegment.moves.push_back(runner);
            }
        } else {
            runner = Walk(runner);
            currentSegment.moves.push_back(runner);
        }
//        currentSegment.moves.push_back(runner);
    }
    } catch( std::runtime_error & e) {
        std::cerr << e.what() << std::endl;
    }

    return bestScore;
}


MazeRunnerPosition MazeRunner::Step(const MazeRunnerPosition & r, Compass d) const {
    MazeRunnerPosition current(r);
    if(d != current.direction) {
        current.score += 1000;
        current.direction = d;
    }
    // Move the runner one step ahead in the chosen direction
    current.position = maze.Move(current.position, current.direction);
    current.score += 1;
    return current;
}
MazeRunnerPosition MazeRunner::Walk(const MazeRunnerPosition & r) const {
    MazeRunnerPosition current(r);
    // Update the orientation
    auto options = maze.Options(current.position, current.direction);
    if(options.empty()) {
        throw std::runtime_error("BANG! Walked into a dead end - this is unexpected");
    }
    return Step(current,options.front());
}

MazeSegment MazeRunner::JumpBack(std::vector<MazeSegment> & segments) {
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
        return MazeRunner::JumpBack(segments);
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

// long MazeRunner::Run3(std::function<bool(long long,const std::vector<MazeSegment> &)> cb) {   // (std::function<void(std::vector<MazeSegment>,long)> exitCallback) {
long MazeRunner::Run3() {
    std::vector<MazeSegment> segments;
    MazeRunnerPosition current = {maze.Start(), Compass::EAST, 0};
    MazeSegment currentSegment = {current, current, std::vector<MazeRunnerPosition>()};
    long bestScore = UPPERLIMIT;
    long maxSegments = 0;
    std::vector<MazeSegment> bestPath;
    long long n = 0;

    start = std::chrono::high_resolution_clock::now();

    // while(cb(n,segments)) {
    while(true) {
        n++;

        #if defined(DEBUG) && DEBUG != 0
            if(segments.size() > maxSegments) {
                maxSegments = (long)segments.size();
            }
            ProgressBar(n, start, (long)segments.size(), (long)maxSegments, current.score, bestScore, 100);
        #endif

        // Is the current segment closed?
        // bool closed = (currentSegment.origin.position != currentSegment.end.position);

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
            #if defined(DEBUG) && DEBUG != 0
                std::cout << "\033[1F" << std::setw(6) << segments.size() << " " << std::setw(6) << currentSegment.moves.size() << " " << std::setw(6) << current.score << "\033[0m" << std::endl;
                std::cout << "\033[7;1m;[---> SPECIAL TILE (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
            #endif
            

            // close the segment
            currentSegment.end = current;
            segments.push_back(currentSegment);
            currentSegment = {current,current,std::vector<MazeRunnerPosition>()};

            // process the segment
            bool shouldPop = false; // If true, after processing, pop/return to the previous segment

            // Found the exit
            if(maze.IsExit(currentSegment.end.position)) {
                // exitCallback(segments,current.score);

                if(current.score < bestScore) {
                    bestPath = segments;
                    bestScore = current.score;
                }
                shouldPop = true;

                #if !defined(DEBUG) || DEBUG == 0
                    if(segments.size() > maxSegments) {
                        maxSegments = (long)segments.size();
                    }
                    ProgressBar(n, start, (long)segments.size(), (long)maxSegments, current.score, bestScore, 100);
                #endif

            } else {
                // Anti loop - Anti Dead End - Anti return to start
                if(maze.IsStart(segments.back().end.position)) {
                    #if defined(DEBUG) && DEBUG != 0
                        std::cout << "\033[7;1m;[---> LOOP DETECTED (thru start) (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                    #endif
                    // Segment ends on the start tile. This is a loop
                    shouldPop = true;
                } else 
                if(maze.IsDeadEnd(segments.back().end.position,segments.back().end.direction)) {
                    #if defined(DEBUG) && DEBUG != 0
                        std::cout << "\033[31;1m;[---> DEAD END DETECTED (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                    #endif
                    // Segment ends on a dead end.
                    shouldPop = true;

                    // Fill the dead end with a wall
                    for(auto d = segments.back().moves.begin() + 1; d < segments.back().moves.end(); d++) {
                        maze.SetTile(d->position,maze.WALL_CHAR);
                    }
                } else {
                    auto sff = std::find_if(segments.begin(), segments.end(), [segments](const MazeSegment & s) {
                        // Look for identical segment
                        return segments.back().origin.position == s.origin.position && segments.back().end.position == s.end.position;
                    });
                    if(sff != segments.end() && sff < segments.end() - 1) {
                        #if defined(DEBUG) && DEBUG != 0
                            std::cout << "\033[7;1m;[---> LOOP DETECTED (thru forward segment) (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                        #endif
                        // Found an identical segment. This is a loop
                        shouldPop = true;
                    } else {
                        auto srw = std::find_if(segments.begin(), segments.end(), [segments](const MazeSegment & s) {
                            // Look for identical segment (in reverse direction)
                            return segments.back().origin.position == s.end.position && segments.back().end.position == s.origin.position;
                        });
                        if(srw != segments.end() && srw < segments.end() - 1) {
                            #if defined(DEBUG) && DEBUG != 0
                                std::cout << "\033[7;1m;[---> LOOP DETECTED (thru backward segment) (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                            #endif
                            // Found an identical segment (inverted). This is a loop
                            shouldPop = true;
                        }
                    }
                }
                // Speed enhancement
                if(current.score > bestScore) {
                    #if defined(DEBUG) && DEBUG != 0
                        std::cout << "\033[7;1m;[---> SCORE EXCEEDED DETECTED (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                        std::cout << "\033[7;31;1m;[---> SCORE: " << current.score << "\033[0m." << std::endl;
                    #endif
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
                // There is a need to abandon the current segment and to return to the one before
                bool exhausted = false;
                while(shouldPop && !exhausted) {     
                    #if defined(DEBUG) && DEBUG != 0
                        std::cout << "\033[7;33;1m;[---> POP REQUESTED\033[22;27m." << std::endl;
                    #endif

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
                    // -> Compare the direction we were going at the end of segment before last (newOrigin)
                    //    with the direction we were going at the beginning of last segment (oldOrigin)
                    // Capture their respective values
                    MazeRunnerPosition newOrigin = {maze.Start(), Compass::EAST, 0};
                    auto oldOrigin = segments.back().origin;
                    auto oldDirection = segments.back().moves.front().direction;    // This is the direction we had chosen after the last segment
                    if(segments.size()>1) {     // If there are 2 or more  segments, then it was the end position of the segments before (as the beginning position of the current segment is already oriented possibly in a different direction)
                        auto i = segments.end() - 2;
                        newOrigin = i->end;
                    }
                    // Drop the last segment
                    segments.pop_back();

                    // Chose the "next" direction option
                    auto options = maze.Options(newOrigin.position,newOrigin.direction);
                    if(options.empty()) {
                        throw std::runtime_error("Unexpected state, this shouldn't be a dead end");
                    }
                    Compass choice;
                    for(auto c = options.begin(); c < options.end(); c++ ) {
                        if(*c == oldDirection /*oldOrigin.direction*/) {
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

                    // Move the runner at the segment's end, update the currentSegment's origin accordingly
                    current = newOrigin;
                    currentSegment.origin = current; // -------------------------------------------------------<<<
                    #if defined(DEBUG) && DEBUG != 0
                        std::cout << "\033[7;32;1m;[---> POPPED @ (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                    #endif

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
                    #if defined(DEBUG) && DEBUG != 0
                        std::cout << "\033[7;1m;[---> SEGMENTS DEPLETED (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                    #endif
                    // All options/segments exhausted; leave
                    break;
                }
            }
        } else {
            // Just a regular tile; move ahead
            #if defined(DEBUG) && DEBUG != 0
                std::cout << "\033[1m;[---> REGULAR TILE (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
            #endif

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

//    exitCallback(segments,current.score);

    return bestScore;
}

void MazeRunner::ProgressBar(long long n, std::chrono::steady_clock::time_point start, long numSegments, long maxSegments, long score, long bestScore, int w) {
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);

    int pc = (int)(w * ((float)numSegments / (float)maxSegments));
    int wpc  = std::min((w-1),pc);
    int wlft = std::max((1),(w - pc));
    
    std::cout << "\033[1F"
    << std::setfill(' ')
    << std::setw(9) << (n++)
    << " "
    << "\033[34;1m" << std::setw(5) << std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start).count() << " min."
    << " "
    << "\033[44;34m" << std::setw(wpc) << ' ' << "\033[0m"
    << "\033[43;32m" << std::setw(wlft) << ' ' << "\033[0m"
    << " "
    << "\033[27;34m"  << std::setw(6) << score << "\033[0m"
    << " - "
    << "\033[7;1;32m" << std::setw(6) << bestScore << "\033[0m"
    << ", "
    << " " << numSegments
    << " - "
    << "\033[27;35m"  << std::setw(6) << maxSegments << "\033[0m"
    << std::endl;
}