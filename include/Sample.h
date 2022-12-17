#ifndef SAMPLE_H
#define SAMPLE_H

#include <vector>
#include <functional>
#include "RingOps.h"
#include "Matrix.h"

namespace KyberSpace {

    std::vector<InnFE> SampleBinomial(std::string seed, std::size_t len);
    std::vector<InnFE> SampleUniformRejection(std::string seed, std::size_t len);

    std::vector<PolyRing> SampleRing(std::string seed, std::size_t len, std::function<std::vector<InnFE>(std::string, std::size_t)> sampler);
    std::vector<RingMatrix> SampleMatrix(std::string seed, std::size_t len, std::size_t n, std::size_t m, std::function<std::vector<InnFE>(std::string, std::size_t)> sampler);

}

#endif