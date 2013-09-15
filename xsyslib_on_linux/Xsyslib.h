#ifndef __XSYSLIB_H__
#define __XSYSLIB_H__

#include <stddef.h>

struct SharedMemory
{
    size_t MaxMemorySize;
    size_t AllocatedSize;
    char   pData[0];
};

void *Alloc(size_t size);
SharedMemory *MemoryPtr(void);
void DebugDumpBinary(
                     const void* p,
                     size_t      Size,
                     const void* StartingAddress
                     );
unsigned int GetTickCount(void);

#define PathName    "/tmp"
#define MemoryId    'a'
#define SemaphoreId 'b'

void InitializeSharedMemory(void);
void DestorySharedMemory(void);
void DisplaySharedMemory(void);

#endif // __XSYSLIB_H__
