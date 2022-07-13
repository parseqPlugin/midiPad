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
	setCombo(cc1, 21);
	setCombo(cc2, 22);
	setCombo(cc3, 23);
	setCombo(cc4, 24);

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

	// DEFINE PAD
	addAndMakeVisible(xyPad);

	// RENDER PLUGIN 
	setSize (400, 510);
	//setResizable(true, true);
	//setResizeLimits(250, 360, 400, 510);

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
	
	const int comboWidth = 80;
	const int comboHeight = 25;
	const int margin = 15;

	const int comboRowHeight = comboHeight + (margin * 2);


	// POSITIONNING
	cc1.setBounds(margin, margin, comboWidth, comboHeight);
	cc2.setBounds(panelWidth - comboWidth - margin, margin, comboWidth, comboHeight);
	
	xyPad.setBounds(margin, comboRowHeight, panelWidth - (margin * 2), panelHeight - (comboRowHeight * 2));
	
	cc3.setBounds(margin, panelHeight - comboHeight - margin, comboWidth, comboHeight);
	cc4.setBounds(panelWidth - comboWidth - margin, panelHeight - comboHeight - margin, comboWidth, comboHeight);

	// Sliders are hidden except when debugging
	sliderX.setBounds(125, 10, 150, 20);
	sliderY.setBounds(125, panelHeight - 30, 150, 20);
}



//==============================================================================
// SLIDER PROCESS
//==============================================================================
/*
void MidiPadAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combo)
{
	if (combo == &cc1)
	{
		if (cc1.getSelectedId() == cc2.getSelectedId() || cc1.getSelectedId() == cc3.getSelectedId() || cc1.getSelectedId() == cc4.getSelectedId())
			juce::NativeMessageBox::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Control 1", "This Continuous Controler number is already assigned to another control.\n\nIt is recommended to use different value for each of the control.", nullptr, nullptr);
	}
	else if (combo == &cc2)
	{
		if (cc2.getSelectedId() == cc1.getSelectedId() || cc2.getSelectedId() == cc3.getSelectedId() || cc2.getSelectedId() == cc4.getSelectedId())
			juce::NativeMessageBox::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Control 2", "This Continuous Controler number is already assigned to another control.\n\nIt is recommended to use different value for each of the control.", nullptr, nullptr);
	}
	else if (combo == &cc3)
	{
		if (cc3.getSelectedId() == cc1.getSelectedId() || cc3.getSelectedId() == cc2.getSelectedId() || cc3.getSelectedId() == cc4.getSelectedId())
			juce::NativeMessageBox::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Control 3", "This Continuous Controler number is already assigned to another control.\n\nIt is recommended to use different value for each of the control.", nullptr, nullptr);
	}
	else 
	{
		if (cc4.getSelectedId() == cc1.getSelectedId() || cc4.getSelectedId() == cc2.getSelectedId() || cc4.getSelectedId() == cc3.getSelectedId())
			juce::NativeMessageBox::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "Control 4", "This Continuous Controler number is already assigned to another control.\n\nIt is recommended to use different value for each of the control.", nullptr, nullptr);
	}

}
*/






