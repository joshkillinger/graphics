#ifndef NOISE_H
#define NOISE_H


#include <cstdlib>
#include <cstdio>
#include <cmath>


class Noise
{
private:
    static const int B = 0x100;
    static const int BM = 0xff;

    static const int N = 0x1000;
    static const int NP = 12;   /* 2^N */
    static const int NM = 0xfff;

    char p[B + B + 2];
    float g3[B + B + 2][3];
    float g2[B + B + 2][2];
    float g1[B + B + 2];
    int start;

public:
    Noise();
    double Noise1(double arg);
    float Noise2(float vec[2]);
    float Noise3(float vec[3]);
};
#endif // NOISE_H
