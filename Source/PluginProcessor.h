/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class FatesLimiterAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FatesLimiterAudioProcessor();
    ~FatesLimiterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	juce::AudioProcessorValueTreeState apvts;

    std::array<juce::LinearSmoothedValue<float>, 2> getRmsValueBefore();
    std::array<juce::LinearSmoothedValue<float>, 2> getRmsValueAfter();

	juce::AudioVisualiserComponent waveform;

private:

    float attackTime = 0.1f, ratio = 20.0f, threshold = -0.3;

    std::array<juce::LinearSmoothedValue<float>, 2> rmsBefore{ 0,0 };
    std::array<juce::LinearSmoothedValue<float>, 2> rmsAfter{ 0,0 };

	juce::dsp::Gain<float> gainProcessor;

	juce::dsp::WaveShaper<float> saturator;

    juce::dsp::Compressor<float> limiter, secondary;

	std::atomic<float>* gainParameter = nullptr;
	//std::atomic<float>* ratioParameter = nullptr;
	std::atomic<float>* releaseParameter = nullptr;
	std::atomic<float>* dirtParameter = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FatesLimiterAudioProcessor)
};
