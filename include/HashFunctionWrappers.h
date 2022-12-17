#ifndef SAM_H
#define SAM_H

#include <string>
#include <utility>

namespace KyberSpace {

    std::string Sam(std::string seed, std::size_t length);

    std::string h256(std::string seed);
    std::string h512(std::string seed);

    //Maybe this is going too far? Idk
    struct KhatRPair {
        std::string Kh, r;

        KhatRPair(std::string Kh, std::string r): Kh(Kh), r(r) {}
    };


    std::string H(std::string seed);
    KhatRPair G(std::string seed);

}

#endif