#include <iostream>
#include <cmath>
#include <cstdlib>
#include "processing.hpp"

using namespace std;

int normalize(double *buffer, int length)
{
    double highestAmp = 0;
    // cout << "Normalizing audio..." << endl;
    for (int ndx = 0; ndx < length; ndx++)
    {
        if (fabs(buffer[ndx]) > highestAmp)
        {
            highestAmp = fabs(buffer[ndx]);
        }
    }
    // The normFac is 1.0/highestAmp
    double normFac = 1.0 / highestAmp;
    for (int ndx = 0; ndx < length; ndx++)
    {
        buffer[ndx] *= normFac;
    }
    // cout << "Audio normalized." << endl;

    return 0;
}

int gain(double *buffer, int length, double gainFac)
{
    for (int ndx = 0; ndx < length; ndx++)
    {
        buffer[ndx] *= gainFac;
    }
    return 0;
}

int clip(double *buffer, int length, double height)
{
    for (int ndx = 0; ndx < length; ndx++)
    {
        if (fabs(buffer[ndx]) > height)
        {
            if (buffer[ndx] < 0.0)
            {
                buffer[ndx] = -1.0 * height;
            }
            else
            {
                buffer[ndx] = height;
            }
        }
    }
    return 0;
}

int reverse(double *inBuf, double *outBuf, int length)
{
    cout << "Reversing audio..." << endl;

    // Copy from one buffer to the other and reverse
    for (int ndx = 0; ndx < length; ndx++)
    {
        outBuf[length - 1 - ndx] = inBuf[ndx];
    }
    cout << "Audio reversed." << endl;

    return 0;
}

// This function will distort the output somewhat
int rectify(double *buffer, int length)
{
    cout << "Rectifying audio..." << endl;
    // Force all negative sample values to be positive
    for (int ndx = 0; ndx < length; ndx++)
    {
        if (buffer[ndx] < 0)
        {
            buffer[ndx] *= 1.0;
        }
    }
    cout << "Audio rectified." << endl;
    return 0;
}

int fadeIn(double *buffer, int SR, int nChnls, double fadeTime)
{
    cout << "Fading in audio..." << endl;
    int nSamps = SR * nChnls * fadeTime;

    for (int ndx = 0; ndx < nSamps; ndx++)
    {
        buffer[ndx] *= double(ndx) / nSamps;
    }
    cout << "Audio faded in." << endl;

    return nSamps;
}

int fadeOut(double *buffer, int length, int SR, int nChnls, double fadeTime)
{
    cout << "Fading out audio..." << endl;

    int nSamps = SR * nChnls * fadeTime;

    for (int ndx = 0; ndx < nSamps; ndx++)
    {
        buffer[length - 1 - ndx] *= double(ndx) / nSamps;
    }
    cout << "Audio faded out." << endl;

    return nSamps;
}

int stereoToMono(double *stereoBuf, int length, double *monoBuf) {
    cout << "Converting Stereo audio to Mono" << endl;

    // Mix a 2-channel signal down to one
    for(int ndx=0; ndx<length/2; ndx++) {
        monoBuf[ndx] = stereoBuf[ndx*2] * 0.5 + stereoBuf[ndx*2+1] * 0.5;
    }
    cout << "Audio is now mono." << endl;

    return 0;
}

int panMod(double *inBuf, int length, double *outBuf, int SR, double vco){
    cout << "Applying pan modulation.." << endl;
    // This only works with a mono signal
    // Make sure that outBuf is twice the length of inBuf
    // Cause the mono signal to periodically alternate between the two stereo channels
    // Smooth transition
    // This requires a vco or lfo
    double amp = 1.0, freq, phase = 0.0;
    double tr;

    freq = vco;
    for (int ndx = 0; ndx < length; ndx++)
    {
        freq = vco * ndx / length;
        tr = 0.5 + (amp * sin(freq * ndx * 2.0 * PI / SR + phase));
        // The even numbered samples are in one channel
        // One channels amplitudes go from 0-1
        outBuf[ndx * 2] = inBuf[ndx] * tr;
        // The odd numbered samples are in the other channel
        // The other channels amplitudes go from 1-0
        outBuf[ndx * 2 - 1] = inBuf[ndx] * (1.0 - tr);
    }
    cout << "Pan modulation applied." << endl;

    return 0;
}

int ampMod(double *buffer, int length, int SR, double vco)
{
    cout << "Applying amplitude modulation.." << endl;

    double amp = 1.0, freq, phase = 0.0;
    double tr;
    for (int ndx = 0; ndx < length; ndx++)
    {
        freq = vco * ndx / length;
        // freq = vco;
        tr = amp * sin(freq * ndx * 2.0 * PI / SR + phase);
        buffer[ndx] = buffer[ndx] * tr;
    }
    cout << "Amplitude modulation applied." << endl;

    return 0;
}

int pitchChange(double *buffer, int length, double originalPitch, double newPitch) {
    cout << "Changing pitch.." << endl;
    double resamplingRatio = newPitch / originalPitch;
    double *tempBuf;
    int counter = 0;
    const int tempBufLength = ((int) length/resamplingRatio+1);

    tempBuf = new double[tempBufLength];

    for(double ndx=0.0;ndx<length;ndx+=resamplingRatio) {
        tempBuf[counter++] = buffer[(int) ndx];
    }
    for(int ndx=0;ndx<length;ndx++) {
        if(ndx<counter) {
            buffer[ndx] = tempBuf[ndx];
        }
        else {
            buffer[ndx] = 0.0;
        }
    }
    cout << "Pitch changed." << endl;
    return 0;
}

int extortion(double *buffer, int length, double height) {
    cout << "Extorting audio.." << endl;
    // Apply extreme gain
    gain(buffer, length, 30.0);
    // Clip if back to 0-1 range
    clip(buffer, length, height);
    cout << "Audio extorted." << endl;
    return 0;
}

int waveShape (double *buffer, int length, double distortionFacPos, double distortionFacNeg) {
    cout << "Waveshaping audio.." << endl;
    // Distortion factors (range 0.2 - 5)
    for(int ndx=0;ndx<length; ndx++) {
        if(buffer[ndx]>=0.0) {
            // Positive sample values only
            buffer[ndx] = tanh(distortionFacPos*buffer[ndx]/tanh(distortionFacPos));
        }
        else {
            // Negative sample values only
            buffer[ndx] = tanh(distortionFacNeg*buffer[ndx]/tanh(distortionFacNeg));            
            }
        }
    cout << "Audio is now waveshaped." << endl;

    return 0;
}

// Invert signal polarity (positive to negative, negative to positive)
int invert(double *buffer, int length)
{
    cout << "Inverting audio polarity.." << endl;
    for (int ndx = 0; ndx < length; ndx++)
    {
        buffer[ndx] *= -1.0;
    }
    cout << "Audio is now inverted." << endl;
    return 0;
}
