#ifndef __DAY16_REINDEER_H__
#define __DAY16_REINDEER_H__

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "Compass.h"


struct Reindeer {
    std::pair<int,int> c = {0,0};   // Cursor Position
    Compass d = Compass::EAST;      // Direction (N,E,S,W)
    long score = 0;                 // Score
    
    // When storing in the stack, keep track of the number of choices available, and the number of choices already tested
    int choices = 0;
    int tested = 0;
};

#endif // __DAY16_REINDEER_H__