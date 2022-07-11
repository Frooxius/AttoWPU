#include "AttoWPUthread.h"

AttoWPUthread::AttoWPUthread(AttoWPU *wpu, float *frequency, bool *unlimited)
{
    emul = wpu;
    time = QTime();
    freq = frequency;
    maxhz = unlimited;
    StopMutex.lock();
}

void AttoWPUthread::Stop()
{
    stop = true;
    if(isRunning())
        StopMutex.lock();
}

void AttoWPUthread::run()
{
    time.start();

    int updatems,   // how many milliseconds need to pass for a screen update
        updatecycles, // how many cycles to do in each step
        cyclesdelta; // how many cycles were processed so far

    for(;;)
    {
        if(*maxhz)
        {
            updatems = 33;
        }
        else
        {
            updatems = std::max(1000.0/(*freq), 33.0);
            updatecycles = (*freq)/(1000/updatems);
            cyclesdelta = 0;
        }
        time.restart();

        do
        {
            if(stop)
            {
                stop = false;
                StopMutex.unlock();
                return;
            }
            emul->Cycle();
            cyclesdelta++;
        } while( (cyclesdelta < updatecycles || *maxhz) && time.elapsed() < updatems);

        emit UpdateUI();

        msleep(std::min(std::max(updatems-time.elapsed(), 0), 1000));
    }
}
