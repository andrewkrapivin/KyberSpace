#include "HashFunctionWrappers.h"
#include "Configuration.h"
#include <cstddef>
#include <cryptopp/filters.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
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

    std::string h256(std::string seed) {
        std::string digest;
        CryptoPP::SHA256 hash;
        hash.Update((const CryptoPP::byte*)seed.data(), seed.size());
        digest.resize(hash.DigestSize());
        hash.Final((CryptoPP::byte*)&digest[0]);
        
        return digest;
    }

    std::string h512(std::string seed) {
        std::string digest;
        CryptoPP::SHA512 hash;
        hash.Update((const CryptoPP::byte*)seed.data(), seed.size());
        digest.resize(hash.DigestSize());
        hash.Final((CryptoPP::byte*)&digest[0]);
        
        return digest;
    }

    std::string H(std::string seed) {
        return h256(seed);
    }

    KhatRPair G(std::string seed) {
        std::string combinedStr = h512(seed);

        std::string Kh;
        Kh.resize(RandomStringBytes);

        std::string r;
        r.resize(RandomStringBytes);

        std::copy(combinedStr.begin(), combinedStr.begin()+RandomStringBytes, Kh.begin());
        std::copy(combinedStr.begin()+RandomStringBytes, combinedStr.end(), r.begin());

        return KhatRPair(Kh, r);
    }

}