/*
  ==============================================================================

    WorkerThread.h
    Created: 22 May 2021 3:53:14pm
    Author:  jonny

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WorkerThread : public juce::Thread
{
public:
    WorkerThread();
    ~WorkerThread();

    void run() override;
private:
    std::vector<int> counters_;
};