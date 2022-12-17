
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
// #include <cryptopp/filters.h>
// #include <cryptopp/cryptlib.h>
// #include <cryptopp/shake.h>
// #include <cryptopp/hex.h>
// #include <cryptopp/base64.h>

#include "RingOps.h"
#include "Matrix.h"
#include "Sample.h"
#include "Kyber.h"
#include "RandomBytes.h"
#include <utility>

// class KyberCPAEncryptor {
//     std::string rho, sigma;
// };

using namespace KyberSpace;
using namespace std;

int main() {

    // InnFE x(5);
    // InnFE y = x.inv();
    // std::cout << x << " " << y << " " << (x*y) << std::endl;

    // PolyRing xrr;
    // PolyRing yrr;
    // xrr.r[2] = 4;
    // xrr.r[3] = -1;
    // yrr.r[2] = 3;
    // yrr.r[3] = 2;
    // std::cout << "("<< xrr << ") * (" << yrr << ") = " << std::endl;

    // PolyRing zrr = xrr * yrr;
    // std::cout << zrr << std::endl;

    // vector<PolyRing> v1(1);
    // v1[0] = xrr;
    // vector<PolyRing> v2(1);
    // v2[0] = yrr;
    // RingMatrix m1(1, 1, v1);
    // RingMatrix m2(1, 1, v2);
    // std::cout << (m1*m2) << std::endl;

    // RingMatrix m = SampleMatrix("Do or do not. There is no try.", 1, 3, 3, SampleBinomial)[0];
    // std::cout << m << std::endl;

    // RingMatrix mu = SampleMatrix("Do or do not. There is no try.", 1, 3, 3, SampleUniformRejection)[0];
    // std::cout << "Mu:" << std::endl;
    // std::cout << mu << std::endl;

    // std::cout << "Mu compressed:" << std::endl;
    // RingMatrix mucomp = mu.compress(7);
    // std::cout << mucomp << std::endl;

    // std::cout << "Mu decompressed:" << std::endl;
    // RingMatrix mudecomp = mucomp.decompress(7);
    // std::cout << mudecomp << std::endl;

    Message m(1, 1);
    m.mat[0].r[0] = 1;
    m.mat[0].r[1] = 1;
    m.mat[0].r[2] = 0;
    m.mat[0].r[3] = 0;
    m.mat[0].r[4] = 0;
    m.mat[0].r[5] = 1;
    m.mat[0].r[6] = 1;
    m.mat[0].r[7] = 0;
    m.mat[0].r[8] = 1;
    m.mat[0].r[9] = 0;
    m.mat[0].r[10] = 1;
    m.mat[0].r[11] = 1;

    for(size_t i{0}; i < 40; i++) {
        std::cout << m.mat[0].r[i];
    }
    std::cout << std::endl;

    KeyPair g = KeyGen();
    EncryptedMessage c = Encrypt(g.pk, m, GetTrueRandomBytes(RandomStringBytes));
    Message dm = Decrypt(g.sk, c);

    for(size_t i{0}; i < 40; i++) {
        std::cout << dm.mat[0].r[i];
    }
    std::cout << std::endl;

}