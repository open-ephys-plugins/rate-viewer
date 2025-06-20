/*
	------------------------------------------------------------------

	This file is part of a plugin for the Open Ephys GUI
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

#ifndef RATEVIEWERCANVAS_H_DEFINED
#define RATEVIEWERCANVAS_H_DEFINED

#include <VisualizerWindowHeaders.h>

class RateViewer;

/**
* 
	Draws spike rate for one electrode

*/
class RateViewerCanvas : public Visualizer
{
public:
    /** Constructor */
    RateViewerCanvas (RateViewer* processor);

    /** Destructor */
    ~RateViewerCanvas();

    /** Updates boundaries of sub-components whenever the canvas size changes */
    void resized() override;

    /** Called when the visualizer's tab becomes visible again -- not actually used */
    void refreshState() override;

    /** Updates settings */
    void updateSettings() override;

    /** Called instead of "repaint()" to avoid re-painting sub-components*/
    void refresh() override;

    /** Draws the canvas background */
    void paint (Graphics& g) override;

    /** Sets the window size for the plot*/
    void setWindowSizeMs (int windowSize_);

    /** Sets the bin size for the plot*/
    void setBinSizeMs (int binSize_);

    /** Counts the total number of samples received */
    void incrementSampleCount (int64 sampleCount);

    /** Adds a spike sample number */
    void addSpike (int64 sample_number);

    /** Sets the sample rate for the selected electrode*/
    void setSampleRate (float sampleRate);

    /** Sets the title as the name of the selected electrode */
    void setPlotTitle (const String& title);

private:
    /** Recounts spikes/bin; returns true if a new bin is available */
    bool countSpikes();

    /** Recomputes bin edges */
    void recomputeBinEdges();

    /** Adjusts plot range so all points are visible*/
    void updatePlotRange();

    /** Pointer to the processor class */
    RateViewer* processor;

    /** Class for plotting data */
    std::unique_ptr<InteractivePlot> plt;

    float sampleRate = 0.0f;

    int windowSize = 1000;
    int binSize = 50;

    int64 mostRecentSample = 0;
    int64 sampleOnLastRedraw = 0;
    int maxCount = 1;

    Array<double> binEdges;
    Array<int> spikeCounts;

    Array<int64> incomingSpikeSampleNums;

    /** Generates an assertion if this class leaks */
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RateViewerCanvas);
};

#endif // RATEVIEWERCANVAS_H_DEFINED
