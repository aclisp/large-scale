#ifndef __XSYSLIB_LEAKYBUCKETCOUNTER_H__
#define __XSYSLIB_LEAKYBUCKETCOUNTER_H__

#include "Object.h"

class LeakyBucketCounter : public Object
{
public:
    LeakyBucketCounter(unsigned short events,
                       unsigned short seconds);
    virtual ~LeakyBucketCounter(void) { };
    bool ThresholdExceeded(void);

private:
    unsigned short fullSize_; // number of credits in full bucket
    unsigned int   fillTime_; // time required to refill bucket
    unsigned short counter_;  // current number of credits in bucket
    unsigned int   startTime_;// time when bucket started to drain
};

#endif // __XSYSLIB_LEAKYBUCKETCOUNTER_H__
