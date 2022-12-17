#include "Sam.h"
#include <cstddef>
#include <cryptopp/filters.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/shake.h>

namespace KyberSpace {

    std::string Sam(std::string seed, std::size_t length) {
        std::string digest;
        CryptoPP::SHAKE256 hash(length);
        hash.Update((const CryptoPP::byte*)seed.data(), seed.size());
        digest.resize(length);
        hash.Final((CryptoPP::byte*)&digest[0]);
        
        return digest;
    }

}