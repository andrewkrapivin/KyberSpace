#include "Sample.h"
#include "Sam.h"
#include "Configuration.h"
#include <iostream>
#include <cstdint>

namespace KyberSpace {

    std::vector<InnFE> SampleBinomial(std::string seed, std::size_t len) {
        std::vector<InnFE> res (len);
        std::string s = Sam(seed, len);

        constexpr std::size_t HammingWeight[16] = { //Really lazy way but this is prob faster than popcnt. Still very slow cause we are doing it byte by byte. Should really do a shuffle with avx2/512, but that will be later, if at all (cause the Sam is probably the bottleneck anyways)
            0, 1, 1, 2, //0-3
            1, 2, 2, 3, //4-7
            1, 2, 2, 3, //8-11
            2, 3, 3, 4, //12-15
        };

        for(size_t i=0; i < len; i++) {
            int8_t upperNibble = ((uint8_t)s[i]) >> 4;
            int8_t lowerNibble = ((uint8_t)s[i]) & 15;
            res[i] = HammingWeight[upperNibble] - HammingWeight[lowerNibble];
        }

        return res;
    }

    //Not doing anything bit perfect for now. So obviously inneficient
    bool MustReject(std::uint16_t num) {
        constexpr std::uint16_t minusOne = (std::uint16_t)-1ull;
        constexpr std::uint16_t minToReject = minusOne - (minusOne % QQ); //Should be the largest number which is zero mod Q so we reject above that
        if(num > minToReject) return true;
        return false;
    }

    std::vector<InnFE> SampleUniformRejection(std::string seed, std::size_t len) {
        std::string s = Sam(seed, len*sizeof(InnFE)*2); //*2 so we can perform rejection sampling. This is obviously imperfect since some small chance we will run out, but that chance is literally negligible

        std::vector<InnFE> res (len);
        for(size_t i=0,j=0; i < len; i++) {
            std::uint16_t num;
            do {
                num = (((std::uint16_t)s[j]) << 8) + s[j+1];
                j+=2;
                res[i] = InnFE(num);
            } while (MustReject(num));
        }

        return res;
    }

    std::vector<PolyRing> SampleRing(std::string seed, std::size_t len, std::function<std::vector<InnFE>(std::string, std::size_t)> sampler) {
        std::vector<InnFE> coeffs = sampler(seed, len*NN);
        std::vector<PolyRing> res;
        res.resize(len);
        for(size_t i=0; i < len; i++) {
            std::array<InnFE, NN> elem;
            for(size_t j=0; j < NN; j++) {
                elem[j] = coeffs[i*NN + j];
            }
            res[i] = PolyRing(elem);
        }
        return res;
    }

    std::vector<RingMatrix> SampleMatrix(std::string seed, std::size_t len, std::size_t n, std::size_t m, std::function<std::vector<InnFE>(std::string, std::size_t)> sampler) {
        std::vector<PolyRing> matRings = SampleRing(seed, n*m*len, sampler);
        std::vector<RingMatrix> mats;
        mats.reserve(len);
        for(size_t i=0; i < len; i++) {
            std::vector<PolyRing> matvec(n*m);
            std::copy(matRings.begin()+(i*n*m), matRings.begin()+((i+1)*n*m), matvec.begin());
            mats.push_back(RingMatrix(n, m, matvec));
        }
        return mats;
    }

}