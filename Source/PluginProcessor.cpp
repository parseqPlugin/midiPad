/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MidiPadAudioProcessor::MidiPadAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
            #if ! JucePlugin_IsMidiEffect
                #if ! JucePlugin_IsSynth
                    .withInput("Input", juce::AudioChannelSet::stereo(), true)
                #endif
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)
            #endif
                ), pluginTreeState(*this, nullptr, "pluginParms", createPluginParms())
#endif
{
    pluginTreeState.state.addListener(this);
}

MidiPadAudioProcessor::~MidiPadAudioProcessor() {}



//========================================================================================================
//========================================================================================================
// MAIN (AUDIO PROCESSOR FUNCTION MODIFIED OR ADD) 


// SENDING DATA ONLY WHEN DRAGGING THE MOUSE
int currentX = 0;
int currentY = 0;


void MidiPadAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::MidiMessage message;

    int x1, x2, y1, y2;
    int cc1Value, cc2Value, cc3Value, cc4Value;
    int cc1Number, cc2Number, cc3Number, cc4Number;

    // Extract new value from TreeState
    int newX = pluginTreeState.getRawParameterValue("X")->load();
    int newY = pluginTreeState.getRawParameterValue("Y")->load();

    // Send values only if the thumb have been dragged in the XYPad
    /**************************************************************
        X Axis value goes from left to right (0 to 127)
        Y Axis value goes from  top to bottom (0 to 127)
        For each corner, we take the lowest of X or Y
    **************************************************************/

    if (newX != currentX || newY != currentY)
    {
        currentX = newX;
        currentY = newY;

        cc1Number = pluginTreeState.getRawParameterValue("CC1")->load();
        cc2Number = pluginTreeState.getRawParameterValue("CC2")->load();
        cc3Number = pluginTreeState.getRawParameterValue("CC3")->load();
        cc4Number = pluginTreeState.getRawParameterValue("CC4")->load();

        x1 = 127 - currentX; 
        x2 = currentX;

        y1 = 127 - currentY; 
        y2 = currentY;

        cc1Value = juce::jmin<int>(x1, y1);
        cc2Value = juce::jmin<int>(x2, y1);
        cc3Value = juce::jmin<int>(x1, y2);
        cc4Value = juce::jmin<int>(x2, y2);
        
        midiMessages.addEvent(juce::MidiMessage::controllerEvent(1, cc1Number, cc1Value), midiMessages.getLastEventTime());
        midiMessages.addEvent(juce::MidiMessage::controllerEvent(1, cc2Number, cc2Value), midiMessages.getLastEventTime());
        midiMessages.addEvent(juce::MidiMessage::controllerEvent(1, cc3Number, cc3Value), midiMessages.getLastEventTime());
        midiMessages.addEvent(juce::MidiMessage::controllerEvent(1, cc4Number, cc4Value), midiMessages.getLastEventTime());
    }
    



}

void MidiPadAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
}

juce::AudioProcessorValueTreeState::ParameterLayout MidiPadAudioProcessor::createPluginParms()
{
    // Vector of unique pointer for the parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // Range of audio  for each sliders
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("X", "x", 0, 127, 64));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("Y", "y", 0, 127, 64));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("CC1", "CC number 1", 1, 127, 10));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("CC2", "CC number 2", 1, 127, 11));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("CC3", "CC number 3", 1, 127, 12));
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("CC4", "CC number 4", 1, 127, 13));

    // Return the parameters (vector)
    return { parameters.begin(), parameters.end() };

}


void MidiPadAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = pluginTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MidiPadAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(pluginTreeState.state.getType()))
            pluginTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}



// END OF MAIN PROCESSING
//==============================================================================
//==============================================================================

const juce::String MidiPadAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiPadAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiPadAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiPadAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiPadAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiPadAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiPadAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiPadAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MidiPadAudioProcessor::getProgramName (int index)
{
    return {};
}

void MidiPadAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void MidiPadAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MidiPadAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiPadAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

bool MidiPadAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiPadAudioProcessor::createEditor()
{
    return new MidiPadAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiPadAudioProcessor();
}
