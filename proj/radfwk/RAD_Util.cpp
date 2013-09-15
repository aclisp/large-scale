#include "RAD_Util.h"

#if defined(RAD_OS_WIN32)
#include <windows.h>
#else
#include <errno.h>
#endif

string RAD_Util::GetLastErrDesc(const string& func)
{
    char buf[256];
    char desc[300];
#ifdef RAD_OS_WIN32
    int ec = GetLastError();
#else
    int ec = errno;
#endif
    strerror_r(ec, buf, sizeof(buf));
    sNprintf(desc, sizeof(desc), "%s: %d %s", func.c_str(), ec, buf);
    return desc;
}
