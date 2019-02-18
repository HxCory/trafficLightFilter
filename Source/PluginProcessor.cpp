#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

// The constructor initializes our filter, as well
// as the AudioProcessorValueTree, from which
// we will access the values for the filter parameters
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
  return 1;  // this should be at least 1
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
  // need a ProcessSpec to prepare the filter
  lastSampleRate = sampleRate;
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
  // Check if the layout is supported.
  // Currently we only support mono or stereo.
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

// function for updating the filter parameters
void TrafficLightFilterAudioProcessor::updateParams()
{
  // get cutoff frequency and resonance values from value tree
  float sFreq = *tree.getRawParameterValue("cutoff");
  float sRes  = *tree.getRawParameterValue("resonance");

  // going into the state of the processor duplicator, and updating the
  // parameters of the filter
  *highPassFilter.state = *dsp::IIR::Coefficients< float >::makeHighPass(
      lastSampleRate, sFreq, sRes);
}

void TrafficLightFilterAudioProcessor::processBlock(
    AudioBuffer< float >& buffer, MidiBuffer& midiMessages)
{
  ScopedNoDenormals noDenormals;
  auto totalNumInputChannels  = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
  {
    buffer.clear(i, 0, buffer.getNumSamples());
  }

  // create buffer, update filter parameters with
  // updateParams function (see above)
  dsp::AudioBlock< float > aBlock(buffer);
  updateParams();

  // use Amount knob as a gain knob before processing audio block
  // NB: in the future, want this to be a distortion knob instead
  float sAmt = *tree.getRawParameterValue("amount");
  buffer.applyGain(sAmt / 100.0);
  highPassFilter.process(dsp::ProcessContextReplacing< float >(aBlock));
}

//==============================================================================
bool TrafficLightFilterAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor* TrafficLightFilterAudioProcessor::createEditor()
{
  return new TrafficLightFilterAudioProcessorEditor(*this);
}

//==============================================================================
void TrafficLightFilterAudioProcessor::getStateInformation(
    MemoryBlock& destData)
{
  // store parameters in the memory block.
  auto state = tree.copyState();
  std::unique_ptr< XmlElement > xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void TrafficLightFilterAudioProcessor::setStateInformation(const void* data,
                                                           int sizeInBytes)
{
  // restore parameters from memory block
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
