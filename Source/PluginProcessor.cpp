/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FatesLimiterAudioProcessor::FatesLimiterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
	 ), apvts(*this, nullptr, juce::Identifier("FatesLimiter"), createParameterLayout())
#endif
{

    saturator.functionToUse = [](float x) {

        if (juce::Decibels::decibelsToGain(-0.2f) < x)
            return std::tanh(x);// 0.9f + (x - 0.9f) / (1.0f + (x - 0.9f) * 10.0f);
        else
            return x;
        };



    //ratioParameter = dynamic_cast<std::atomic<float>*>(apvts.getRawParameterValue("ratio"));
    gainParameter = apvts.getRawParameterValue("gain");
    releaseParameter = apvts.getRawParameterValue("release");
	dirtParameter = apvts.getRawParameterValue("dirt");

    //Fixed attack and ratio
	limiter.setAttack(attackTime);
    limiter.setRatio(ratio);
	limiter.setThreshold(threshold);

    
    
}

FatesLimiterAudioProcessor::~FatesLimiterAudioProcessor()
{
}

//==============================================================================
const juce::String FatesLimiterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FatesLimiterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FatesLimiterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FatesLimiterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FatesLimiterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FatesLimiterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FatesLimiterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FatesLimiterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FatesLimiterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FatesLimiterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FatesLimiterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;

	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();
	spec.sampleRate = sampleRate;

    saturator.prepare(spec);
	limiter.prepare(spec);
    secondary.prepare(spec);

}

void FatesLimiterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FatesLimiterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FatesLimiterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	float gain = gainParameter->load();
	//float ratio = ratioParameter->load();
	float release = releaseParameter->load();
	bool dirt = dirtParameter->load() > 0.5f;
    

	gainProcessor.setGainDecibels(gain);

	secondary.setThreshold(-0.2f);
	secondary.setRelease(500.0f);
	secondary.setRatio(1.5f);
	secondary.setAttack(0.1f);

	
	limiter.setRelease(release);



	juce::dsp::AudioBlock<float> block = juce::dsp::AudioBlock<float>(buffer);
	juce::dsp::ProcessContextReplacing<float> context = juce::dsp::ProcessContextReplacing<float>(block);

	

	gainProcessor.process(context);
	limiter.process(context);

    if(dirt)
        saturator.process(context);
    
    secondary.process(context);

}

//==============================================================================
bool FatesLimiterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FatesLimiterAudioProcessor::createEditor()
{
	//return new juce::GenericAudioProcessorEditor(*this);
    return new FatesLimiterAudioProcessorEditor (*this);
}

//==============================================================================
void FatesLimiterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FatesLimiterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FatesLimiterAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FatesLimiterAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "gain",1 }, "Gain", -20.0f, 20.0f, 1.0f));
    //layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "ratio" ,1}, "Ratio", 1.5, 6, 4));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "release",1 }, "Release", 1.0f, 1000.0f, 10.0f));
	layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "dirt",1 }, "Dirt", true));

    return layout;

}