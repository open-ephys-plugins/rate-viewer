/*
	------------------------------------------------------------------

	This file is part of the Open Ephys GUI
	Copyright (C) 2025 Open Ephys

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
#ifndef RATEVIEWEREDITOR_H_DEFINED
#define RATEVIEWEREDITOR_H_DEFINED

#include <VisualizerEditorHeaders.h>

/** 
	The editor for the RateViewer

	Includes buttons for opening the canvas in a tab or window
*/

class RateViewerEditor : public VisualizerEditor,
						 public ComboBox::Listener
{
public:

	/** Constructor */
	RateViewerEditor(GenericProcessor* parentNode);

	/** Destructor */
	~RateViewerEditor() { }

	/** Creates the canvas */
	Visualizer* createNewCanvas() override;

	/** ComboBox::Listener callback*/
    void comboBoxChanged(ComboBox* comboBox) override;

	/** Called when selected stream is updated*/
    void selectedStreamHasChanged() override;

private:

	std::unique_ptr<ComboBox> electrodeList;

	/** Generates an assertion if this class leaks */
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RateViewerEditor);
};


#endif // RATEVIEWEREDITOR_H_DEFINED
