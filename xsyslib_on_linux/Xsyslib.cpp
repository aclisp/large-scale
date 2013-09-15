#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <iostream>
#include "Xsyslib.h"
#include "Trace.h"


using namespace std;


const unsigned int Delimiter = 0xDD000000;
const unsigned int SizeMask  = 0x00FFFFFF;
const size_t       MaxCapacity = 2 * 1024 * 1024; // 2MB
const size_t       MaxMemorySize = 2 * 1024 * 1024; // 1.5MB

static SharedMemory *PSharedMemory = NULL;


void *Alloc(size_t size)
{
    size_t newAllocatedSize = PSharedMemory->AllocatedSize +
                              sizeof(Delimiter) +
                              size;
    if(newAllocatedSize > PSharedMemory->MaxMemorySize || size > SizeMask)
        return NULL;
    char *pCurrent = PSharedMemory->pData +
                     PSharedMemory->AllocatedSize;
    PSharedMemory->AllocatedSize = newAllocatedSize;
    *((unsigned int *)pCurrent) = (Delimiter | (size & SizeMask));
    return (pCurrent + sizeof(Delimiter));
}

SharedMemory *MemoryPtr(void)
{
    return PSharedMemory;
}

// format control constants
//
// the following constants have default value, but can be overrided by
// compiler command line or previous definitions.

/// address field text width
#ifndef DUMPBIN_ADDRESS_WIDTH
#define DUMPBIN_ADDRESS_WIDTH   5       // 4 characters and 1 space, e.g. '0000 '
#endif

/// hex text width per byte
#ifndef DUMPBIN_HEX_WIDTH_PER_BYTE
#define DUMPBIN_HEX_WIDTH_PER_BYTE  3   // 2 characters and 1 space, e.g. 'F0 '
#endif

/// byte dumped per line
#ifndef DUMPBIN_BYTE_PER_LINE
#define DUMPBIN_BYTE_PER_LINE   16
#endif

//////////////////////////////////////////////////////////////////////////
// default output effect
//------------------------------------------------------------------------
// addr hex                    divider                  char
//------------------------------------------------------------------------
// 0000 E9 06 00 00 00 CC CC CC-CC CC CC 55 8B EC 83 EC ...........U....
// 0010 40 53 56 57 8D 7D C0 B9-10 00 00 00 B8 CC CC CC @SVW.}..........
// 0020 CC F3 AB 6A 00 6A 64 68-05 10 40 00 E8 2A 00 00 ...j.jdh..@..*..
// 0030 00 83 C4 0C 33 C0 5F 5E-5B 83 C4 40 3B EC E8 68 ....3._^[..@;..h
// 0040 02 00 00 8B E5 5D C3 CC-CC CC CC CC CC CC CC CC .....]..........
// 0050 CC CC CC CC CC CC CC CC-CC CC CC 55 8B EC 81 EC ...........U....
// 0060 A8 00 00 00                                     ....
//////////////////////////////////////////////////////////////////////////

void DebugDumpBinary(
                     const void* p,
                     size_t      Size,
                     const void* StartingAddress
                     )
{
    static const char* HexAlphabet = "0123456789ABCDEF";
    const unsigned char* Data = (const unsigned char*)p;
    size_t i;

    if(StartingAddress)
    {
        printf("Starting address: %p\n", StartingAddress);
    }

    for(i=0; i<Size; i+=DUMPBIN_BYTE_PER_LINE)
    {
        int j;
        unsigned int a = (unsigned int)StartingAddress+i;

        // DebugView display each DbgPrint output in one line,
        // so I gather them in one line
        char Line[
            DUMPBIN_ADDRESS_WIDTH+  // Address width
                DUMPBIN_BYTE_PER_LINE*DUMPBIN_HEX_WIDTH_PER_BYTE+ // hex
                DUMPBIN_BYTE_PER_LINE*1 // character
                +1  // tail NULL
        ];

        // print the low 2 byte of address
        Line[0] = HexAlphabet[(a>>12) & 0x0F];
        Line[1] = HexAlphabet[(a>>8) & 0x0F];
        Line[2] = HexAlphabet[(a>>4) & 0x0F];
        Line[3] = HexAlphabet[(a) & 0x0F];
        Line[4] = ' ';

        for(j=0; j<DUMPBIN_BYTE_PER_LINE; ++j)
        {
            size_t HexPos =
                DUMPBIN_ADDRESS_WIDTH + j*DUMPBIN_HEX_WIDTH_PER_BYTE;
            size_t CharPos =
                DUMPBIN_ADDRESS_WIDTH +
                DUMPBIN_BYTE_PER_LINE*DUMPBIN_HEX_WIDTH_PER_BYTE+
                j;

            if(i+j < Size)
            {
                unsigned char c = Data[i+j];

                Line[HexPos+0] = HexAlphabet[c>>4];
                Line[HexPos+1] = HexAlphabet[c&0x0F];
                Line[HexPos+2] = ' ';
                Line[CharPos] = isprint(c) ? c : '.';

                if(j==8)
                {
                    Line[HexPos-1] = '-';
                }
            }
            else
            {
                Line[HexPos+0] = ' ';
                Line[HexPos+1] = ' ';
                Line[HexPos+2] = ' ';
                Line[CharPos] = ' ';
            }
        }

        Line[sizeof(Line)-1] = 0;   // ensure in case

        printf("%s\n", Line);
    }
}

unsigned int GetTickCount(void)
{
    timespec tp;

    if(clock_gettime(CLOCK_MONOTONIC, &tp) < 0)
        return 0;

    return tp.tv_sec * 1000 + tp.tv_nsec / 1000000.0;
}


void InitializeSharedMemory(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    int       shm_id;
    key_t     mem_key;
    void      *shm_ptr;

    mem_key = ftok(PathName, MemoryId);
    if (mem_key < 0) 
        trace.ErrorQuit(errno, 1, "ftok");
    

    shm_id = shmget(mem_key, MaxCapacity, IPC_CREAT | 0666);
    if (shm_id < 0) 
        trace.ErrorQuit(errno, 1, "shmget");

    shm_ptr = shmat(shm_id, NULL, 0);  /* attach */
    if (shm_ptr == (void *)-1) 
        trace.ErrorQuit(errno, 1, "shmat");

    PSharedMemory = (SharedMemory *)shm_ptr;
    memset(PSharedMemory, 0, MaxCapacity);
    PSharedMemory->MaxMemorySize = MaxMemorySize;
}

void DestorySharedMemory(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    int       shm_id;
    key_t     mem_key;
    void      *shm_ptr;

    mem_key = ftok(PathName, MemoryId);
    if (mem_key < 0) 
        trace.ErrorSys(errno, "ftok");
       
    shm_id = shmget(mem_key, MaxCapacity, 0666);
    if (shm_id < 0)
        trace.ErrorSys(errno, "shmget");

    shm_ptr = PSharedMemory;

    if(shmdt(shm_ptr) < 0)
        trace.ErrorSys(errno, "shmdt");

    if(shmctl(shm_id, IPC_RMID, NULL) < 0)
        trace.ErrorSys(errno, "shmctl");
}

void DisplaySharedMemory(void)
{
    cout << "SharedMemory from " << MemoryPtr() << ":" << endl;
    cout << "  MaxMemorySize : " << MemoryPtr()->MaxMemorySize << endl;
    cout << "  AllocatedSize : " << MemoryPtr()->AllocatedSize << endl;
}
