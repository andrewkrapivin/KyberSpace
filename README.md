# KyberSpace

This is a really simple implementation of (an outdated version of) Kyber IND-CPA secure encryption and IND-CCA secure KEM (although I would not trust it for anything, obviously). 

To build (for now):
g++ main.cpp src/* -I include -lcryptopp -std=c++17 -g -Wall -march=native

Requires the Crypto++ library. On ubuntu:
apt install libcrypto++-dev

Future plans are:
1. Update this to reflect the newest Kyber version. Currently I used the older version that does not employ rounding and is based on the following paper: https://eprint.iacr.org/2017/634.pdf
2. Output binary versions of the public key, encrypted messages, and so on, so they can be stored in files. Additionally, this binary version should be storing the compressed versions to be as space efficient as the real implementation.
3. Implement a montgomery reduction for modular multiplication
4. Implement NTT for faster polynomial multiplication
5. Accelerate (some of?) the operations with AVX512?
6. Figure out how SHA & SHAKE work to implement that in tune with Kyber? They are supposedly SIMDable (although maybe with AVX512 that changed?), but can use SIMD to run several hashes at once, so that might be interesting.