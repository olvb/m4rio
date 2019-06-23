#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

/**
 */
class IPAudioProcessorEditor : public AudioProcessorEditor,
                               private Slider::Listener {
public:
    IPAudioProcessorEditor(IPAudioProcessor&);
    ~IPAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(Slider* slider) override;

    IPAudioProcessor& processor;
    Slider bitSlider;
    Slider inSlider;
    Slider outSlider;
    Slider wetDrySlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IPAudioProcessorEditor)
};
