#include "../Map.h"

int main(int argc, char ** argv, char ** envp) {
    if(argc == 1) {
        std::cerr << "Usage: test_charmap <param>" << std::endl;
        return 1;
    }

    if(argc == 2 && 0 == strcmp(argv[1], "xxx")) {
        if(false) {
            std::cerr << argv[1] << " Failed: True is False" << std::endl;
            return 1;
        }
        std::cerr << argv[1] << " OK" << std::endl;
        return 0;
    }
   
    std::cerr << "Usage: test_Compass <param (xxx|yyy|zzz)>" << std::endl;
    return 1;
}