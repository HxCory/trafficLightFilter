#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
 */
class TrafficLightFilterAudioProcessorEditor : public AudioProcessorEditor
{
 public:
  TrafficLightFilterAudioProcessorEditor(TrafficLightFilterAudioProcessor&);
  ~TrafficLightFilterAudioProcessorEditor();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

 private:
  Slider cutoffslider;
  Slider resslider;
  Slider amtslider;

  std::unique_ptr< AudioProcessorValueTreeState::SliderAttachment > cutoff;
  std::unique_ptr< AudioProcessorValueTreeState::SliderAttachment > resonance;
  std::unique_ptr< AudioProcessorValueTreeState::SliderAttachment > amount;
  TrafficLightFilterAudioProcessor& processor;

  //==============================================================================

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
      TrafficLightFilterAudioProcessorEditor)
};
