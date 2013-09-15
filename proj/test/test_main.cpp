#include "radutil.h"
#include "radtest.h"
#if defined(RAD_OS_WIN32)
#include <winsock2.h>

static void socket_init()
{
    // Initialise the socket subsystem
    // This is only provided for compatibility with Windows.

    // Initialise WinPoxySockets on Win32
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    if ((err = WSAStartup(wVersionRequested, &wsaData)) != 0)
    {
        radlog(L_FATAL|L_CONS, "WSAStartup occurs error(%d)", err);
    }

    /* Confirm that the WinSock DLL supports 2.2.        */
    /* Note that if the DLL supports versions later      */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
         HIBYTE( wsaData.wVersion ) != 2 ) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        WSACleanup();
        radlog(L_FATAL|L_CONS, "Can not acquire winsock 2.2");
    }

    // If successful, or if not required, then continue!
}

static void socket_clean()
{
    if(WSACleanup() == SOCKET_ERROR) {
        radlog(L_WARN|L_CONS, "WSACleanup occurs error(%d)", WSAGetLastError());
    }
}

#else /* !RAD_OS_WIN32 */
static void socket_init() {}
static void socket_clean() {}
#endif /* !RAD_OS_WIN32 */


int main(int argc, char* argv[])
{
    socket_init();
    rad_start_heap_tracking();
    rad_run_test();
    rad_stop_heap_tracking();
    socket_clean();
    return 0;
}
