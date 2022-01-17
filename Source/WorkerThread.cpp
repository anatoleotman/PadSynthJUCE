/*
  ==============================================================================

    WorkerThread.cpp
    Created: 22 May 2021 3:53:14pm
    Author:  jonny

  ==============================================================================
*/

#include "WorkerThread.h"

WorkerThread::WorkerThread(): Thread("pad_synth_worker"), counters_({0, 0})
{
    //this->startThread();
    //for (int i = 0; i < 11; ++i) {
    //    DBG("WorkerThread::WorkerThread i " << i);
    //}
    //for (int j = 0; j < 11; j++) {
    //    DBG("WorkerThread::WorkerThread j " << j);
    //}
}

WorkerThread::~WorkerThread()
{
    this->stopThread(2000);
    DBG("WorkerThread::~WorkerThread");
}

void WorkerThread::run()
{
    while (!this->threadShouldExit()) {
        this->wait(3000);
        DBG("WorkerThread::run " << this->getThreadName() << " " << ++counters_[0] << " " << counters_[1]++);
        //++counters_[0];
    }
}
