/*
------------------------------------------------------------------

This file is part of a plugin for the Open Ephys GUI
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


#include "RateViewerEditor.h"

#include "RateViewerCanvas.h"
#include "RateViewer.h"


RateViewerEditor::RateViewerEditor(GenericProcessor* p)
    : VisualizerEditor(p, "Spike Rate", 210),
      rateViewerCanvas(nullptr)
{

    electrodeList = std::make_unique<ComboBox>("Electrode List");
    electrodeList->addListener(this);
    electrodeList->setBounds(50,40,120,20);
    addAndMakeVisible(electrodeList.get());

    addTextBoxParameterEditor("window_size", 15, 75);

    addTextBoxParameterEditor("bin_size", 120, 75);

    rateViewerNode = (RateViewer*)p;

}

Visualizer* RateViewerEditor::createNewCanvas()
{
    rateViewerCanvas = new RateViewerCanvas(rateViewerNode);

    rateViewerNode->canvas = rateViewerCanvas;

    selectedStreamHasChanged();

    return rateViewerCanvas;
}

void RateViewerEditor::comboBoxChanged(ComboBox* comboBox)
{
    if (comboBox == electrodeList.get())
    {
        if(currentElectrodes.size() == 0)
        {
            rateViewerNode->setActiveElectrode("None");
        }
        else
        {
            rateViewerNode->setActiveElectrode(
                currentElectrodes[electrodeList->getSelectedId() - 1]);
        }
    }
}

void RateViewerEditor::selectedStreamHasChanged()
{
    electrodeList->clear();

    if (selectedStream == 0)
    {
        return;
    }


    currentElectrodes = rateViewerNode->getElectrodesForStream(selectedStream);

    int id = 0;

    for (auto electrode : currentElectrodes)
    {

        electrodeList->addItem(electrode, ++id);
            
    }

    electrodeList->setSelectedId(1, sendNotification);
}
