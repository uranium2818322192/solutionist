#include "console.h"

namespace Console
{
    HANDLE stdOutputHandle;
}

void Console::init()
{
    stdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Console::print(String string)
{
    WriteConsole(stdOutputHandle, string.buffer, string.length, nullptr, nullptr);
}

__declspec(safebuffers) // It's safe since size_t doesn't exceed 20 digit limit of the buffer
void Console::print(size_t number)
{
    char string[20];
    unsigned int digitIndex = 19;

    for(;;)
    {
        string[digitIndex] = '0' + (number % 10);
        number /= 10;
        if(number <= 0) { break; }
        digitIndex--;
    }

    unsigned int length = 20 - digitIndex;
    char* buffer = &string[0] + digitIndex;

    WriteConsole(stdOutputHandle, buffer, length, nullptr, nullptr);
}