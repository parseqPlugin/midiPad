#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "XyPad.h"



class MidiPadAudioProcessorEditor : public juce::AudioProcessorEditor
{
public: MidiPadAudioProcessorEditor (MidiPadAudioProcessor&); 
       ~MidiPadAudioProcessorEditor() override;

	   
	   //==============================================================================
	// AUDIO PROCESSOR EDITOR CLASS FUNCTIONS
	void paint(juce::Graphics&) override;
	void resized() override;


	//==============================================================================
	// USER FUNCTION

	void setSlider(juce::Slider& slider)
	{
		// Slider are defined as listener but are not shown on the panel
		addAndMakeVisible(slider);
		slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
		slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromRGB(185, 185, 185));
		slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromRGB(240, 138, 7));
		slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
		slider.setVisible(false); // Sliders are hidden except when debugging
	}


	void setCombo(juce::ComboBox& comboBox, int defaultCC)
	{
		addAndMakeVisible(comboBox);
		comboBox.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
		for (int i = 1; i <= 127; i++)
		{
			comboBox.addItem("CC " + std::to_string(i), i);
		}
		comboBox.setSelectedId(defaultCC);
	}



private:
	juce::Label pluginName;
	juce::ComboBox cc1, cc2, cc3, cc4;
	juce::Slider sliderX, sliderY;
	Gui::midiPad xyPad;
    
	// Unique pointer to attche slider to the audio processor
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> xSliderAttachement;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ySliderAttachement;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> cc1ComboAttachement;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> cc2ComboAttachement;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> cc3ComboAttachement;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> cc4ComboAttachement;

    // Pointer to the Audio Processor
	MidiPadAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPadAudioProcessorEditor)
};
