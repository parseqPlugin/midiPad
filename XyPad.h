#pragma once
#include <JuceHeader.h>

namespace Gui
{


	class midiPad : public juce::Component, juce::Slider::Listener
	{
	public:

		// XYPAD
		int control1, control2, control3, control4;

		midiPad();
		void resized() override;
		void paint(juce::Graphics& g) override;

		// SLIDERS MANAGEMENT
		enum Axis { X, Y };
		void registerSlider(juce::Slider* slider, Axis axis);
		void deregisterSlider(juce::Slider* slider);


		/*---------------------------------------------------------*/
		// MOUSE MANAGEMENT CLASS
		class Thumb : public juce::Component
		{
		public:
			Thumb();
			void mouseDown(const juce::MouseEvent& event) override;
			void mouseDrag(const juce::MouseEvent& event) override;
			void paint(juce::Graphics& g) override;
			std::function<void(juce::Point<double>)> moveCallback;
		private:
			juce::ComponentBoundsConstrainer constrainer;
			juce::ComponentDragger dragger;
		};
		/*---------------------------------------------------------*/

	private:
		static constexpr int thumbWidth = 20;
		void sliderValueChanged(juce::Slider* slider) override;

		Thumb thumb;
		std::vector<juce::Slider*> xSliders, ySliders;
		std::mutex mutex;
		std::mutex vectorMutex;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(midiPad);
	};	
}