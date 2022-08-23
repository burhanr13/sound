#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include <math.h>

typedef float Waveform(float t, float freq);

float sine(float t, float freq)
{
    return (1 + sinf(2 * M_PI * t * freq)) / 2;
}

float square(float t, float freq)
{
    return (int)(2 * t * freq) % 2;
}

float triangle(float t, float freq)
{
    return (2 * ((int)(2 * t * freq) % 2) - 1) * (1 - 2 * fmodf(t * freq, 1));
}

float saw(float t, float freq)
{
    return fmodf(t * freq, 1);
}

float noise(float t, float freq)
{
    if (freq)
    {
        return (float) rand() / RAND_MAX;
    }
    return 0;
}


#endif