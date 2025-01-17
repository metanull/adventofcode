#include "MazeRunner.h"
#include <algorithm>
#include <map>
#include <sstream>

#ifndef UPPERLIMIT
    #define UPPERLIMIT LONG_MAX
#endif

MazeRunner::MazeRunner(const Maze & map) {
    maze = map;
    
    segments.clear();
    start = {maze.Start(), Compass::EAST, 0};
    long bestScore = UPPERLIMIT;
}

long MazeRunner::Run() {
    long long n = 0;
    segments.clear();
    start = {maze.Start(), Compass::EAST, 0};
    long bestScore = UPPERLIMIT;
    long maxSegments = 0;

    MazeRunnerPosition runner = start;
    MazeSegment currentSegment = {runner, runner, std::vector<MazeRunnerPosition>()};
    std::vector<MazeSegment> bestPath;

    std::map<std::string, int> positionCount;
    
    try {
    while(true) {
            size_t w = 100;
            if(segments.size() > maxSegments) {
                maxSegments = segments.size();
            }            
            auto cappedpc =  ((float)(maxSegments - segments.size()) / 100);
            float pc = cappedpc;
            std::cout << "\033[1F"
             << "\033[44;34m" << std::setw(std::min((int)(w-(w*pc)),(int)w-1)) << std::setfill(' ') << ' '
             << "\033[43;32m" << std::setw((int)(w*pc)) << std::setfill(' ') << ' '
             << std::setfill(' ')
             << "\033[0m"
             << "\033[27;34m" << " " << std::setw(6) << runner.score
             << "\033[7;1;32m"  << " " << std::setw(6) << bestScore
             << "\033[27;35m"  << " " << std::setw(6) << maxSegments
             << "\033[22;27;0m" << " " << segments.size()
             << "\033[22;27;0m" << " " << (n++)
             << "\033[22;27;0m." << std::endl;
        
        // Are we just starting?
        bool isFirstMove = segments.size() == 0 && currentSegment.moves.empty();

        std::stringstream ss;
        ss << runner.position.first << "," << runner.position.second;
        if (positionCount.find(ss.str()) == positionCount.end()) {
            positionCount[ss.str()] = 1;
        } else {
            positionCount[ss.str()]++;
            if (positionCount[ss.str()] > 1000) {
                std::cout << "Runner has visited position (" << runner.position.first << ", " << runner.position.second << ") more than 30 times." << std::endl;
                // Add any additional logic you want to execute when the count is greater than 30
            }
        }

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
            } else {
                // Avoid loops; dead ends; return to start
                if( maze.IsStart(segments.back().end.position)) {
                    shouldJumpBack = true;
                } else
                if( maze.IsDeadEnd(segments.back().end.position, segments.back().end.direction)) {
                    shouldJumpBack = true;
                } else {
                    shouldJumpBack = DetectLoop(segments.back());
                }

                // Avoid score overflow
                if( segments.back().end.score > bestScore) {
                    shouldJumpBack = true;
                }
            }

            // Jump back or Walk ahead
            if(shouldJumpBack) {
                runner = JumpBack();
                currentSegment.origin = runner;
            } else {
                runner = Walk(runner);
            }
        } else {
            runner = Walk(runner);
        }
        currentSegment.moves.push_back(runner);
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
    current.position = maze.Move(current.position, d);
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
MazeRunnerPosition MazeRunner::JumpBack() {
    while(!segments.empty() && segments.back().moves.empty()) {
        segments.pop_back();
    }
    if(segments.empty()) {
        throw std::runtime_error("Depleted");
    }

    // Abandon last segment, jump to where we were right before this one
    auto newOrigin = start;                     // Unless if there are segments in the list, take start as the new origin
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
        return MazeRunner::JumpBack();
    }
    // Update the orientation as per the choice
    return Step(newOrigin,choice);
}

bool MazeRunner::DetectLoop(const MazeSegment & s) {
    auto sff = std::find_if(segments.begin(), segments.end(), [this](const MazeSegment & s) {
        // Look for identical segment, if one is found, this is a loop
        return segments.back().origin.position == s.origin.position && segments.back().end.position == s.end.position;
    });
    if(sff != segments.end() && sff < segments.end() - 1) {
        return true;
    }
    auto srw = std::find_if(segments.begin(), segments.end(), [this](const MazeSegment & s) {
        // Look for identical segment (in reverse direction), if one is found, this is a loop
        return segments.back().origin.position == s.end.position && segments.back().end.position == s.origin.position;
    });
    if(srw != segments.end() && srw < segments.end() - 1) {
        return true;
    }
    return false;
}

