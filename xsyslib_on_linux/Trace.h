#ifndef __XSYSLIB_TRACE_H__
#define __XSYSLIB_TRACE_H__

#include <string>

class LogSource
{
public:
    LogSource(const char *file, int line)
        : file_(file), line_(line)
    { }
    const char *File() const
    { return file_; }
    int Line() const
    { return line_; }

private:
    const char *file_;
    int line_;
};

class Trace
{
public:
    Trace(const char *func);
    ~Trace();

    Trace& Parm(const char *name, int value);
    Trace& Parm(const char *name, bool value);
    Trace& Parm(const char *name, const char *value);
    Trace& Parm(const char *name, const std::string& value);
    Trace& Parm(const char *name, double value);
    Trace& Parm(const char *name, const void *value);

    void EnterFunc();
    void DebugOutput(const char *msg, ...);
    void ErrorSys(int errnum, const char *msg, ...);
    void ErrorQuit(int errnum, int status, const char *msg, ...);
    
    static void DebugOutput_s(const char *msg, ...);
    static void ErrorSys_s(int errnum, const char *msg, ...);
    static void ErrorQuit_s(int errnum, int status, const char *msg, ...);

    static bool IsTraceActive;
    static int NestedLevel;

private:
    std::string Context(const char *preamble) const;

    static std::string Context_s(const char *preamble);

    static const int IndentWidth = 2;
    
    const char *function_;
    std::ostringstream *parameter_;
};

#endif // __XSYSLIB_TRACE_H__
