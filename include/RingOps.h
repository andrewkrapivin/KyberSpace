#ifndef RING_OPS_H
#define RING_OPS_H

#include "Configuration.h"
#include <cstdint>
#include <array>
#include <cstddef>
#include <ostream>

namespace KyberSpace {
    //Stores a field element of Z_QQ
    struct InnFE {
        uint16_t f;

        InnFE(int32_t ff = 0);

        InnFE compress(std::size_t d);
        InnFE decompress(std::size_t d);

        InnFE inv() const;

        InnFE operator + (const InnFE& e);
        InnFE operator + (const uint16_t& e);
        InnFE operator - (const InnFE& e);
        InnFE operator - (const uint16_t& e);
        InnFE operator * (const InnFE& e);
        InnFE operator * (const uint16_t& e);
        InnFE operator / (const InnFE& e);
        InnFE operator / (const uint16_t& e);

        bool operator == (const InnFE &)const;
        bool operator == (const uint16_t &)const;
    };

    InnFE powFE(InnFE x, size_t p);

    std::ostream& operator<<(std::ostream &out, const InnFE &c);

    // inline std::ostream& operator<<(std::ostream &out, const InnFE &c)
    // {
    //     out << c.f;
    //     return out;
    // }

    //Encodes Z_q[X] / (X^NN + 1)
    struct alignas(64) PolyRing {
        std::array<InnFE, NN> r;

        PolyRing();
        PolyRing(std::array<InnFE, NN> r);

        PolyRing compress(std::size_t d); //For now this compression is kinda fake. Not actually bitwise compression to compress the results, but well whatever. We'll do that later, probably even with a diff type, like compressedPolyRing
        PolyRing decompress(std::size_t d);

        PolyRing operator + (const PolyRing& e);
        PolyRing operator - (const PolyRing& e);
        PolyRing operator * (const PolyRing& e);
        PolyRing operator * (const InnFE& e);
        
        bool operator == (const PolyRing &)const;
    };

    std::ostream& operator<<(std::ostream &out, const PolyRing &c);

    // inline std::ostream& operator<<(std::ostream &out, const PolyRing &c)
    // {
    //     for(ssize_t i=NN-1; i >= 0; i--) {
    //         out << c.r[i] << "x^" << i;
    //         if(i != 0) out << "+";
    //     }
    //     return out;
    // }
}

#endif