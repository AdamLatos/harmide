#pragma once

#include <JuceHeader.h>

struct SynthSound : public juce::SynthesiserSound
{
    SynthSound() {}

    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }

};

//==============================================================================
struct SynthVoice : public juce::SynthesiserVoice
{
    SynthVoice::SynthVoice(const juce::dsp::ProcessSpec &spec);

private:
    virtual bool canPlaySound(juce::SynthesiserSound*) override;
    virtual void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    virtual void stopNote(float velocity, bool allowTailOff) override;
    virtual void pitchWheelMoved(int newPitchWheelValue) override;
    virtual void controllerMoved(int controllerNumber, int newControllerValue) override;
    virtual void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    juce::dsp::Oscillator<float> osc;
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParameters;
    bool sound_on;
};