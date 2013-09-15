#include "Trace.h"
#include "Thread.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
#include <sstream>
#include <errno.h>

using namespace std;


bool Trace::IsTraceActive = true;
int  Trace::NestedLevel = 0;


std::string Trace::Context(const char *preamble) const
{
    char pid[24];
    snprintf(pid, sizeof(pid), "%5u.%5u", GetTickCount(), getpid());
    pid[sizeof(pid)-1] = '\0';
    string s = "> ";
    s.insert(0, pid);
    
    if(IsTraceActive)
        s.insert(s.end(), (NestedLevel - 1) * IndentWidth, ' ');
    
    s.append(preamble);
    s.append(" ");
    s.append(function_);
    return s;
}

std::string Trace::Context_s(const char *preamble)
{
    char pid[24];
    snprintf(pid, sizeof(pid), "%5u.%5u", GetTickCount(), getpid());
    pid[sizeof(pid)-1] = '\0';
    string s = "> ";
    s.insert(0, pid);
    
    if(IsTraceActive)
        s.insert(s.end(), (NestedLevel - 1) * IndentWidth, ' ');
    
    s.append(preamble);
    return s;
}

void Trace::EnterFunc()
{
    if(IsTraceActive)
    {
        string paramStr = parameter_->str();
        cout << Context("Enter");
        if(paramStr.size() == 0)
            cout << endl;
        else
            cout << " " << paramStr << endl;
    }
}

Trace::Trace(const char *func)
: function_(func), parameter_(NULL)
{
    if(IsTraceActive)
    {
        parameter_ = new ostringstream;
        NestedLevel ++;
    }
}

Trace::~Trace()
{
    if(IsTraceActive)
    {
        cout << Context("Leave") << endl;

        NestedLevel --;
        delete parameter_;
    }
}

Trace& Trace::Parm(const char *name, int value)
{
    if(IsTraceActive)
    {
        *parameter_ << "(" << name << ":" << value << ") ";
    }
    return *this;
}

Trace& Trace::Parm(const char *name, bool value)
{
    if(IsTraceActive)
    {
        *parameter_ << "(" << name << ":" << value << ") ";
    }
    return *this;
}

Trace& Trace::Parm(const char *name, const char *value)
{
    if(IsTraceActive)
    {
        *parameter_ << "(" << name << ":" << value << ") ";
    }
    return *this;
}

Trace& Trace::Parm(const char *name, const std::string& value)
{
    if(IsTraceActive)
    {
        *parameter_ << "(" << name << ":" << value << ") ";
    }
    return *this;
}

Trace& Trace::Parm(const char *name, double value)
{
    if(IsTraceActive)
    {
        *parameter_ << "(" << name << ":" << value << ") ";
    }
    return *this;
}

Trace& Trace::Parm(const char *name, const void *value)
{
    if(IsTraceActive)
    {
        *parameter_ << "(" << name << ":" << value << ") ";
    }
    return *this;
}

void Trace::DebugOutput(const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    fprintf(stdout, "%s ", Context("DEBUG").c_str());
    vfprintf(stdout, msg, ap);
    fprintf(stdout, "\n");
    va_end(ap);
}

void Trace::ErrorSys(int errnum, const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    fprintf(stderr, "%s\n", Context("WARN ").c_str());
    fprintf(stderr, " *** ");
    vfprintf(stderr, msg, ap);
    if(errnum == 0)
        fprintf(stderr, "\n");
    else
        fprintf(stderr, " %s\n", strerror(errnum));
    va_end(ap);
}

void Trace::ErrorQuit(int errnum, int status, const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    fprintf(stderr, "%s\n", Context("ERROR").c_str());
    fprintf(stderr, " *** ");
    vfprintf(stderr, msg, ap);
    if(errnum == 0)
        fprintf(stderr, "\n");
    else
        fprintf(stderr, " %s\n", strerror(errnum));
    va_end(ap);

    exit(status);
}

void Trace::DebugOutput_s(const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    fprintf(stdout, "%s ", Context_s("DEBUG").c_str());
    vfprintf(stdout, msg, ap);
    fprintf(stdout, "\n");
    va_end(ap);
}

void Trace::ErrorSys_s(int errnum, const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    fprintf(stderr, "%s\n", Context_s("WARN ").c_str());
    fprintf(stderr, " *** ");
    vfprintf(stderr, msg, ap);
    if(errnum == 0)
        fprintf(stderr, "\n");
    else
        fprintf(stderr, " %s\n", strerror(errnum));
    va_end(ap);
}

void Trace::ErrorQuit_s(int errnum, int status, const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    fprintf(stderr, "%s\n", Context_s("ERROR").c_str());
    fprintf(stderr, " *** ");
    vfprintf(stderr, msg, ap);
    if(errnum == 0)
        fprintf(stderr, "\n");
    else
        fprintf(stderr, " %s\n", strerror(errnum));
    va_end(ap);

    exit(status);
}
