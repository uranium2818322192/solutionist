#pragma once
#include "string.h"

namespace File
{
    extern String runningDirectory;
    extern String currentDirectory;
    extern String hintFileName;

    struct Instance
    {
        char* data = nullptr;
        size_t size = 0;
    };

    void init();
    void cleanUp();
    void appendToCurrentDirectory(String string, bool isSubdirectory);
    File::Instance* openAndRead(String fileNameWithRelativePath);
    HANDLE openFileForWrite(const char* fileNameWithRelativePath);
}