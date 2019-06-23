#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/**
 */
class IPAudioProcessor : public AudioProcessor {
public:
    IPAudioProcessor();
    ~IPAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const String& newName) override {}

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void setInGain(float inGain) { this->targetInGain = inGain; }
    float getInGain() const { return this->targetInGain; }

    void setBitDepth(int bitDepth) { this->targetBitDepth = bitDepth; }
    int getBitDepth() const { return this->targetBitDepth; }

    void setOutGain(float outGain) { this->targetOutGain = outGain; }
    float getOutGain() const { return this->targetOutGain; }

    void setWetDryBal(float wetDryBal) { this->targetWetDryBal = wetDryBal; }
    float getWetDryBal() const { return this->targetWetDryBal; }

private:
    float inGain;
    int bitDepth;
    float outGain;
    float wetDryBal;

    float targetInGain;
    int targetBitDepth;
    float targetOutGain;
    float targetWetDryBal;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IPAudioProcessor)
};
