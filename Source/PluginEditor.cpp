#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
TrafficLightFilterAudioProcessorEditor::TrafficLightFilterAudioProcessorEditor(
    TrafficLightFilterAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
  setSize(400, 400);

  cutoffslider.setSliderStyle(Slider::RotaryVerticalDrag);
  cutoffslider.setRange(10.0, 20000.0, 1.0);
  cutoffslider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
  cutoffslider.setPopupDisplayEnabled(true, false, this);
  cutoffslider.setTextValueSuffix(" Cutoff");
  cutoffslider.setValue(1000.0);
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
  resslider.setColour(Slider::rotarySliderFillColourId, Colours::goldenrod);
  resslider.setColour(Slider::rotarySliderOutlineColourId, Colours::yellow);
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

  cutoffslider.setSkewFactorFromMidPoint(1000.0f);
}

TrafficLightFilterAudioProcessorEditor::
    ~TrafficLightFilterAudioProcessorEditor()
{
}

//==============================================================================
void TrafficLightFilterAudioProcessorEditor::paint(Graphics& g)
{
  auto fixture = Rectangle< float >(0.2 * getWidth(), 0.05 * getHeight(),
                                    0.3 * getWidth(), 0.85 * getHeight());
  auto background = Rectangle< float >(0.0, 0.0, getWidth(), getHeight());
  auto grad = ColourGradient(Colours::dimgrey, 0.0, 0.0, Colours::darkslateblue,
                             350.0, 350.0, false);
  g.setGradientFill(grad);
  g.fillRect(background);
  g.setColour(Colours::black);
  g.fillRoundedRectangle(fixture, 12.0);
  g.setColour(Colours::indianred);
  g.setFont(25.0f);

  g.drawFittedText("Cutoff", 0.18 * getWidth(), 0.2 * getHeight(), getWidth(),
                   30, Justification::centred, 1);
  g.setColour(Colours::goldenrod);
  g.drawFittedText("Resonance", 0.18 * getWidth(), 0.45 * getHeight(),
                   getWidth(), 30, Justification::centred, 1);
  g.setColour(Colours::green);
  g.drawFittedText("Amount", 0.18 * getWidth(), 0.7 * getHeight(), getWidth(),
                   30, Justification::centred, 1);
}

void TrafficLightFilterAudioProcessorEditor::resized()
{
  cutoffslider.setBounds(0.1 * getWidth(), 0.1 * getHeight(), 0.5 * getWidth(),
                         100);
  resslider.setBounds(0.1 * getWidth(), 0.35 * getHeight(), 0.5 * getWidth(),
                      100);
  amtslider.setBounds(0.1 * getWidth(), 0.6 * getHeight(), 0.5 * getWidth(),
                      100);
}