long MazeRunner::Run3(std::function<void(std::vector<MazeSegment>,long)> exitCallback) {
    std::vector<MazeSegment> segments;
    MazeRunnerPosition start = {maze.Start(), Compass::EAST, 0};
    MazeRunnerPosition current = start;
    MazeSegment currentSegment = {current, current, std::vector<MazeRunnerPosition>()};
    long bestScore = UPPERLIMIT;
    long maxSegments = 1;
    std::vector<MazeSegment> bestPath;
    long long n = 0;

    while(true) {
            size_t w = 100;
            if(segments.size() > maxSegments) {
                maxSegments = segments.size();
            }
            auto cappedpc =  ((float)(maxSegments - segments.size()) / 100);
            float pc = cappedpc; // / 100;

            std::cout << "\033[1F"
             << "\033[44;34m" << std::setw(std::min((int)(w-(w*pc)),(int)w-1)) << std::setfill(' ') << ' '
             << "\033[43;32m" << std::setw((int)(w*pc)) << std::setfill(' ') << ' '
             << std::setfill(' ')
             << "\033[0m"
             << "\033[27;34m" << " " << std::setw(6) << current.score
             << "\033[7;1;32m"  << " " << std::setw(6) << bestScore
             << "\033[27;35m"  << " " << std::setw(6) << maxSegments
             << "\033[22;27;0m" << " " << segments.size()
             << "\033[22;27;0m" << " " << (n++)
             << "\033[22;27;0m." << std::endl;

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
            std::cout << "\033[1F" << std::setw(6) << segments.size() << " " << std::setw(6) << currentSegment.moves.size() << " " << std::setw(6) << current.score << "\033[0m" << std::endl;
            #ifdef DEBUG
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
                #ifdef DEBUG
                    std::cout << "\033[7;1m;[---> EXIT FOUND (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                    std::cout << "\033[7;33;1m;[---> SCORE: " << current.score << "\033[0m." << std::endl;
                #endif
                exitCallback(segments,current.score);

                if(current.score < bestScore) {
                    bestPath = segments;
                    bestScore = current.score;
                }
                shouldPop = true;
            } else {
                // Anti loop - Anti Dead End - Anti return to start
                if(maze.IsStart(segments.back().end.position)) {
                    #ifdef DEBUG
                        std::cout << "\033[7;1m;[---> LOOP DETECTED (thru start) (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                    #endif
                    // Segment ends on the start tile. This is a loop
                    shouldPop = true;
                } else 
                if(maze.IsDeadEnd(segments.back().end.position,segments.back().end.direction)) {
                    #ifdef DEBUG
                        std::cout << "\033[31;1m;[---> DEAD END DETECTED (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                    #endif
                    // Segment ends on a dead end.
                    shouldPop = true;
                } else {
                    auto sff = std::find_if(segments.begin(), segments.end(), [segments](const MazeSegment & s) {
                        // Look for identical segment
                        return segments.back().origin.position == s.origin.position && segments.back().end.position == s.end.position;
                    });
                    if(sff != segments.end() && sff < segments.end() - 1) {
                        #ifdef DEBUG
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
                            #ifdef DEBUG
                                std::cout << "\033[7;1m;[---> LOOP DETECTED (thru backward segment) (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                            #endif
                            // Found an identical segment (inverted). This is a loop
                            shouldPop = true;
                        }
                    }
                }
                // Speed enhancement
                if(current.score > bestScore) {
                    #ifdef DEBUG
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
                    #ifdef DEBUG   
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

                    // Move the runner at the segment's end, update the currentSegment's origin accordingly
                    current = atEndPrevLast;
                    currentSegment.origin = current; // -------------------------------------------------------<<<
                    #ifdef DEBUG
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
                    #ifdef DEBUG
                        std::cout << "\033[7;1m;[---> SEGMENTS DEPLETED (" << current.position.first << ", " << current.position.second << ") \033[22;27m." << std::endl;
                    #endif
                    // All options/segments exhausted; leave
                    break;
                }
            }
        } else {
            // Just a regular tile; move ahead
            #ifdef DEBUG
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

    exitCallback(segments,current.score);

    return bestScore;
}

