#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <cstddef>

namespace KyberSpace {
    constexpr std::size_t NN = 256;
    constexpr std::size_t QQ = 7681;
    constexpr std::size_t QQd2 = (QQ+1)/2;

    constexpr std::size_t BitsPerByte = 8;
    constexpr std::size_t RandomStringSize = 256;
    constexpr std::size_t RandomStringBytes = RandomStringSize/BitsPerByte;

    //These need to be configurable (ex template) later.
    constexpr std::size_t KK = 3;
    constexpr std::size_t Dt = 11, Dv = 3, Du = 11;
    constexpr std::size_t Nu = 4; //This is just formality (at least for now). Nu is hardcoded in the binomial generator as 4 for now.
}

#endif