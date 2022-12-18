#ifndef RING_MATRIX_H
#define RING_MATRIX_H

#include <vector>
#include <ostream>
#include "RingOps.h"

namespace KyberSpace {

    struct RingMatrix {
        size_t n, m;
        std::vector<PolyRing> mat;

        RingMatrix(size_t n, size_t m);
        RingMatrix(size_t n, size_t m, std::vector<PolyRing> mat);

        RingMatrix compress(std::size_t d);
        RingMatrix decompress(std::size_t d);

        RingMatrix T();

        RingMatrix operator + (RingMatrix const & e);
        RingMatrix operator - (RingMatrix const & e);
        RingMatrix operator * (InnFE const & s);
        RingMatrix operator * (RingMatrix const & e);

        bool operator == (const RingMatrix &)const;
    };

    std::ostream& operator<<(std::ostream &out, RingMatrix const& e);

    // //NOT a good print function. Should def be improved.
    // std::ostream& operator<<(std::ostream &out, RingMatrix const& e) {
    //     out << "[";
    //     for (size_t i=0; i < e.n; i++) {
    //         for(size_t j=0; j < e.m; j++) {
    //             out << e.mat[i*e.n + j];
    //             if(j < e.m - 1) {
    //                 out << " | ";
    //             }
    //         }
    //         if (i != e.n - 1)
    //             out << '\n';
    //     }
    //     out << ']';
    //     return out;
    // }

}

#endif