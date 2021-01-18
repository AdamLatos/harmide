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
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
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
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
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
    buffer.clear();
    /*
    auto* leftBuf = buffer.getWritePointer(0);
    auto* rightBuf = buffer.getWritePointer(1);
    auto totalSamps = buffer.getNumSamples(); 
    */
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