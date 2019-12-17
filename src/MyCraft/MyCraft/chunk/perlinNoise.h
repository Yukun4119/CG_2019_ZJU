/*
* �������ο���
* https://mrl.nyu.edu/~perlin/noise/
* http://flafla2.github.io/2014/08/09/perlinnoise.html
*/

#pragma once

#include <random>

class PerlinNoise
{
private:
    unsigned char p[512];

    // ʹ��fade������ֵ
    double fade(double t);

    // ���Բ�ֵ
    double lerp(double t, double a, double b);

    // ����hashѡȡ�ݶ����������������������(x, y, z)�ĵ��
    double grad(unsigned char hash, double x, double y, double z);

public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    double noise(double x, double y);
    double noise(double x, double y, double z);
    double octaveNoise(double x, double y, int octaves, double persistence);

    // ����seed����p����
    void reseed(unsigned int seed);
};

