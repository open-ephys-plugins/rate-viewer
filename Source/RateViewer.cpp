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

#include "RateViewer.h"

#include "RateViewerEditor.h"
#include "RateViewerCanvas.h"

RateViewer::RateViewer() 
    : GenericProcessor("Rate Viewer")
{
    addIntParameter(Parameter::GLOBAL_SCOPE,
                    "window_size",
                    "Size of the window in ms",
                    1000, 100, 5000);
    
    addIntParameter(Parameter::GLOBAL_SCOPE,
                    "bin_size",
                    "Size of the bins in ms",
                    50, 25, 500);
}


RateViewer::~RateViewer()
{

}


AudioProcessorEditor* RateViewer::createEditor()
{
    editor = std::make_unique<RateViewerEditor>(this);
    return editor.get();
}


void RateViewer::updateSettings()
{
    electrodes.clear();
    electrodeMap.clear();

    for(auto spikeChan : spikeChannels)
    {
        if(spikeChan->isValid())
        {
            Electrode* electrode = new Electrode();
            electrode->name = spikeChan->getName();
            electrode->numChannels = spikeChan->getNumChannels();
            electrode->streamId = spikeChan->getStreamId();
            electrode->sampleRate = spikeChan->getSampleRate();
            electrodes.add(electrode);
            electrodeMap[spikeChan] = electrode;
        }
    }

    if (canvas != nullptr)
    {
        parameterValueChanged(getParameter("window_size"));
        parameterValueChanged(getParameter("bin_size"));
    }
    
}

bool RateViewer::startAcquisition()
{
	((RateViewerEditor*)getEditor())->enable();
    return true;
}

bool RateViewer::stopAcquisition()
{
    ((RateViewerEditor*)getEditor())->disable();
    return true;
}


void RateViewer::process(AudioBuffer<float>& buffer)
{	
    checkForEvents(true);

    for (auto stream : getDataStreams())
    {
        if (stream->getStreamId() == getEditor()->getCurrentStream())
        {
            int64 mostRecentSample = getFirstSampleNumberForBlock(stream->getStreamId()) + getNumSamplesInBlock(stream->getStreamId());

            if(canvas != nullptr)
                canvas->setMostRecentSample(mostRecentSample);
        }
    }

}


void RateViewer::parameterValueChanged(Parameter* param)
{

   if (param->getName().equalsIgnoreCase("window_size"))
    {
        int windowSize = (int)param->getValue();

        if(canvas != nullptr)
            canvas->setWindowSizeMs(windowSize);
    }
    else if (param->getName().equalsIgnoreCase("bin_size"))
    {
        int binSize = (int)param->getValue();

        if(canvas != nullptr)
            canvas->setBinSizeMs(binSize);
    }
}


void RateViewer::handleTTLEvent(TTLEventPtr event)
{

}


void RateViewer::handleSpike(SpikePtr spike)
{
    if(spike->getStreamId() == getEditor()->getCurrentStream()
       && electrodeMap.at(spike->getChannelInfo())->isActive
       && canvas != nullptr)
    {
        canvas->addSpike(spike->getSampleNumber());
    } 
}


void RateViewer::handleBroadcastMessage(String message)
{

}


void RateViewer::saveCustomParametersToXml(XmlElement* parentElement)
{

}


void RateViewer::loadCustomParametersFromXml(XmlElement* parentElement)
{

}

Array<String> RateViewer::getElectrodesForStream(uint16 streamId)
{
    Array<String> electrodesForStream;

    for (auto electrode : electrodes)
    {
        if (electrode->streamId == streamId)
            electrodesForStream.add(electrode->name);
    }

    return electrodesForStream;
}

void RateViewer::setActiveElectrode(uint16 streamId, String name)
{
    for (auto electrode : electrodes)
    {
        if (electrode->name.equalsIgnoreCase(name) && electrode->streamId == streamId)
        {
            electrode->isActive = true;

            if(canvas != nullptr)
            {
                canvas->setSampleRate(electrode->sampleRate);
                canvas->setPlotTitle(electrode->name);
            }
        }
        else
        {
            electrode->isActive = false;
        }
    }
}