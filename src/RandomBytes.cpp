#include "RandomBytes.h"
#include <cassert>
#include <random>
#include <iostream>

namespace KyberSpace {

    //"True" random. Obviously random_device not perfect & its slow, but its at least the best you can get that comes standard on cpus.
    std::string GetTrueRandomBytes(std::size_t numBytes) {
        assert(numBytes % 4 == 0); //For now just make sure this holds
        std::string bytes(numBytes, 0);
        std::random_device rd;
        for(size_t i=0; i < numBytes/4; i++) {
            uint32_t* word = (uint32_t*) &bytes[i*4];
            *word = rd();
        }
        return bytes;
    }

}