#include "RAD_Cron.h"
#include <time.h>

RAD_Cron * RAD_Cron::m_pGlobalCron = NULL;

class StringReader {
public:
    StringReader(const string& str) : s(str.c_str()), p(0) {}
    int getchar() { return s[p++]; }
    void ungetc() { p--; }
    int number(int c);
    const char* s;
    size_t p;
};

int StringReader::number(int c)
{
    int n = 0;

    while ( isdigit(c) ) {
        n = n*10 + c - '0';
        c = getchar();
    }
    ungetc();
    if ( n >= 60 )
        return -1;
    return n;
}

static string ToPrinable(const vector<char>& sched)
{
    int col = 0;
    string ret;
    char buf[256];

    for ( vector<char>::const_iterator ci = sched.begin();
          ci != sched.end() && col < 5; ++col ) {
        switch( *ci ) {
        case RAD_CronTask::EXACT:
            ret.append(1, *ci++);
            sNprintf(buf, sizeof(buf), "%d", *ci++);
            ret.append(buf);
            continue;
        case RAD_CronTask::ANY:
            ret.append(1, *ci++);
            continue;
        case RAD_CronTask::LIST:
            ++ci;
            ret.append(1, '[');
            do {
                sNprintf(buf, sizeof(buf), "%d", *ci++);
                ret.append(buf);
                ret.append(1, ',');
            } while( *ci != RAD_CronTask::LIST );
            ++ci;
            *ret.rbegin() = ']';
            continue;
        case RAD_CronTask::RANGE:
            ++ci;
            ret.append(1, '[');
            sNprintf(buf, sizeof(buf), "%d", *ci++);
            ret.append(buf);
            ret.append(1, '-');
            sNprintf(buf, sizeof(buf), "%d", *ci++);
            ret.append(buf);
            ret.append(1, ']');
            continue;
        }
    }

    return ret;
}

bool RAD_CronTask::Schedule(const string& sched)
{
    StringReader r(sched);
    char *cp;
    int i, c, n;

    m_ParsedSched.resize( sched.size() + 5 );
    vector<char>(m_ParsedSched).swap(m_ParsedSched); // shrink it
    cp = &m_ParsedSched[0];

    for( i=0 ; i<5 ; ++i )
    {
        do
            c = r.getchar();
        while(c == ' ' || c == '\t'); // get first non-space char of
                                      // one field
        if(c == '\0')
            return false; // fail if nothing got from this field

        if(c == '*') {
            *cp++ = ANY;
            continue;
        }                 // field got

        if ((n = r.number(c)) < 0)
            return false;

        c = r.getchar();
        if(c == ',')
            goto mlist;
        if(c == '-')
            goto mrange;
        if(c != '\t' && c != ' ' && c != '\0')
            return false; // fields are separated by one of these

        *cp++ = EXACT;
        *cp++ = n;
        if(c == '\0' && i != 4)
            return false;
        continue;

    mlist:
        *cp++ = LIST;
        *cp++ = n;
        do {
            if ((n = r.number(r.getchar())) < 0)
                return false;
            *cp++ = n;
            c = r.getchar();
        } while (c == ',');
        if(c != '\t' && c != ' ' && c != '\0')
            return false;
        *cp++ = LIST;
        if(c == '\0' && i != 4)
            return false;
        continue;

    mrange:
        *cp++ = RANGE;
        *cp++ = n;
        if ((n = r.number(r.getchar())) < 0)
            return false;
        c = r.getchar();
        if(c != '\t' && c != ' ' && c != '\0')
            return false;
        *cp++ = n;
        if(c == '\0' && i != 4)
            return false;
        continue;
    }

    DEBUG("[RAD_CronTask] Schedule '%s' at '%s'",
          sched.c_str(), ToPrinable(m_ParsedSched).c_str());
    return true;
}

RAD_CronTask::RAD_CronTask(unsigned int act)
    : RAD_Thread("RAD_CronTask")
    , m_ActivationTime(0)
    , m_nTotalActivity(act)
{
    m_ThreadName = "RAD_CronTask";
    m_bShouldWatch = false;
}

RAD_CronTask::~RAD_CronTask()
{
}

RAD_Cron::RAD_Cron()
{
    m_ThreadName = "RAD_Cron";
}

RAD_Cron::~RAD_Cron()
{
    ClearTaskList();
}

unsigned int RAD_Cron::ActivityRemain() const
{
    if(!rad_lock(m_TaskListLock))
        return 0;

    int ar = 0;
    time_t now = time ( 0 );

    for( list < RAD_CronTask * > ::const_iterator ti = m_TaskList.begin();
         ti != m_TaskList.end(); ++ ti ) {
        const RAD_CronTask * task = *ti;
        if ( task->m_Alive == ASAlive &&
             task->m_ActivationTime > 0 &&
             task->m_nTotalActivity != 0 ) {
            int taskRemain = task->m_nTotalActivity -
                (now - task->m_ActivationTime);
            rad_assert(taskRemain >= 0);
            if( ar < taskRemain )
                ar = taskRemain;
        }
    }

    rad_unlock(m_TaskListLock);
    return ar;
}

