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

Electrode::Electrode(SpikeChannel* channel)
    : isActive(true)
{

    name = channel->getName();
    streamName = channel->getStreamName();

    streamId = channel->getStreamId();
    uniqueId = channel->getUniqueId();

    numChannels = channel->getNumChannels();
    numSamples = channel->getPrePeakSamples() + channel->getPostPeakSamples();
}

bool Electrode::matchesChannel(SpikeChannel* channel)
{
    if (channel->getUniqueId() == uniqueId)
    {
        isActive = true;
        return true;
    }
    else {
        return false;
    }

}

void Electrode::updateSettings(SpikeChannel* channel)
{
    name = channel->getName();
}


RateViewer::RateViewer() 
    : GenericProcessor("Rate Viewer")
{

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
    for(auto electrode : electrodes)
        electrode->reset();

    for(auto spikeChan : spikeChannels)
    {
        if(spikeChan->isValid())
        {
            bool foundMatch = false;

            for(auto electrode : electrodes)
            {
                if(electrode->matchesChannel(spikeChan))
                {
                    electrode->updateSettings(spikeChan);
                    electrodeMap[spikeChan] = electrode;
                    foundMatch = true;
                    break;
                }
            }

            if(!foundMatch)
            {
                Electrode* electrode = new Electrode(spikeChan);
                electrodes.add(electrode);
                electrodeMap[spikeChan] = electrode;
            }
        }
    }
}


void RateViewer::process(AudioBuffer<float>& buffer)
{

    checkForEvents(true);
	 
}


void RateViewer::handleTTLEvent(TTLEventPtr event)
{

}


void RateViewer::handleSpike(SpikePtr event)
{

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