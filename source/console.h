#pragma once
#include "string.h"

namespace Console
{
    extern HANDLE stdOutputHandle;

    void init();
    void print(String string);
    void print(size_t number);
}

#define PRINTOUT(str)   {                                                       \
                            WriteConsole(Console::stdOutputHandle,              \
                                         str,                                   \
                                         sizeof(str) - 1,                       \
                                         nullptr,                               \
                                         nullptr);                              \
                        }