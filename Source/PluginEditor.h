/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

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
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
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
