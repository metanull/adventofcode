#include "MazeRunner.h"

/**
 * this is an obsolete and deprecated method, see Run instead
 */
// long MazeRunner::__Run_Backup(std::function<bool(long long,const std::vector<MazeSegment> &)> cb) {   // (std::function<void(std::vector<MazeSegment>,long)> exitCallback) {
long MazeRunner::__Run_Backup() {
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