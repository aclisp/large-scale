// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/25 13:59:49
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_CRON_H__
#define __RADFWK_CRON_H__

#include <radfwk/RAD_Thread.h>

class RAD_CronTask;

/* @ref http://minnie.tuhs.org/UnixTree/V7/usr/src/cmd/cron.c.html
 *
 * Cron is a topmost thread object that should be coded carefully
 * to be never crashed. Thread watchdog and other services is cron
 * tasks running on the global cron. The process main thread is
 * usually used as an OA&M console which understood command line.
 */
class RADFWK_API RAD_Cron : public RAD_Thread
{
public:
    RAD_Cron();
    virtual ~RAD_Cron();

    void Start();
    void Stop(unsigned int delay);
    unsigned int ActivityRemain() const;
    void KillAllTaskImmediately();
    void ClearTaskList();
    void AddTask(RAD_CronTask *);

    list < RAD_CronTask * > m_TaskList;
    mutable RAD_Mutex m_TaskListLock;

    static RAD_Cron& Global();
    static RAD_Cron * m_pGlobalCron;

private:
    virtual int MainLoop();
    virtual int StartThreadHook();
    virtual void KillThreadHook();

    void StartCronTask();
    void CheckCronTask();
    void Rest();
    void SetExactMinuSec();
    const char* Match(const char* p, int v);

    time_t m_ExactMinuSec;
    int m_iFlag;
};

/* Cron task is a controllable, time-limited thread object scheduled
 * by cron. They should NOT be watched by thread watchdog because
 * cron takes care of them.
 */
class RADFWK_API RAD_CronTask : public RAD_Thread
{
public:
    explicit RAD_CronTask(unsigned int act = 45);
    virtual ~RAD_CronTask();

    enum MatchCode {
        EXACT = '@' ,
        ANY   = '*' ,
        LIST  = ',' ,
        RANGE = '-' ,
    };

    bool Schedule(const string& sched);
    time_t m_ActivationTime;
    unsigned int m_nTotalActivity; // zero means always alive
    vector < char > m_ParsedSched;
};

#endif // __RADFWK_CRON_H__
