#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 */
class TrafficLightFilterAudioProcessor : public AudioProcessor,
                                         public IIRFilter,
                                         public IIRCoefficients
{
 public:
  //==============================================================================
  TrafficLightFilterAudioProcessor();
  ~TrafficLightFilterAudioProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(AudioBuffer< float >&, MidiBuffer&) override;

  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String& newName) override;

  //==============================================================================
  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  void updateParams();
  AudioProcessorValueTreeState tree;

 private:
  float lastSampleRate;
  float previousAmt;

  // Use a ProcessorDuplicator for the IIR filter to turn mono into stereo
  dsp::ProcessorDuplicator< dsp::IIR::Filter< float >,
                            dsp::IIR::Coefficients< float > >
      highPassFilter;

  //==============================================================================
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrafficLightFilterAudioProcessor)
};

// Distortion class not implemented yet
template < typename Type >
class Distortion
{
 public:
  //==============================================================================
  Distortion()
  {
    auto& waveshaper = processorChain.template get< waveshaperIndex >();  // [5]
    waveshaper.functionToUse = [](Type x) {
      return jlimit(Type(-0.1), Type(0.1), x);  // [6]
    };
  }

  //==============================================================================
  void prepare(const juce::dsp::ProcessSpec& spec)
  {
    processorChain.prepare(spec);
  }

  //==============================================================================
  template < typename ProcessContext >
  void process(const ProcessContext& context) noexcept
  {
    processorChain.process(context);
  }

  //==============================================================================
  void reset() noexcept { processorChain.reset(); }

 private:
  //==============================================================================
  enum
  {
    waveshaperIndex
  };

  juce::dsp::ProcessorChain< juce::dsp::WaveShaper< Type > > processorChain;
};