RAD_Cron& RAD_Cron::Global()
{
    rad_assert(m_pGlobalCron != NULL);
    return *m_pGlobalCron;
}

int RAD_Cron::MainLoop()
{
    for( ; !IsStopped(); Delay(1000)) {
        Tick();
        time_t now = time( 0 );
        if( now - m_ExactMinuSec >= 60 ) {
             SetExactMinuSec();
             StartCronTask();
        }
        CheckCronTask();
    }
    return 0;
}

int RAD_Cron::StartThreadHook()
{
    return 0;
}

void RAD_Cron::KillThreadHook()
{
    KillAllTaskImmediately();
}

void RAD_Cron::StartCronTask()
{
    struct tm loct;
    localtime_r(&m_ExactMinuSec, &loct);
    loct.tm_mon++; // 1-12 for month

    if(!rad_lock(m_TaskListLock))
        return;

    for( list < RAD_CronTask * > ::iterator ti = m_TaskList.begin();
         ti != m_TaskList.end(); ++ ti ) {
        RAD_CronTask * task = *ti;
        const char* cp = &task->m_ParsedSched[0];
        m_iFlag = 0;
        cp = Match(cp, loct.tm_min);
        cp = Match(cp, loct.tm_hour);
        cp = Match(cp, loct.tm_mday);
        cp = Match(cp, loct.tm_mon);
        cp = Match(cp, loct.tm_wday);
        if(m_iFlag == 0) {
            task->StartThread();
            task->m_ActivationTime = m_ExactMinuSec;
        }
    }

    rad_unlock(m_TaskListLock);
}

void RAD_Cron::CheckCronTask()
{
    if(!rad_lock(m_TaskListLock))
        return;

    time_t now = time ( 0 );

    for( list < RAD_CronTask * > ::iterator ti = m_TaskList.begin();
         ti != m_TaskList.end(); ++ ti ) {
        RAD_CronTask * task = *ti;
        if( task->m_ActivationTime > 0 && task->m_nTotalActivity != 0 ) {
            if( now - task->m_ActivationTime > (int)task->m_nTotalActivity ) {
                task->StopAndWait(0);
                task->m_ActivationTime = 0;
                radlog(L_WARN|L_CONS, "[RAD_Cron] task '%s' is killed by '%s'",
                       task->m_ThreadName.c_str(), m_ThreadName.c_str());
            }
            else if( task->m_Alive == ASStop ) {
                task->StopAndWait(0);
                task->m_ActivationTime = 0;
            }
        }
    }

    rad_unlock(m_TaskListLock);
}

void RAD_Cron::Rest()
{
    time_t t = time ( 0 );
    int i = m_ExactMinuSec - t;
    if(i > 0)
        Delay(i*1000);
}

void RAD_Cron::SetExactMinuSec()
{
    time(&m_ExactMinuSec);
    struct tm loct;
    localtime_r(&m_ExactMinuSec, &loct);
    m_ExactMinuSec -= loct.tm_sec;
}

const char* RAD_Cron::Match(const char* p, int v)
{
    const char* cp = p;

    switch(*cp++) {
    case RAD_CronTask::EXACT:
        if(*cp++ != v)
            m_iFlag++;
        return cp;
    case RAD_CronTask::ANY:
        return cp;
    case RAD_CronTask::LIST:
        while(*cp != RAD_CronTask::LIST)
            if(*cp++ == v) {
                while(*cp++ != RAD_CronTask::LIST)
                    ;
                return cp;
            }
        m_iFlag++;
        return cp+1;
    case RAD_CronTask::RANGE:
        if(*cp > v || cp[1] < v)
            m_iFlag++;
        return cp+2;
    }
    if(cp[-1] != v)
        m_iFlag++;
    return cp;
}

void RAD_Cron::ClearTaskList()
{
    KillAllTaskImmediately();

    if(!rad_lock(m_TaskListLock))
        return;

    for( list < RAD_CronTask * > ::iterator ti = m_TaskList.begin();
         ti != m_TaskList.end(); ++ ti ) {
        RAD_CronTask * task = *ti;
        delete task;
    }
    m_TaskList.clear();

    rad_unlock(m_TaskListLock);
}

void RAD_Cron::KillAllTaskImmediately()
{
    if(!rad_lock(m_TaskListLock))
        return;

    for( list < RAD_CronTask * > ::iterator ti = m_TaskList.begin();
         ti != m_TaskList.end(); ++ ti ) {
        RAD_CronTask * task = *ti;
        if( task->m_Alive == ASAlive ) {
            task->StopAndWait(0);
            task->m_ActivationTime = 0;
        }
    }

    rad_unlock(m_TaskListLock);
}

void RAD_Cron::Start()
{
    SetExactMinuSec();

    StartThread();
}

void RAD_Cron::Stop(unsigned int delay)
{
    if( delay < ActivityRemain() )
        KillAllTaskImmediately();

    StopAndWait(delay);
}

void RAD_Cron::AddTask(RAD_CronTask * task)
{
    if(!rad_lock(m_TaskListLock))
        return;

    m_TaskList.push_back(task);

    rad_unlock(m_TaskListLock);
}
