#ifndef KYBER_H
#define KYBER_H

#include <string>
#include <utility>
#include "Matrix.h"

namespace KyberSpace {
    //Todo: since this current system does a bad job of differentiating between matrix & vector, maybe do some templating?
    //So the templating is kinda just to force stuff to use RingVector & RingMatrix

    //Also, there's a lot of adhoc naming. Maybe its a little too much?

    //For now this terrible typedef. Really terrible lol. BUt after all a vector is a matrix, just an n x 1 matrix, so whatever.
    //Thus it really serves just as a bit of a like "oh let's remind ourselves of this"
    typedef RingMatrix RingVector;
    //Also using a matrix for scalars to simplify multiplication and operations and stuff
    typedef RingMatrix ScalarPolyRing;
    typedef ScalarPolyRing Message;

    //For now we use a polyring to store a message of 256 bits. Obviously overkill but whatever
    //When I will implement compression, probably want the PolyRing to be like more legit. Maybe have a couple different types for this? But for now whatever
    // typedef std::pair<RingVector, std::string> PublicKey;
    struct PublicKey {
        RingVector t;
        std::string rho;

        PublicKey(RingVector t, std::string rho): t(t), rho(rho){}
    };

    typedef RingVector SecretKey;
    // typedef std::pair<RingVector, ScalarPolyRing> EncryptedMessage;

    struct EncryptedMessage {
        RingVector u;
        ScalarPolyRing v;

        EncryptedMessage(RingVector u, ScalarPolyRing v): u(u), v(v) {}
    };

    struct KeyPair {
        PublicKey pk;
        SecretKey sk;

        KeyPair(PublicKey pk, SecretKey sk): pk(pk), sk(sk) {}
    };

    KeyPair KeyGen();
    EncryptedMessage Encrypt(PublicKey pk, Message m, std::string r);
    EncryptedMessage Encrypt(PublicKey pk, std::string mstr, std::string r);
    Message Decrypt(SecretKey sk, EncryptedMessage c);

    struct KEMMessage {
        EncryptedMessage c;
        std::string K;

        KEMMessage(EncryptedMessage c, std::string K): c(c), K(K){}
    };

    struct KEMSecretKey {
        SecretKey s;
        std::string z;

        KEMSecretKey(SecretKey s, std::string z) : s(s), z(z) {}
    };

    struct KEMKeyPair {
        PublicKey pk;
        KEMSecretKey sk;

        KEMKeyPair(PublicKey pk, KEMSecretKey sk) : pk(pk), sk(sk) {}
    };

    
    KEMKeyPair KEMKeyGen();
    KEMMessage Encapsulate(PublicKey pk);
    std::string Decapsulate(KEMKeyPair kemkp, EncryptedMessage c);

}

#endif