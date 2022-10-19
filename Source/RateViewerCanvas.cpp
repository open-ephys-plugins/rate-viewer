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
	  sampleRate(0.0f),
	  binSize(0),
	  windowSize(0),
	  mostRecentSample(0),
	  lastValidIndex(-1),
	  maxCount(0)
{
	viewport = std::make_unique<Viewport>("Viewport");
	viewport->setScrollBarsShown(true, true);

	plt.clear();
	plt.xlabel("Offset(ms)");
	plt.ylabel("Rate (Hz)");
	plt.setInteractive(InteractivePlotMode::OFF);
	plt.setBackgroundColour(Colours::darkslategrey);
	plt.show();

	viewport->setViewedComponent(&plt, false);
	addAndMakeVisible(viewport.get());

	setWindowSizeMs(processor->getWindowSizeMs());

}


RateViewerCanvas::~RateViewerCanvas()
{

}


void RateViewerCanvas::resized()
{
	viewport->setBounds(0, 50, getWidth(), getHeight()-50);
	plt.setBounds(0, 0, getWidth() - viewport->getScrollBarThickness(), getHeight() - 100);
}

void RateViewerCanvas::refreshState()
{

}


void RateViewerCanvas::update()
{        
	
}


void RateViewerCanvas::refresh()
{
	recount();
	
	std::vector<float> x, y;

	for(int i = 0; i < binEdges.size() - 1; i++)
	{
		float bin = (binEdges[i] + binEdges[i + 1]) / 2;
		x.push_back(bin);
		y.push_back(counts[i] * 1000 / binSize);
	}

	plt.clear();
	plt.plot(x, y, Colours::lightyellow, 1.0, 1.0f, PlotType::FILLED);
}


void RateViewerCanvas::paint(Graphics& g)
{
	g.fillAll(Colours::darkgrey);

}


void RateViewerCanvas::recount()
{
    
    const int nBins = binEdgesInSamples.size() - 1;
    counts.clear();
	counts.insertMultiple(0, 0, nBins);

	int windowSizeInSamples = windowSize * sampleRate / 1000;
    
    for (int i = 0; i < incomingSpikeSampleNums.size(); i++)
    {
        int relativeSampleNum = incomingSpikeSampleNums[i] - mostRecentSample;
		
		if(relativeSampleNum > windowSizeInSamples)
		{
			lastValidIndex = i + 1;
		}
		else
		{
			for (int j = 0; j < nBins; j++)
			{
				
				if (relativeSampleNum > binEdgesInSamples[j] 
					&& relativeSampleNum < binEdgesInSamples[j+1])
				{
					int lastCount = counts[j];
					int newCount = lastCount + 1;

					maxCount = jmax(newCount, maxCount);
									
					counts.set(j, newCount);

					break;
				}
					
			}
		}
    }

	if(lastValidIndex > -1)
	{
		incomingSpikeSampleNums.removeRange(0, lastValidIndex);
	}

	updatePlotRange();
}


void RateViewerCanvas::updatePlotRange()
{
	XYRange range;
	range.xmin = (float)-windowSize;
	range.xmax = 0.0f;
	range.ymin = 0.0f;
	range.ymax = (float)maxCount * 1000 / binSize;

	plt.setRange(range);
}


void RateViewerCanvas::setWindowSizeMs(int windowSize_)
{
	windowSize = windowSize_;

	setBinSizeMs(processor->getBinSizeMs());

	updatePlotRange();
}

void RateViewerCanvas::setBinSizeMs(int binSize_)
{
	binSize = binSize_;

	recompute();

	maxCount = 1;

	if(CoreServices::getAcquisitionStatus())
		stopCallbacks();

	refreshRate = 1000/binSize;

	if(CoreServices::getAcquisitionStatus())
		startCallbacks();
}

void RateViewerCanvas::recompute()
{
	binEdges.clear();
	binEdgesInSamples.clear();

	if(binSize == 0 || windowSize == 0)
		return;

	double binEdge = (double) -windowSize;

	while(binEdge < 0)
	{
		binEdges.add(binEdge);
		binEdgesInSamples.add(binEdge * sampleRate / 1000);
		binEdge += (double)binSize;
	}

	binEdges.add(0.0);
	binEdgesInSamples.add(0);
}

void RateViewerCanvas::setMostRecentSample(int64 sampleNum)
{
	mostRecentSample = sampleNum;
}

void RateViewerCanvas::setSampleRate(float sampleRate_)
{
	sampleRate = sampleRate_;

	recompute();
}

void RateViewerCanvas::addSpike(int64 sample_num)
{
	incomingSpikeSampleNums.add(sample_num);
}

void RateViewerCanvas::setPlotTitle(const String& title)
{
	plt.title(title);
}