/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Synth.h"

//==============================================================================
HarmideAudioProcessor::HarmideAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
      : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

HarmideAudioProcessor::~HarmideAudioProcessor()
{
}

//==============================================================================
const juce::String HarmideAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HarmideAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HarmideAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HarmideAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HarmideAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HarmideAudioProcessor::getNumPrograms()
{
    return 1;
}

int HarmideAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HarmideAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HarmideAudioProcessor::getProgramName (int index)
{
    return {};
}

void HarmideAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HarmideAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::String message;
    message << "Preparing plugin\n";
    message << " samplesPerBlock = " << samplesPerBlock << "\n";
    message << " sampleRate = " << sampleRate << "\n";
    juce::Logger::getCurrentLogger()->writeToLog(message);
    synth.setCurrentPlaybackSampleRate(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getNumOutputChannels();

    synth.addSound(new SynthSound());
    for (int i = 0; i < 8; i++) {
        synth.addVoice(new SynthVoice(spec));
    }
}

void HarmideAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HarmideAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HarmideAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalSamps = buffer.getNumSamples(); 

    synth.renderNextBlock(buffer, midiMessages, 0, totalSamps);
}

//==============================================================================
bool HarmideAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* HarmideAudioProcessor::createEditor()
{
    return new HarmideAudioProcessorEditor (*this);
}

//==============================================================================
void HarmideAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HarmideAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmideAudioProcessor();
}