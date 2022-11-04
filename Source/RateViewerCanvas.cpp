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
	: processor(processor_)
{

	plt.xlabel("Offset(ms)");
	plt.ylabel("Rate (Hz)");
	plt.setInteractive(InteractivePlotMode::OFF);
	plt.setBackgroundColour(Colours::darkslategrey);
	addAndMakeVisible(&plt);

	plt.setBounds(50, 50, 800, 500);

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
	updatePlotRange();
}


void RateViewerCanvas::refresh()
{
	if (countSpikes())
	{
		std::vector<float> x, y;

		for (int i = 0; i < binEdges.size() - 1; i++)
		{
			x.push_back(binEdges[i]);
			y.push_back(counts[i] * 1000 / binSize);
		}

		plt.clear();
		plt.plot(x, y, Colours::lightyellow, 1.0, 1.0f, PlotType::FILLED);
	}

}


void RateViewerCanvas::paint(Graphics& g)
{
	
	g.fillAll(Colours::darkgrey);

}


bool RateViewerCanvas::countSpikes()
{
    
	int elapsedSamples = mostRecentSample - sampleOnLastRedraw;
	float elapsedTimeMs = float(elapsedSamples) / sampleRate * 1000.0f;

	if (elapsedTimeMs < binSize)
		return false;

	counts.remove(0); // remove oldest count

	int newSpikeCount = incomingSpikeSampleNums.size();

	if (newSpikeCount > maxCount)
		maxCount = newSpikeCount;

	counts.add(newSpikeCount); // add most recent count

	incomingSpikeSampleNums.clear();

	updatePlotRange();

	sampleOnLastRedraw = mostRecentSample;

	return true;
	
}


void RateViewerCanvas::updatePlotRange()
{
	XYRange range;
	range.xmin = (float)-windowSize;
	range.xmax = (float) -binSize;
	range.ymin = 0.0f;
	range.ymax = (float)maxCount * 1000 / binSize;

	plt.setRange(range);
}


void RateViewerCanvas::setWindowSizeMs(int windowSize_)
{
	windowSize = windowSize_;

	recomputeBinEdges();

	updatePlotRange();
}

void RateViewerCanvas::setBinSizeMs(int binSize_)
{
	binSize = binSize_;

	recomputeBinEdges();

	maxCount = 1;
}

void RateViewerCanvas::recomputeBinEdges()
{
	
	binEdges.clear();
	counts.clear();

	if (binSize == 0 || windowSize == 0)
		return;

	double binEdge = (double) -windowSize;

	while (binEdge < 0)
	{
		binEdges.add(binEdge);
		binEdge += (double)binSize;
	}

	binEdges.add(0.0);

	counts.insertMultiple(0, 0, binEdges.size());
}


void RateViewerCanvas::addSpike(int64 sample_num)
{
	incomingSpikeSampleNums.add(sample_num);
}

void RateViewerCanvas::setMostRecentSample(int64 sampleNum)
{
	mostRecentSample = sampleNum;
}


void RateViewerCanvas::setSampleRate(float sampleRate_)
{
	sampleRate = sampleRate_;

	recomputeBinEdges();
}

void RateViewerCanvas::setPlotTitle(const String& title)
{
	plt.title(title);
}