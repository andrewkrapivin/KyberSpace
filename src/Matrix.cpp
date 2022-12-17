#include "Matrix.h"
#include <cassert>
#include <iostream>

using namespace KyberSpace;

RingMatrix::RingMatrix(size_t n, size_t m) : n(n), m(m), mat(n*m) {
}

RingMatrix::RingMatrix(size_t n, size_t m, std::vector<PolyRing> mat) : n(n), m(m), mat(mat) {
    assert (mat.size() == n*m);
}

RingMatrix RingMatrix::compress(std::size_t d) {
    RingMatrix res(n, m, mat);
    for(size_t i{0}; i < n*m; i++) {
        res.mat[i] = res.mat[i].compress(d);
    }
    return res;
}

RingMatrix RingMatrix::decompress(std::size_t d) {
    RingMatrix res(n, m, mat);
    for(size_t i{0}; i < n*m; i++) {
        res.mat[i] = res.mat[i].decompress(d);
    }
    return res;
}


//Check these functions (& multiplication) to see if it works for non square matrices later maybe.
RingMatrix RingMatrix::T() {
    std::vector<PolyRing> tmat(m*n);
    for(size_t i{0}; i < n; i++) {
        for(size_t j{0}; j < m; j++) {
            tmat[j*n+i] = mat[i*m+j];
        }
    }
    return RingMatrix(m, n, tmat);
}

RingMatrix RingMatrix::operator+(RingMatrix const & e) {
    assert(n == e.n && m == e.m);
    RingMatrix res (n, m);
    for(size_t i=0; i < n*m; i++) {
        res.mat[i] = mat[i] + e.mat[i];
    }
    return res;
}


RingMatrix RingMatrix::operator-(RingMatrix const & e) {
    assert(n == e.n && m == e.m);
    RingMatrix res (n, m);
    for(size_t i=0; i < n*m; i++) {
        res.mat[i] = mat[i] - e.mat[i];
    }
    return res;
}

RingMatrix RingMatrix::operator * (InnFE const & s) {
    RingMatrix res (n, m);
    for(size_t i=0; i < n*m; i++) {
        res.mat[i] = mat[i] * s;
    }
    return res;
}


//Maybe todo? Optimize this to be better memory optimized. However, shouldn't matter cause k(=m=n) is so small
RingMatrix RingMatrix::operator*(RingMatrix const & e) {
    assert(m == e.n);
    RingMatrix res (n, e.m);
    for(size_t k = 0; k < m; k++) {
        for(size_t i = 0; i < n; i++) {
            for(size_t j = 0; j < e.m; j++) {
                res.mat[i*e.m + j] = res.mat[i*e.m + j] + mat[i*m + k] * e.mat[k*e.m + j];
            }
        }
    }
    return res;
}