#pragma once
#include "memory.h"
#include "console.h"

namespace DynamicStorage
{    
    template<typename T>
    struct Instance
    {
        T* baseAddress = nullptr;
        size_t pageLimit = 0;
        size_t currentPageCount = 0;
        size_t size = 0;

        T& operator[](size_t index)
        {
            if(index >= size)
            {
                PRINTOUT("Illegal DynamicStorage access.");
                __fastfail(1);
            }

            return *(baseAddress + index);
        }

        bool operator+=(T value)
        {
            size_t requiredSize = (size + 1) * sizeof(T);
            size_t allocatedSize = currentPageCount * Memory::pageSize;

            if(requiredSize > allocatedSize)
            {
                if(currentPageCount == pageLimit)
                {
                    return false;
                }

                VirtualAlloc(reinterpret_cast<char*>(baseAddress) + allocatedSize,
                             Memory::pageSize,
                             MEM_COMMIT,
                             PAGE_READWRITE);

                currentPageCount++;
            }

            *(baseAddress + size) = value;
            size++;

            return true;
        }
    };

    template<typename T>
    void allocate(DynamicStorage::Instance<T>* instance, size_t pageLimit = 100)
    {
        instance->baseAddress = static_cast<T*>(VirtualAlloc(nullptr,
                                                             pageLimit * Memory::pageSize,
                                                             MEM_RESERVE,
                                                             PAGE_NOACCESS));
        instance->pageLimit = pageLimit;   
    }

    template<typename T>
    void deallocate(DynamicStorage::Instance<T>* instance)
    {
        VirtualFree(instance->baseAddress, 0, MEM_RELEASE);
    }
}