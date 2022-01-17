/*
  ==============================================================================

    MyPadSynth.h
    Created: 22 Mar 2021 9:12:13pm
    Author:  jonny

  ==============================================================================
*/

#pragma once
#include "fftwrapper/FFTwrapper.h"
#include <vector>

class MyPadSynth
{
public:
    MyPadSynth( int N,
                int samplerate,
                int number_harmonics,
                float fundamental_frequency,
                float base_frequency);
    /*  PADsynth:
        N                - is the samplesize (eg: 262144)
        samplerate 	 - samplerate (eg. 44100)
        number_harmonics - the number of harmonics that are computed */
        //	PADsynth(int N_, int samplerate_, int number_harmonics_);

    ~MyPadSynth();

    struct PadSynthParameters {
        PadSynthParameters() {};
        PadSynthParameters(int N,
                            int samplerate,
                            int number_harmonics,
                            float fundamental_frequency,
                            float base_frequency) {
            this->N = N;
            this->samplerate = samplerate;
            this->number_harmonics = number_harmonics;
            this->fundamental_frequency = fundamental_frequency;
            this->base_frequency = base_frequency;
        };

        int N;
        int samplerate;
        int number_harmonics;
        float fundamental_frequency;
        float base_frequency;
    };
    /* set the amplitude of the n'th harmonic */
    void setharmonic(int n, float value);

    /* get the amplitude of the n'th harmonic */
    float getharmonic(int n);

    /*  synth() generates the wavetable
        f		- the fundamental frequency (eg. 440 Hz)
        bw		- bandwidth in cents of the fundamental frequency (eg. 25 cents)
        bwscale	- how the bandwidth increase on the higher harmonics (recomanded value: 1.0)
        *smp	- a pointer to allocated memory that can hold N samples */
    void synth(float f1, float bandwidth, float bandwidth_scale, std::vector<float>& samples);

    const std::vector<float>& getWavetable() const;
    const std::vector<float> getHarmonics() { return this->A_; }
    const int getSampleRate() { return this->samplerate_; }

    int& getCurrentIndex() { return current_index_; };
    const float getFundamentalFrequency() { return this->fundamental_frequency_; };

    void set_parameters(int N,
                        int samplerate,
                        int number_harmonics,
                        float fundamental_frequency,
                        float base_frequency);
    void set_parameters(PadSynthParameters params);

    PadSynthParameters get_parameters();

    std::vector<float> prototype_signal_;
protected:
    int wavetable_size_;			//Size of the sample

    /* IFFT() - inverse fast fourier transform
       YOU MUST IMPLEMENT THIS METHOD!
       *freq_real and *freq_imaginary represents the real and the imaginary part of the spectrum,
       The result should be in *smp array.
       The size of the *samples array is N and the size of the freq_real and freq_imaginary is N/2 */
    virtual void IFFT(std::vector<float>& freq_real, std::vector<float>& freq_imaginary, std::vector<float>& samples);

    /* relF():
        This method returns the N'th overtone's position relative
        to the fundamental frequency.
        By default it returns N.
        You may override it to make metallic sounds or other
        instruments where the overtones are not harmonic.  */
    virtual float relF(int N);

    /* profile():
        This is the profile of one harmonic
        In this case is a Gaussian distribution (e^(-x^2))
            The amplitude is divided by the bandwidth to ensure that the harmonic
        keeps the same amplitude regardless of the bandwidth */
    virtual float profile(float fi, float bwi);

    /* RND() - a random number generator that
        returns values between 0 and 1
    */
    virtual float RND();

    void normalize(std::vector<float>& samples);
    std::vector<float> fourier_series_coefficients(int numbers_of_coefficients, int sample_rate, const std::vector<float>& samples);

    enum class timbre_type { formants, square, sawtooth };
    void build_harmonics(MyPadSynth::timbre_type harmonics_profile);

    void resample_harmonics(float resampling_factor);


private:
    int samplerate_;
    int number_harmonics_;
    float fundamental_frequency_;
    float base_frequency_;

    std::vector<float> A_;		//Amplitude of the harmonics
    std::vector<float> freq_amp_;	//Amplitude spectrum

    std::vector<float> wavetable_;

    int current_index_;
};