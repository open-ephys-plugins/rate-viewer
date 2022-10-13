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

#include "RateViewerCanvas.h"

#include "RateViewer.h"


RateViewerCanvas::RateViewerCanvas(RateViewer* processor_)
	: processor(processor_),
	  windowSize(0),
	  binSize(0),
	  mostRecentSample(0)
{
	viewport = std::make_unique<Viewport>("Viewport");
	viewport->setScrollBarsShown(true, true);

	plt.title("Spike Rate");
	plt.show();

	viewport->setViewedComponent(&plt, false);
	addAndMakeVisible(viewport.get());
}


RateViewerCanvas::~RateViewerCanvas()
{

}


void RateViewerCanvas::resized()
{

}

void RateViewerCanvas::refreshState()
{

}


void RateViewerCanvas::update()
{        
	for (auto sample_number : incomingSpikeSampleNums)
	{
		if(sample_number > (mostRecentSample - windowSize))
		{
			
		}
		
	}
	
	incomingSpikeSampleNums.clear();
}


void RateViewerCanvas::refresh()
{

}


void RateViewerCanvas::paint(Graphics& g)
{

	g.fillAll(Colours::black);

}


void RateViewerCanvas::setWindowSizeMs(int windowSize_)
{
	windowSize = windowSize_;

	setBinSizeMs(binSize);
}

void RateViewerCanvas::setBinSizeMs(int binSize_)
{
	binSize = binSize_;

	binEdges.clear();

	double binEdge = (double) -windowSize;

	while(binEdge < 0)
	{
		binEdges.add(binEdge);
		binEdge += (double)binSize;
	}

	binEdges.add(0);
}

void RateViewerCanvas::setMostRecentSample(int64 sampleNum)
{
	mostRecentSample = sampleNum;
}

void RateViewerCanvas::addSpike(int64 sample_num)
{
	incomingSpikeSampleNums.add(sample_num);
}