#include "HelperFuncs.h"
#include <iostream>

namespace KyberSpace {

    void printKStringAsHex(std::string s) {
        char HexTable[16] = {
            '0', '1', '2', '3',
            '4', '5', '6', '7',
            '8', '9', 'A', 'B',
            'C', 'D', 'E', 'F'
        };
        for(unsigned char c: s) {
            unsigned char top = HexTable[c >> 4];
            unsigned char bot = HexTable[c & 15];
            std::cout << top << bot << " ";
        }
        std::cout << std::endl;
    }

}