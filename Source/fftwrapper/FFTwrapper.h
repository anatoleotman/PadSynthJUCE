#ifndef FFT_WRAPPER_H
#define FFT_WRAPPER_H

struct FFTFREQS {
    float* s, * c;//sine and cosine components
};

#include <fftw3.h>

void newFFTFREQS(FFTFREQS* f, int size);
void deleteFFTFREQS(FFTFREQS* f);

class FFTwrapper {
public:
    FFTwrapper(int fftsize_);
    ~FFTwrapper();
    void smps2freqs(float* smps, FFTFREQS freqs);
    void freqs2smps(FFTFREQS freqs, float* smps);
private:
    int fftsize;
    double* tmpfftdata1, * tmpfftdata2;
    fftw_plan planfftw, planfftw_inv;
};
#endif
