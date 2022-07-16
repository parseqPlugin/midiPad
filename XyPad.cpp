#include "XyPad.h"


namespace Gui
{
	// MIDIPAD - MAIN PROCESS
	void midiPad::Thumb::mouseDrag(const juce::MouseEvent& event)
	{
		int x, y;

		dragger.dragComponent(this, event, &constrainer);
		if (moveCallback)
		{
			x = getPosition().getX();
			y = getPosition().getY();
			moveCallback(getPosition().toDouble());
		}

	}


	// MIDIPAD - STYLE
	void midiPad::paint(juce::Graphics& g)
	{
		const auto bounds = getLocalBounds().toFloat();

		g.setColour(juce::Colour::fromRGB(67, 67, 67));
		g.fillRoundedRectangle(bounds, 10);

		const auto r = thumbWidth / 2.f;
		const auto thumbX = thumb.getX() + r;
		const auto thumbY = thumb.getY() + r;

		g.setColour(juce::Colour::fromRGB(185, 185, 185));
		g.drawLine(juce::Line<float> { {0.f, thumbY}, { bounds.getWidth(), thumbY } });

		g.setColour(juce::Colour::fromRGB(185, 185, 185));
		g.drawLine(juce::Line<float> { {thumbX, 0.f}, { thumbX, bounds.getHeight() } });

		juce::Path circle;
		circle.addEllipse(thumb.getBoundsInParent().toFloat());
		const juce::DropShadow dropShadow;
		dropShadow.drawForPath(g, circle);
	}
	
	//=========================================================================================
	// SLIDER LISTENING
	void midiPad::registerSlider(juce::Slider* slider, Axis axis)
	{
		slider->addListener(this);
		
		const std::lock_guard<std::mutex> lock(vectorMutex);
		if (axis == Axis::X)
		{
			xSliders.push_back(slider);
		}
		if (axis == Axis::Y)
		{
			ySliders.push_back(slider);
		}
	}

	void midiPad::deregisterSlider(juce::Slider* slider)
	{
		slider->removeListener(this);
		
		const std::lock_guard<std::mutex> lock(vectorMutex);
		xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end());
		ySliders.erase(std::remove(ySliders.begin(), ySliders.end(), slider), ySliders.end());
	}

	void midiPad::sliderValueChanged(juce::Slider* slider)
	{
		int posX = 0;
		int posY = 0;

		// Avoid loopback
		if (thumb.isMouseOverOrDragging(false))
			return;

		//Apply changed for the corresponding 
		const auto isSliderX = std::find(xSliders.begin(), xSliders.end(), slider) != xSliders.end();
		const auto bounds = getLocalBounds().toDouble();
		const auto w = static_cast<double>(thumbWidth);

		if (isSliderX)
		{
			posX = juce::jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), 0.0, bounds.getWidth() - w);
			posY = thumb.getY();
		}
		else
		{
			posX = thumb.getX();
			posY = juce::jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), 0.0, bounds.getHeight() - w);
		}

		thumb.setTopLeftPosition(posX, posY);
		repaint();
		
	}

	//=========================================================================================
	midiPad::midiPad()
	{
		addAndMakeVisible(thumb);
		thumb.moveCallback = [&](juce::Point<double> position)
		{
			const std::lock_guard<std::mutex> lock(mutex);
			const auto bounds = getLocalBounds().toDouble();
			const auto w = static_cast<double>(thumbWidth);

			for (auto* slider : xSliders)
			{
				slider->setValue(juce::jmap(position.getX(), 0.0, bounds.getWidth() - w, slider->getMinimum(), slider->getMaximum()));
			}

			for (auto* slider : ySliders)
			{
				slider->setValue(juce::jmap(position.getY(), 0.0, bounds.getHeight() - w, slider->getMinimum(), slider->getMaximum()));
			}

			repaint();
		};
	}

	//=========================================================================================
	midiPad::Thumb::Thumb()
	{
		constrainer.setMinimumOnscreenAmounts(thumbWidth, thumbWidth, thumbWidth, thumbWidth);
	}

	void midiPad::Thumb::mouseDown(const juce::MouseEvent& event)
	{
		dragger.startDraggingComponent(this, event);
	}

	void midiPad::Thumb::paint(juce::Graphics& g)
	{		
		g.setColour(juce::Colour::fromRGB(240,138,7));
		g.fillEllipse(getLocalBounds().toFloat());
	}


	void midiPad::resized()
	{		
		thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbWidth, thumbWidth));

		//Force a changed to apply the value to the XYPAD when reloading the plugin
		if (!xSliders.empty())
			sliderValueChanged(xSliders[0]);
		if (!ySliders.empty())
			sliderValueChanged(ySliders[0]);
	}

}
