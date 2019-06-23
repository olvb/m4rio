
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cassert>
#include <cmath>

IPAudioProcessor::IPAudioProcessor()
    :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo(), true).withOutput("Output", AudioChannelSet::stereo(), true))
#endif
    , inGain(1.0f)
    , bitDepth(8)
    , outGain(1.0f)
    , wetDryBal(1.0f)
    , targetInGain(1.0f)
    , targetBitDepth(8)
    , targetOutGain(1.0f)
    , targetWetDryBal(1.0f) {
}

IPAudioProcessor::~IPAudioProcessor() {}

AudioProcessorEditor* IPAudioProcessor::createEditor() {
    return new IPAudioProcessorEditor(*this);
}

void IPAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void IPAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IPAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo()) {
        return false;
    }

    // This checks if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
        return false;
    }

    return true;
}
#endif

void IPAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // 0-clear output channels with no signal
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    if (inGain == targetInGain && bitDepth == targetBitDepth && outGain == targetOutGain && wetDryBal == targetWetDryBal) {
        float wetGain = wetDryBal;
        float dryGain = 1.0f - wetDryBal;
        float maxValue = (float) exp2(bitDepth) / 2.0f;

        for (int channel = 0; channel < totalNumInputChannels && channel < totalNumOutputChannels; channel++) {
            auto* inChannelData = buffer.getReadPointer(channel);
            auto* outChannelData = buffer.getWritePointer(channel);

            for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
                float wetValue = inChannelData[sample] * inGain;
                wetValue = (float) roundToInt(wetValue * maxValue) / maxValue;
                wetValue *= outGain * wetGain;

                float dryValue = inChannelData[sample] * dryGain;
                outChannelData[sample] = wetValue + dryValue;
            }
        }
        return;
    }

    // progessively adjust values to avoid clicks

    float inGainStep = (targetInGain - inGain) / buffer.getNumSamples();
    float bitDepthStep = (float) (targetBitDepth - bitDepth) / buffer.getNumSamples();
    float outGainStep = (targetOutGain - outGain) / buffer.getNumSamples();
    float wetDryBalStep = (targetWetDryBal - wetDryBal) / buffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels && channel < totalNumOutputChannels; channel++) {
        auto* inChannelData = buffer.getReadPointer(channel);
        auto* outChannelData = buffer.getWritePointer(channel);

        float tempInGain = inGain;
        float tempBitDepth = bitDepth;
        float tempWetDryBal = wetDryBal;
        float tempOutGain = outGain;

        for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
            tempInGain += inGainStep;
            tempBitDepth += bitDepthStep;
            tempOutGain += outGainStep;
            tempWetDryBal += wetDryBalStep;

            float tempMaxValue = (float) exp2(tempBitDepth) / 2.0f;
            float tempWetGain = tempWetDryBal;
            float tempDryGain = (1.0f - tempWetDryBal);

            float wetValue = inChannelData[sample] * tempInGain;
            wetValue = (float) roundToInt(wetValue * tempMaxValue) / tempMaxValue;
            wetValue *= tempOutGain * tempWetGain;

            float dryValue = inChannelData[sample] * tempDryGain;
            outChannelData[sample] = wetValue + dryValue;
        }
    }

    inGain = targetInGain;
    bitDepth = targetBitDepth;
    outGain = targetOutGain;
    wetDryBal = targetWetDryBal;
}

void IPAudioProcessor::getStateInformation(MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void IPAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new IPAudioProcessor();
}
