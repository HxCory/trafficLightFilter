/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
TrafficLightFilterAudioProcessorEditor::TrafficLightFilterAudioProcessorEditor(
    TrafficLightFilterAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(400, 400);
  // these define the parameters of our slider object
  cutoffslider.setSliderStyle(Slider::RotaryVerticalDrag);
  cutoffslider.setRange(10.0, 20000.0, 1.0);
  cutoffslider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
  cutoffslider.setPopupDisplayEnabled(true, false, this);
  cutoffslider.setTextValueSuffix(" Cutoff");
  cutoffslider.setValue(600.0);
  cutoffslider.setColour(Slider::rotarySliderFillColourId, Colours::darkred);
  cutoffslider.setColour(Slider::rotarySliderOutlineColourId, Colours::red);
  addAndMakeVisible(&cutoffslider);
  cutoff.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "cutoff", cutoffslider));

  resslider.setSliderStyle(Slider::RotaryVerticalDrag);
  resslider.setRange(1.0, 6.0, 0.1);
  resslider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
  resslider.setPopupDisplayEnabled(true, false, this);
  resslider.setTextValueSuffix(" Resonance");
  resslider.setValue(1.0);
  resslider.setColour(Slider::rotarySliderFillColourId, Colours::yellow);
  resslider.setColour(Slider::rotarySliderOutlineColourId,
                      Colours::lightyellow);
  addAndMakeVisible(&resslider);
  resonance.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "resonance", resslider));

  amtslider.setSliderStyle(Slider::Rotary);
  amtslider.setRange(0.0, 100.0, 1.0);
  amtslider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
  amtslider.setPopupDisplayEnabled(true, false, this);
  amtslider.setTextValueSuffix(" Amount");
  amtslider.setValue(100.0);
  amtslider.setColour(Slider::rotarySliderFillColourId, Colours::green);
  amtslider.setColour(Slider::rotarySliderOutlineColourId, Colours::lightgreen);
  addAndMakeVisible(&amtslider);
  amount.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "amount", amtslider));

  //  std::cout << "constructing slider attachments" << std::endl;
  //  cutoff = new AudioProcessorValueTreeState::SliderAttachment(
  //      processor.tree, "cutoff", cutoffslider);
  //  resonance = new AudioProcessorValueTreeState::SliderAttachment(
  //      processor.tree, "resonance", resslider);
  //  amount = new AudioProcessorValueTreeState::SliderAttachment(
  //      processor.tree, "amount", amtslider);

  cutoffslider.setSkewFactorFromMidPoint(1000.0f);
}

TrafficLightFilterAudioProcessorEditor::
    ~TrafficLightFilterAudioProcessorEditor()
{
}

//==============================================================================
void TrafficLightFilterAudioProcessorEditor::paint(Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)

  // fill the whole window black
  g.fillAll(Colours::transparentBlack);
  // set the current drawing colour to red
  g.setColour(Colours::darkred);
  // set the font size and draw text to the screen
  g.setFont(15.0f);
  g.drawFittedText("Cutoff", 0.1 * getWidth(), 0.25 * getHeight(), getWidth(),
                   30, Justification::centred, 1);
  g.setColour(Colours::yellow);
  g.drawFittedText("Resonance", 0.1 * getWidth(), 0.5 * getHeight(), getWidth(),
                   30, Justification::centred, 1);
  g.setColour(Colours::green);
  g.drawFittedText("Amount", 0.1 * getWidth(), 0.75 * getHeight(), getWidth(),
                   30, Justification::centred, 1);
}

void TrafficLightFilterAudioProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  cutoffslider.setBounds(0.1 * getWidth(), 0.1 * getHeight(), 0.5 * getWidth(),
                         100);
  resslider.setBounds(0.1 * getWidth(), 0.35 * getHeight(), 0.5 * getWidth(),
                      100);
  amtslider.setBounds(0.1 * getWidth(), 0.6 * getHeight(), 0.5 * getWidth(),
                      100);
}

// void TrafficLightFilterAudioProcessorEditor::sliderValueChanged(Slider*
// slider)
// {
//   processor.cutoffFreq = cutoffslider.getValue();
//   processor.resonance  = resslider.getValue();
//   processor.filterAmt  = amtslider.getValue();
// }
