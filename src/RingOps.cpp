#include "RingOps.h"
#include <algorithm>
#include <cassert>

namespace KyberSpace {

    //Should I do with negative number support? It certainly makes a bit easier for some of the stuff like the binomial distribution.
    InnFE::InnFE(int32_t ff) {
        f = ff;
        if (ff < 0) {
            f = QQ-((0-ff)%QQ);
        }
        else if((uint32_t)ff >= QQ) {
            f = ff % QQ;
        }
    }

    InnFE InnFE::compress(std::size_t d) {
        assert(12 >= d && d >= 1);
        std::uint32_t ff = f;
        ff = ((ff*(1u << d) + QQd2) / QQ) & ((1u << d)-1);
        return InnFE(ff);
    }

    InnFE InnFE::decompress(std::size_t d) {
        assert(12 >= d && d >= 1);
        std::uint32_t ff = f;
        ff = (ff*QQ + (1u << (d-1))) / (1u << d); //Check that this is rounded up rather than down too tired to do that rn
        assert (((int32_t)ff) >= 0);
        return InnFE(ff);
    }

    InnFE powFE(InnFE x, size_t p) {
        if(p == 0) {
            return 1;
        }
        else if (p & 1) {
            return x + powFE(x * 2, p >> 1);
        }
        else {
            return powFE(x * 2, p >> 1);
        }
    }

    InnFE InnFE::inv() const {
        return powFE(InnFE(f), QQ-2);
    }

    InnFE InnFE::operator+(const InnFE& e) {
        InnFE res(e.f + f);
        if(res.f >= QQ) res.f -= QQ;
        return res;
    }

    InnFE InnFE::operator + (const uint16_t& e) {
        InnFE res(f + e);
        if(res.f >= QQ) res.f -= QQ;
        return res;
    }

    InnFE InnFE::operator-(const InnFE& e){
        InnFE res(f - e.f);
        if(res.f >= QQ) res.f += QQ; //Cause unsigned integer overflow
        return res;
    }

    InnFE InnFE::operator-(const uint16_t& e) {
        InnFE res(f - e);
        if(res.f >= QQ) res.f += QQ; //Cause unsigned integer overflow
        return res;
    }

    InnFE InnFE::operator * (const InnFE& e) {
        uint32_t res = ((uint32_t)f*e.f)%QQ;
        return res;
    }

    InnFE InnFE::operator * (const uint16_t& e) {
        uint32_t res = ((uint32_t)f*e)%QQ;
        return res;
    }

    InnFE InnFE::operator / (const InnFE& e) {
        return e.inv()*f;
    }

    InnFE InnFE::operator / (const uint16_t& e) {
        return InnFE(e).inv()*f;
    }

    std::ostream& operator<<(std::ostream &out, const InnFE &c)
    {
        out << c.f;
        return out;
    }







    PolyRing::PolyRing() {
        std::fill(r.begin(), r.end(), 0);
    }

    PolyRing::PolyRing(std::array<InnFE, NN> r) : r(r) {}


    PolyRing PolyRing::compress(std::size_t d) {
        PolyRing res;
        for(size_t i{0}; i < NN; i++) {
            res.r[i] = r[i].compress(d);
        }
        return res;
    }

    PolyRing PolyRing::decompress(std::size_t d) {
        PolyRing res;
        for(size_t i{0}; i < NN; i++) {
            res.r[i] = r[i].decompress(d);
        }
        return res;
    }

    PolyRing PolyRing::operator + (const PolyRing& e) {
        PolyRing res;
        for(size_t i=0; i < NN; i++) {
            res.r[i] = r[i] + e.r[i];
        }
        return res;
    }


    PolyRing PolyRing::operator - (const PolyRing& e) {
        PolyRing res;
        for(size_t i=0; i < NN; i++) {
            res.r[i] = r[i] - e.r[i];
        }
        return res;
    }

    PolyRing PolyRing::operator*(const InnFE& e) {
        PolyRing res;
        for(size_t i=0; i < NN; i++) {
            res.r[i] = r[i] * e;
        }
        return res;
    }


    PolyRing PolyRing::operator * (const PolyRing& e) {
        PolyRing res;
        for(size_t i=0; i < NN; i++) {
            for(size_t j=0; j < NN; j++) {
                InnFE term = r[i] * e.r[j];
                size_t p = i + j;
                if(p >= NN)
                    res.r[p-NN] = res.r[p-NN] - term;
                else
                    res.r[p] = res.r[p] + term;
            }
        }
        return res;
    }

    inline std::ostream& operator<<(std::ostream &out, const PolyRing &c)
    {
        for(ssize_t i=NN-1; i >= 0; i--) {
            out << c.r[i] << "x^" << i;
            if(i != 0) out << "+";
        }
        return out;
    }

}