/*
  ==============================================================================

    PadSynthVoice.cpp
    Created: 4 Apr 2021 5:15:27pm
    Author:  jonny

  ==============================================================================
*/

#include "PadSynthVoice.h"


PadSynthVoice::PadSynthVoice() : current_synthesiser_sound_(nullptr), 
                                 current_index_(0),
                                 overlap_index_(-1) 
    {
        this->overlap_buffer_.setSize(1, 48000 / 5); // overlap buffer 200 ms
        this->fade_out_parameters_.attack = .0f;
        this->fade_out_parameters_.decay = .0f;
        this->fade_out_parameters_.sustain = 1.f;
        this->fade_out_parameters_.release = .2f; //200 ms
        this->fade_out_.setParameters(this->fade_out_parameters_);
        srand((unsigned)time(0));
    }

void PadSynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* synthesiser_sound, int /*currentPitchWheelPosition*/)
{
    this->adsr_.noteOn();
    this->current_synthesiser_sound_ = dynamic_cast<PadSynthSound*>(synthesiser_sound);
    DBG("PadSynthVoice::startNote" << " / midiNoteNumber" << midiNoteNumber << " / freq" << this->current_synthesiser_sound_->getFundamentalFrequency());
    //this->current_index_ = 0; // random index within wavetable_size
    this->current_index_ = rand() % (int)(this->current_synthesiser_sound_->getWavetable().size());
}

void PadSynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    //DBG("PadSynthVoice::stopNote" << "allowTailOff " << (int)allowTailOff);
    //this->adsr_.noteOff();
    //DBG("PadSynthVoice::stopNote" << " / freq" << this->current_synthesiser_sound_->getFundamentalFrequency());
    //this->clearCurrentNote();
    //this->current_synthesiser_sound_ = nullptr;
    if (allowTailOff)
    {
        //if (tailOff == 0.0)
        //    tailOff = 1.0;
        this->adsr_.noteOff();
    }
    else
    {
        DBG("PadSynthVoice::stopNote note stealing" << "this->renderNextBlock(this->overlap_buffer_");
        this->renderNextBlock(this->overlap_buffer_, 0, this->overlap_buffer_.getNumSamples());
        this->fade_out_.applyEnvelopeToBuffer(this->overlap_buffer_, 0, this->overlap_buffer_.getNumSamples());
        //for (auto i = this->overlap_buffer_.getNumChannels(); --i >= 0;) {
        //    this->overlap_buffer_.applyGainRamp(0, this->overlap_buffer_.getNumSamples(), 1.f, 0.f);
        //}    
        this->overlap_index_ = 0;
        //this->fade_out_.noteOn();
        //this->fade_out_.noteOff(); // only release
        this->clearCurrentNote();
    }
}

void PadSynthVoice::setADSRSampleRate(double sampleRate) {
    this->adsr_.setSampleRate(sampleRate);
    this->fade_out_.setSampleRate(sampleRate);
}


void PadSynthVoice::getEnvelopeParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release)
{
    this->adsr_parameters_.attack = *attack;
    this->adsr_parameters_.decay = *decay;
    this->adsr_parameters_.sustain = *sustain;
    this->adsr_parameters_.release = *release;
}

void PadSynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    //DBG("PadSynthVoice::renderNextBlock"); // OK
    if (this->current_synthesiser_sound_ != nullptr) {
        //DBG("PadSynthVoice::renderNextBlock" << " / freq" << this->current_synthesiser_sound_->getFundamentalFrequency());
        //int& index = this->current_synthesiser_sound_->getCurrentIndex();
        this->adsr_.setParameters(this->adsr_parameters_);
        //const int wavetable_size = (int)this->current_synthesiser_sound_->getWavetable().size();
        const int wavetable_size1 = this->current_synthesiser_sound_->getAudioBuffer().getNumSamples();
        while (--numSamples >= 0) {
            //DBG("PadSynthVoice::renderNextBlock" << " / startSample" << startSample << " / this->current_index_" << this->current_index_);
            
            //check if (overlapIndex > -1)
            //    if true, add the buffer contents to your outputBufferand increase the overlapIndex
            //        if overlapIndex == bufferSize->overlapIndex = -1
            if (this->overlap_index_ > -1) {
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                    outputBuffer.addSample(i, startSample, this->overlap_buffer_.getSample(0, this->overlap_index_));
                }
                ++this->overlap_index_;
                if (this->overlap_index_ == this->overlap_buffer_.getNumSamples()) {
                    DBG("PadSynthVoice::renderNextBlock this->overlap_index_ == this->overlap_buffer_.getNumSamples()" << " note stealing buffer ended");
                    //this->clearCurrentNote();
                    this->overlap_index_ = -1;
                }
            }         
            //float currentSample = this->current_synthesiser_sound_->getWavetable().at(this->current_index_ % wavetable_size);
            //float currentSample1 = this->current_synthesiser_sound_->getAudioBuffer().getSample(0, this->current_index_ % wavetable_size1);
            float currentSample1 = this->current_synthesiser_sound_->getSample(this->current_index_ % wavetable_size1);
            this->current_index_ = (this->current_index_ + 1);
            for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                //outputBuffer.addSample(i, startSample, currentSample * this->adsr_.getNextSample());
                outputBuffer.addSample(i, startSample, currentSample1 * this->adsr_.getNextSample());
            }
            ++startSample;
        }
        if (!this->adsr_.isActive()) {
            //DBG("PadSynthVoice::renderNextBlock note released gracefully");
            this->clearCurrentNote();
            this->current_synthesiser_sound_ = nullptr;
        }
    }
    //if (angleDelta != 0.0)
    //{
    //    if (tailOff > 0.0) // [7]
    //    {
    //        while (--numSamples >= 0)
    //        {
    //            auto currentSample = (float) (std::sin (currentAngle) * level * tailOff);

    //            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
    //                outputBuffer.addSample (i, startSample, currentSample);

    //            currentAngle += angleDelta;
    //            ++startSample;

    //            tailOff *= 0.99; // [8]

    //            if (tailOff <= 0.005)
    //            {
    //                clearCurrentNote(); // [9]

    //                //angleDelta = 0.0;
    //                break;
    //            }
    //        }
    //    }
    //    else
    //    {
    //        while (--numSamples >= 0) // [6]
    //        {
    //            auto currentSample = (float) (std::sin (currentAngle) * level);

    //            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
    //                outputBuffer.addSample (i, startSample, currentSample);

    //            currentAngle += angleDelta;
    //            ++startSample;
    //        }
    //    }
    //}
}