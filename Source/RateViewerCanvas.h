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

#ifndef RATEVIEWERCANVAS_H_DEFINED
#define RATEVIEWERCANVAS_H_DEFINED

#include <VisualizerWindowHeaders.h>

class RateViewer;

/**
* 
	Draws data in real time

*/
class RateViewerCanvas : public Visualizer
{
public:

	/** Constructor */
	RateViewerCanvas(RateViewer* processor);

	/** Destructor */
	~RateViewerCanvas();

	/** Updates boundaries of sub-components whenever the canvas size changes */
	void resized() override;

	/** Called when the visualizer's tab becomes visible again */
	void refreshState() override;

	/** Updates settings */
	void update() override;

	/** Called instead of "repaint()" to avoid re-painting sub-components*/
	void refresh() override;

	// /** Called when data acquisition is active.*/
    // void beginAnimation() override;

    // /** Called when data acquisition ends.*/
    // void endAnimation() override;

	/** Draws the canvas background */
	void paint(Graphics& g) override;

	void setWindowSizeMs(int windowSize_);

	void setBinSizeMs(int binSize_);

	void setMostRecentSample(int64 sampleNum);

	void setSampleRate(float sampleRate);

	/** Adds a spike time */
    void addSpike(int64 sample_number);

private:

	/** Recounts spikes/bin */
    void recount();

	/** Recomputes bin edges */
    void recompute();

	void updatePlotRange();

	/** Pointer to the processor class */
	RateViewer* processor;

	std::unique_ptr<Viewport> viewport;

	/** Class for plotting data */
	InteractivePlot plt;

	float sampleRate;

	int windowSize, binSize;
	int64 mostRecentSample;

	Array<double> binEdges;
	Array<int> binEdgesInSamples;

	Array<double> relativeTimes;

	Array<int64> incomingSpikeSampleNums;

	int lastValidIndex;

	Array<int> counts;
	int maxCount;
	

	/** Generates an assertion if this class leaks */
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RateViewerCanvas);
};


#endif // RATEVIEWERCANVAS_H_DEFINED