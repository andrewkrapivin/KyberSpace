#include "Kyber.h"
#include "RandomBytes.h"
#include "Sample.h"
#include <cstddef>
#include <iostream>

namespace KyberSpace {

    KeyPair KeyGen() {
        std::string rho = GetTrueRandomBytes(RandomStringBytes);
        std::string sigma = GetTrueRandomBytes(RandomStringBytes);

        RingMatrix A = SampleMatrix(rho, 1, KK, KK, SampleUniformRejection)[0];
        std::vector<RingVector> se = SampleMatrix(sigma, 2, KK, 1, SampleBinomial);
        RingVector s = se[0];
        RingVector e = se[1];

        RingVector t = (A*s + e).compress(Dt);
        // RingVector t = (A*s + e);
        
        return KeyPair(PublicKey(t, rho), s);
    }

    EncryptedMessage Encrypt(PublicKey pk, Message m, std::string r) {
        RingVector t = pk.t.decompress(Dt);
        // RingVector t = pk.t;

        std::string rho = pk.rho;
        RingMatrix A = SampleMatrix(rho, 1, KK, KK, SampleUniformRejection)[0];

        std::vector<PolyRing> samples = SampleRing(r, KK+KK+1, SampleBinomial);
        std::vector<PolyRing> rvec(KK), e1vec(KK), e2vec(1);

        std::copy(samples.begin(), samples.begin()+KK, rvec.begin());
        std::copy(samples.begin()+KK, samples.begin()+2*KK, e1vec.begin());
        std::copy(samples.begin()+2*KK, samples.begin()+2*KK+1, e2vec.begin());

        RingVector rr(KK, 1, rvec), e1(KK, 1, e1vec);
        ScalarPolyRing e2(1, 1, e2vec);

        RingVector u = (A.T()*rr + e1).compress(Du);
        ScalarPolyRing v = (t.T()*rr+ e2 + m*QQd2).compress(Dv);

        // RingVector u = (A.T()*rr + e1);
        // ScalarPolyRing v = (t.T()*rr+ e2 + m*QQd2);
        
        return EncryptedMessage(u, v);
    }

    Message Decrypt(SecretKey sk, EncryptedMessage c) {
        RingVector u = c.u.decompress(Du);
        ScalarPolyRing v = c.v.decompress(Dv);
        // RingVector u = c.u;
        // ScalarPolyRing v = c.v;
        return (v-sk.T()*u).compress(1);
    }

}