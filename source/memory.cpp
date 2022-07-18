#include "memory.h"

namespace Memory
{
    HANDLE defaultHeap;
    size_t pageSize;
}

void Memory::init()
{
    defaultHeap = GetProcessHeap();

    SYSTEM_INFO systemInformation;
    GetSystemInfo(&systemInformation);
    pageSize = systemInformation.dwPageSize;
}