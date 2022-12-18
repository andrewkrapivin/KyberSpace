#include "Kyber.h"
#include "RandomBytes.h"
#include "Sample.h"
#include "HashFunctionWrappers.h"
#include "HelperFuncs.h"
#include <cstddef>
#include <iostream>
#include <sstream>

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

    //Figure out a way to do this better later. Def do it in the class, but some special way.
    Message mStrToMessage(std::string mstr) {
        std::vector<PolyRing> p(1);
        for(size_t i{0}; i < NN; i++) {
            p[0].r[i] = (mstr[i/8] & (1u << (i % 8))) != 0;
        }
        return Message(1, 1, p);
    }

    EncryptedMessage Encrypt(PublicKey pk, std::string mstr, std::string r) {
        return Encrypt(pk, mStrToMessage(mstr), r);
    }

    Message Decrypt(SecretKey sk, EncryptedMessage c) {
        RingVector u = c.u.decompress(Du);
        ScalarPolyRing v = c.v.decompress(Dv);
        // RingVector u = c.u;
        // ScalarPolyRing v = c.v;
        return (v-sk.T()*u).compress(1);
    }





    KEMKeyPair KEMKeyGen() {
        KeyPair kp = KeyGen();
        std::string z = GetTrueRandomBytes(RandomStringBytes);
        return KEMKeyPair(kp.pk, KEMSecretKey(kp.sk, z));
    }

    std::string mPolyRingToStr(Message m) {
        std::string s;
        s.resize(RandomStringBytes);
        for(size_t i{0}; i < NN; i++) {
            s[i/8] |= (m.mat[0].r[i].f << (i % 8));
        }
        return s;
    }

    //This is *REALLY* inneficient, as it prints a bunch of extra superfluous stuff aside from the values. But for now it works. Will improve later once I get compression working, cause that will need to be done anyways.
    KhatRPair getKhr(PublicKey pk, std::string mstr) {
        std::stringstream pkToStr;
        pkToStr << pk.t << pk.rho;
        // std::cout << pkToStr.str() << std::endl;
        KhatRPair Khr = G(H(pkToStr.str()) + mstr);
        return Khr;
    }

    //Again, really inneficient:
    std::string getK(std::string Kh, EncryptedMessage c) {
        std::stringstream cToStr;
        cToStr << c.u << c.v;
        std::string K = H(Kh + H(cToStr.str()));
        return K;
    }

    KEMMessage Encapsulate(PublicKey pk) {
        std::string mstr = GetTrueRandomBytes(RandomStringBytes);
        // std::cout << "mstr: ";
        // printKStringAsHex(mstr);

        KhatRPair Khr = getKhr(pk, mstr);

        // std::cout << "r: ";
        // printKStringAsHex(Khr.r);

        EncryptedMessage c = Encrypt(pk, mstr, Khr.r);
        std::string K = getK(Khr.Kh, c);
        
        return KEMMessage(c, K);
    }

    std::string Decapsulate(KEMKeyPair kemkp, EncryptedMessage c) {
        Message mp = Decrypt(kemkp.sk.s, c);
        KhatRPair Khrp = getKhr(kemkp.pk, mPolyRingToStr(mp));
        // std::cout << "mstr: ";
        // printKStringAsHex(mPolyRingToStr(mp));
        // std::cout << "r: ";
        // printKStringAsHex(Khrp.r);
        EncryptedMessage cp = Encrypt(kemkp.pk, mp, Khrp.r);
        if(c.u == cp.u && c.v == cp.v) {
            return getK(Khrp.Kh, cp);
        }
        else{
            return getK(kemkp.sk.z, cp);
        }
    }

}