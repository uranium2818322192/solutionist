#pragma once
#include "memory.h"

#define ConstantString(string) string, sizeof(string)-1

struct String
{
    String() = default;
    String(char* buffer, size_t length) : buffer(buffer), length(length) {};
    String(const char* constantString, size_t initialLength) : length(initialLength)
    {
        buffer = Memory::allocate<char>(initialLength);

        for(size_t i = 0; i < initialLength; i++)
        {
            buffer[i] = constantString[i];
        }
    }

    char* buffer = nullptr;
    size_t length = 0;
};

inline bool operator==(const String& lhs, const String& rhs)
{
    if(lhs.length != rhs.length)
    {
        return false;
    }

    for(size_t i = 0; i < lhs.length; i++)
    {
        if(lhs.buffer[i] != rhs.buffer[i])
        {
            return false;
        }
    }

    return true;
}