#include "LeakyBucketCounter.h"
#include "Thread.h"

LeakyBucketCounter::LeakyBucketCounter(unsigned short events, 
                                       unsigned short seconds)
{
    fullSize_  = events;
    fillTime_  = seconds * 1000;
    counter_   = events;
    startTime_ = GetTickCount();
}

bool LeakyBucketCounter::ThresholdExceeded(void)
{
    unsigned int currTime = GetTickCount();
    if(counter_ == fullSize_)
        startTime_ = currTime; // bucket starting to drain now
    else if((currTime - startTime_) >= fillTime_)
    {
        // It's time to refill the bucket.
        counter_ = fullSize_;
        startTime_ = currTime;
    }
    counter_--;
    if(counter_ == 0)
    {
        // Refill the empty bucket.
        counter_ = fullSize_;
        startTime_ = currTime;
        return true;
    }
    return false;
}
