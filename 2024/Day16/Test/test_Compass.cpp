#include "../Compass.h"

int main(int argc, char ** argv, char ** envp) {

    /*
    std::cerr << "argc: " << argc << std::endl;
    std::cerr << "argv[0]: " << argv[0] << std::endl;
    if(argc == 2) {
        std::cerr << "argv[1]: " << argv[1] << std::endl;
    }
    */

    if(argc == 1) {
        std::cerr << "Usage: test_Compass <param>" << std::endl;
        return 1;
    }

    if(argc == 2 && 0 == strcmp(argv[1], "cw")) {
        Compass c(Compass::EAST);
        if(c.Clockwise() != Compass::SOUTH) {
            std::cerr << "Clockwise failed (E>S)? " << c << std::endl;
            return 1;
        }
        if(c.Clockwise() != Compass::WEST) {
            std::cerr << "Clockwise failed (S>W)? " << c << std::endl;
            return 1;
        }
        if(c.Clockwise() != Compass::NORTH) {
            std::cerr << "Clockwise failed (W>N)? " << c << std::endl;
            return 1;
        }
        if(c.Clockwise() != Compass::EAST) {
            std::cerr << "Clockwise failed (N>E)? " << c << std::endl;
            return 1;
        }
        std::cerr << "Clockwise OK" << std::endl;
        return 0;
    }

    if(argc == 2 && 0 == strcmp(argv[1], "ccw")) {
        Compass c(Compass::EAST);
        if(c.CounterClockwise() != Compass::NORTH) {
            std::cerr << "CounterClockwise failed (E>N)? " << c << std::endl;
            return 1;
        }
        if(c.CounterClockwise() != Compass::WEST) {
            std::cerr << "CounterClockwise failed (N>W)? " << c << std::endl;
            return 1;
        }
        if(c.CounterClockwise() != Compass::SOUTH) {
            std::cerr << "CounterClockwise failed (W>S)? " << c << std::endl;
            return 1;
        }
        if(c.CounterClockwise() != Compass::EAST) {
            std::cerr << "CounterClockwise failed (S>E)? " << c << std::endl;
            return 1;
        }
        std::cerr << "CounterClockwise OK" << std::endl;
        return 0;
    }

    if(argc == 2 && 0 == strcmp(argv[1], "rev")) {
        Compass c(Compass::EAST);
        if(c.Reverse() != Compass::WEST) {
            std::cerr << "Reverse failed (E>W)? " << c << std::endl;
            return 1;
        }
        c = Compass::WEST;
        if(c.Reverse() != Compass::EAST) {
            std::cerr << "Reverse failed (W>E)? " << c << std::endl;
            return 1;
        }
        c = Compass::NORTH;
        if(c.Reverse() != Compass::SOUTH) {
            std::cerr << "Reverse failed (N>S)? " << c << std::endl;
            return 1;
        }
        c = Compass::SOUTH;
        if(c.Reverse() != Compass::NORTH) {
            std::cerr << "Reverse failed (S>N)? " << c << std::endl;
            return 1;
        }
        std::cerr << "Reverse OK" << std::endl;
        return 0;
    }

    if(argc == 2 && 0 == strcmp(argv[1], "eq")) {
        Compass c(Compass::EAST);
        Compass d(Compass::EAST);
        if(c != Compass::EAST) {
            std::cerr << "Equality failed (E==E)? " << c << std::endl;
            return 1;
        }
        if(!(c != Compass::WEST)) {
            std::cerr << "Equality failed (E!=W)? " << c << std::endl;
            return 1;
        }
        if(!(c != Compass::SOUTH)) {
            std::cerr << "Equality failed (E!=S)? " << c << std::endl;
            return 1;
        }
        if(!(c != Compass::NORTH)) {
            std::cerr << "Equality failed (E!=N)? " << c << std::endl;
            return 1;
        }
        if(!(c != (char)0)) {
            std::cerr << "Equality failed (E!=0x00)? " << c << std::endl;
            return 1;
        }
        if(c != 'E') {
            std::cerr << "Equality failed (E=='E')? " << c << std::endl;
            return 1;
        }
        if((char)c != Compass::EAST) {
            std::cerr << "Equality failed ((char)E=='E')? " << c << std::endl;
            return 1;
        }
        if(c != d) {
            std::cerr << "Equality failed (e(E)==d(E))? " << c << std::endl;
            return 1;
        }
        
        std::cerr << "Equality OK" << std::endl;
        return 0;
    }

    if(argc == 2 && 0 == strcmp(argv[1], "assign")) {
        Compass e(Compass::EAST);
        Compass g(e);
        Compass h = e;
        Compass i('E');
        Compass j = Compass::EAST;
        Compass k = 'E';
        if(e != g) {
            std::cerr << "Assignment failed (g(e) == g)? " << e << std::endl;
            return 1;
        }
        if(e != h) {
            std::cerr << "Assignment failed (h=e)? " << e << std::endl;
            return 1;
        }
        if(e != i) {
            std::cerr << "Assignment failed (i='E')? " << e << std::endl;
            return 1;
        }
        if(e != j) {
            std::cerr << "Assignment failed (j=Compass::EAST)? " << e << std::endl;
            return 1;
        }
        if(e != k) {
            std::cerr << "Assignment failed (k='E')? " << e << std::endl;
            return 1;
        }
        std::cerr << "Assignment OK" << std::endl;
        return 0;
    }

    if(argc == 2 && 0 == strcmp(argv[1], "ref")) {
        Compass e(Compass::EAST);
        if(e.Clockwise().Clockwise().Clockwise().Clockwise() != Compass::EAST) {
            std::cerr << "Reference failed (E->S->W->N->E)? " << e << std::endl;
            return 1;
        }
        if(e.CounterClockwise().CounterClockwise().CounterClockwise().CounterClockwise() != Compass::EAST) {
            std::cerr << "Reference failed (E->N->W->S->E)? " << e << std::endl;
            return 1;
        }
        if(e.Reverse().Reverse() != Compass::EAST) {
            std::cerr << "Reference failed (E->W->E)? " << e << std::endl;
            return 1;
        }
        std::cerr << "Reference OK" << std::endl;
        return 0;
    }
    
    std::cerr << "Usage: test_Compass <param (cw|ccw|rev|eq|assign|ref)>" << std::endl;
    return 1;
}