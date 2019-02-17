/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TrafficLightFilterAudioProcessor::TrafficLightFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
                         ),
      tree(*this, nullptr, Identifier("TrafficLightFilter"),
           {std::make_unique< AudioParameterFloat >("cutoff", "Cutoff", 10.0f,
                                                    20000.0f, 1000.0f),
            std::make_unique< AudioParameterFloat >("resonance", "Resonance",
                                                    1.0f, 12.0f, 1.0f),
            std::make_unique< AudioParameterFloat >("amount", "Amount", 0.0f,
                                                    100.0f, 100.0f)

           }),
      highPassFilter(
          dsp::IIR::Coefficients< float >::makeHighPass(44100, 1000.0f, 0.1f))
#endif
{
  NormalisableRange< float > cutRange(10.0f, 20000.0f);
  NormalisableRange< float > resRange(1.0f, 6.0f);
  NormalisableRange< float > amtRange(0.0f, 100.0f);
}

TrafficLightFilterAudioProcessor::~TrafficLightFilterAudioProcessor() {}

//==============================================================================
const String TrafficLightFilterAudioProcessor::getName() const
{
  return JucePlugin_Name;
}

bool TrafficLightFilterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool TrafficLightFilterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool TrafficLightFilterAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double TrafficLightFilterAudioProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int TrafficLightFilterAudioProcessor::getNumPrograms()
{
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int TrafficLightFilterAudioProcessor::getCurrentProgram() { return 0; }

void TrafficLightFilterAudioProcessor::setCurrentProgram(int index) {}

const String TrafficLightFilterAudioProcessor::getProgramName(int index)
{
  return {};
}

void TrafficLightFilterAudioProcessor::changeProgramName(int index,
                                                         const String& newName)
{
}

//==============================================================================
void TrafficLightFilterAudioProcessor::prepareToPlay(double sampleRate,
                                                     int samplesPerBlock)
{
  lastSampleRate = sampleRate;
  std::cout << "running" << std::endl;
  dsp::ProcessSpec spec;
  spec.sampleRate       = sampleRate;
  spec.maximumBlockSize = samplesPerBlock;
  spec.numChannels      = getTotalNumOutputChannels();

  highPassFilter.reset();
  highPassFilter.prepare(spec);
}

void TrafficLightFilterAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TrafficLightFilterAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
  ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;

// This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void TrafficLightFilterAudioProcessor::updateParams()
{
  std::cout << "updateParams called" << std::endl;
  float sFreq = *tree.getRawParameterValue("cutoff");
  float sRes  = *tree.getRawParameterValue("resonance");
  float sAmt  = *tree.getRawParameterValue("amount");
  std::cout << "floats set" << std::endl;
  // going into "state" of the process duplicator
  *highPassFilter.state = *dsp::IIR::Coefficients< float >::makeHighPass(
      lastSampleRate, sFreq, sRes);
  std::cout << "state updated" << std::endl;
  std::cout << "exiting uP" << std::endl;
}

void TrafficLightFilterAudioProcessor::processBlock(
    AudioBuffer< float >& buffer, MidiBuffer& midiMessages)
{
  std::cout << "processBlock called" << std::endl;
  ScopedNoDenormals noDenormals;
  auto totalNumInputChannels  = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
  {
    buffer.clear(i, 0, buffer.getNumSamples());
  }

  std::cout << "declare block" << std::endl;
  dsp::AudioBlock< float > aBlock(buffer);
  std::cout << "about to update params" << std::endl;
  updateParams();
  highPassFilter.process(dsp::ProcessContextReplacing< float >(aBlock));
}

//==============================================================================
bool TrafficLightFilterAudioProcessor::hasEditor() const
{
  return true;  // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TrafficLightFilterAudioProcessor::createEditor()
{
  return new TrafficLightFilterAudioProcessorEditor(*this);
}

//==============================================================================
void TrafficLightFilterAudioProcessor::getStateInformation(
    MemoryBlock& destData)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  auto state = tree.copyState();
  std::unique_ptr< XmlElement > xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void TrafficLightFilterAudioProcessor::setStateInformation(const void* data,
                                                           int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  std::unique_ptr< XmlElement > xmlState(getXmlFromBinary(data, sizeInBytes));
  if (xmlState.get() != nullptr)
    if (xmlState->hasTagName(tree.state.getType()))
      tree.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new TrafficLightFilterAudioProcessor();
}
