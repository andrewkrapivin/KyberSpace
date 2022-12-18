#include <string>
#include <immintrin.h>

namespace KyberSpace {

    //Only actually returns true random bytes if the hardware supports it (and even then, just "true" random), cause uses std::random_device. Will replace this with the random number generator in Crypto++ later for a *bit* more assurance.
    std::string GetTrueRandomBytes(std::size_t numBytes);

}