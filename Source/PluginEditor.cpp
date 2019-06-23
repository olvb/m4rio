#include "PluginEditor.h"
#include "PluginProcessor.h"

IPAudioProcessorEditor::IPAudioProcessorEditor(IPAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , processor(p) {
    setSize(400, 300);

    bitSlider.setSliderStyle(Slider::LinearHorizontal);
    bitSlider.setRange(1.0, 32.0, 1.0);
    bitSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    bitSlider.setPopupDisplayEnabled(true, false, this);
    bitSlider.setTextValueSuffix(" bit");
    bitSlider.setValue(p.getBitDepth());
    bitSlider.addListener(this);
    addAndMakeVisible(&bitSlider);

    inSlider.setSliderStyle(Slider::LinearHorizontal);
    inSlider.setRange(-100.0f, 20.0f, 1.0f);
    inSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    inSlider.setPopupDisplayEnabled(true, false, this);
    inSlider.setTextValueSuffix(" dB");
    float dbGain = Decibels::gainToDecibels(p.getInGain());
    inSlider.setValue(dbGain);
    inSlider.addListener(this);
    addAndMakeVisible(&inSlider);

    outSlider.setSliderStyle(Slider::LinearHorizontal);
    outSlider.setRange(-100.0f, 20.0f, 1.0f);
    outSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    outSlider.setPopupDisplayEnabled(true, false, this);
    outSlider.setTextValueSuffix(" dB");
    dbGain = Decibels::gainToDecibels(p.getOutGain());
    outSlider.setValue(dbGain);
    outSlider.addListener(this);
    addAndMakeVisible(&outSlider);

    //wetDrySlider.setSliderStyle(Slider::Rotary);
    wetDrySlider.setSliderStyle(Slider::LinearHorizontal);
    wetDrySlider.setRange(0.0f, 1.0f);
    wetDrySlider.setValue(p.getWetDryBal());
    wetDrySlider.addListener(this);
    addAndMakeVisible(&wetDrySlider);
}

IPAudioProcessorEditor::~IPAudioProcessorEditor() {}

void IPAudioProcessorEditor::paint(Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Bit crusher", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void IPAudioProcessorEditor::resized() {
    bitSlider.setBounds(20, 20, getWidth() - 60, 20);
    inSlider.setBounds(20, 60, getWidth() - 60, 20);
    outSlider.setBounds(20, 140, getWidth() - 60, 20);
    wetDrySlider.setBounds(20, 100, getWidth() - 60, 20);
}

void IPAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    if (slider == &bitSlider) {
        processor.setBitDepth(bitSlider.getValue());
    } else if (slider == &inSlider) {
        float gain = Decibels::decibelsToGain(inSlider.getValue());
        processor.setInGain(gain);
    } else if (slider == &outSlider) {
        float gain = Decibels::decibelsToGain(outSlider.getValue());
        processor.setOutGain(gain);
    } else if (slider == &wetDrySlider) {
        processor.setWetDryBal(wetDrySlider.getValue());
    }
}
