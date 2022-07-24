#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "XyPad.h"




//==============================================================================
// MAIN EDITOR PROCESS
//==============================================================================

MidiPadAudioProcessorEditor::MidiPadAudioProcessorEditor (MidiPadAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // DEFINE COMBO TO SELECT CC NUMBER
	setCombo(cc1);
	setCombo(cc2);
	setCombo(cc3);
	setCombo(cc4);

	// DEFINE SLIDERS (not shown)
	setSlider(sliderX);
	setSlider(sliderY);

	//REGISTER SLIDERS FOR XYPAD
	xyPad.registerSlider(&sliderX, Gui::midiPad::Axis::X);
	xyPad.registerSlider(&sliderY, Gui::midiPad::Axis::Y);

    // Define pointer to Audio Processor to update the ValueTreeState
	xSliderAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getPluginTreeState(), "X", sliderX);
	ySliderAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getPluginTreeState(), "Y", sliderY);
	cc1ComboAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getPluginTreeState(), "CC1", cc1);
	cc2ComboAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getPluginTreeState(), "CC2", cc2);
	cc3ComboAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getPluginTreeState(), "CC3", cc3);
	cc4ComboAttachement = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getPluginTreeState(), "CC4", cc4);

	// Define plugin name
	addAndMakeVisible(pluginName);
	pluginName.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromRGB(116, 116, 116));
	pluginName.setJustificationType(juce::Justification::centred);
	pluginName.setText("midiPad 0.9.6", juce::NotificationType::dontSendNotification);

	// DEFINE PAD
	addAndMakeVisible(xyPad);

	// RENDER PLUGIN 
	setSize (430, 525);
	setResizable(true, true);
	setResizeLimits(250, 360, 800, 910);

}

MidiPadAudioProcessorEditor::~MidiPadAudioProcessorEditor() 
{
	xyPad.deregisterSlider(&sliderX);
	xyPad.deregisterSlider(&sliderY);
}


void MidiPadAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(52, 52, 52));
}

void MidiPadAudioProcessorEditor::resized()
{
    const auto panel = getLocalBounds();
	const auto panelWidth = panel.getWidth();
	const auto panelHeight = panel.getHeight();
	
	const int marginHeader = 30;
	const int margin = 15;

	const int comboWidth = 80;
	const int comboHeight = 25;

	const int padTop = marginHeader + comboHeight + margin;

	const int labelHeight = 25;

	// Positionning controls
	cc1.setBounds(margin, marginHeader, comboWidth, comboHeight);
	cc2.setBounds(panelWidth - comboWidth - margin, marginHeader, comboWidth, comboHeight);
	xyPad.setBounds(margin, padTop, panelWidth - (margin * 2), panelHeight - comboHeight - (margin * 2) - padTop);
	cc3.setBounds(margin, panelHeight - comboHeight - margin, comboWidth, comboHeight);
	cc4.setBounds(panelWidth - comboWidth - margin, panelHeight - comboHeight - margin, comboWidth, comboHeight);

	// Positionning plgin name
	pluginName.setBounds(0, 5, panelWidth, labelHeight);

	// Sliders are hidden except when debugging
	sliderX.setBounds(125, 10, 150, 20);
	sliderY.setBounds(125, panelHeight - 30, 150, 20);
}





