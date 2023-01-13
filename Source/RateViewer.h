/*
	------------------------------------------------------------------

	This file is part of the Open Ephys GUI
	Copyright (C) 2022 Open Ephys

	------------------------------------------------------------------

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

//This prevents include loops. We recommend changing the macro to a name suitable for your plugin
#ifndef RATEVIEWER_H_DEFINED
#define RATEVIEWER_H_DEFINED

#include <ProcessorHeaders.h>

class RateViewerCanvas;

/** 
	A plugin that includes a canvas for displaying incoming data
	or an extended settings interface.
*/

class RateViewer : public GenericProcessor
{
public:
	/** The class constructor, used to initialize any members.*/
	RateViewer();

	/** The class destructor, used to deallocate memory*/
	~RateViewer();

	/** If the processor has a custom editor, this method must be defined to instantiate it. */
	AudioProcessorEditor* createEditor() override;

	/** Called every time the settings of an upstream plugin are changed.
		Allows the processor to handle variations in the channel configuration or any other parameter
		passed through signal chain. The processor can use this function to modify channel objects that
		will be passed to downstream plugins. */
	void updateSettings() override;

	/** Defines the functionality of the processor.
		The process method is called every time a new data buffer is available.
		Visualizer plugins typically use this method to send data to the canvas for display purposes */
	void process(AudioBuffer<float>& buffer) override;

	/** Used to alter parameters of data acquisition. */
    void parameterValueChanged(Parameter* param) override;

	/** Handles events received by the processor
		Called automatically for each received event whenever checkForEvents() is called from
		the plugin's process() method */
	void handleTTLEvent(TTLEventPtr event) override;

	/** Handles spikes received by the processor
		Called automatically for each received spike whenever checkForEvents(true) is called from 
		the plugin's process() method */
	void handleSpike(SpikePtr spike) override;

	/** Handles broadcast messages sent during acquisition
		Called automatically whenever a broadcast message is sent through the signal chain */
	void handleBroadcastMessage(String message) override;

	/** Saving custom settings to XML. This method is not needed to save the state of
		Parameter objects */
	void saveCustomParametersToXml(XmlElement* parentElement) override;

	/** Load custom settings from XML. This method is not needed to load the state of
		Parameter objects*/
	void loadCustomParametersFromXml(XmlElement* parentElement) override;

	/** Enables the editor */
    bool startAcquisition() override;

    /** Disables the editor*/
    bool stopAcquisition() override;

	/** Returns an array of available electrodes*/
    Array<String> getElectrodesForStream(uint16 streamId);

	void setActiveElectrode(uint16 streamId, String name);

	/** Pointer to the Visualizer -- initialize to nullptr*/
	RateViewerCanvas* canvas = nullptr;

private:

	struct Electrode
    {
        String name;

        int numChannels;

		uint16 streamId;

		float sampleRate;

		bool isActive = false;
    };


	OwnedArray<Electrode> electrodes;
	std::map<const SpikeChannel*, Electrode*> electrodeMap;

	/** Generates an assertion if this class leaks */
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RateViewer);

};

#endif // RATEVIEWER_H_DEFINED