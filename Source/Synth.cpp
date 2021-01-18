#include "Synth.h"

SynthVoice::SynthVoice(const juce::dsp::ProcessSpec &spec) 
{
	osc.initialise(std::sinf, 2048);
	osc.setFrequency(440);
	osc.prepare(spec);
	adsr.setSampleRate(spec.sampleRate);
	adsr.setParameters({ 0.2, 0.2, 1.0, 0.2 });
	sound_on = false;
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound*)
{
	return true;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	auto freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	osc.setFrequency(freq);
	adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	if (allowTailOff) {
		adsr.noteOff();
	}
	else {
		clearCurrentNote();
	}
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (adsr.isActive()) {
		while (--numSamples >= 0) {
			auto currentSample = osc.processSample(0.0);
			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, currentSample * adsr.getNextSample() * 0.5);
			++startSample;
		}
	}
	else {
		clearCurrentNote();
	}
}
