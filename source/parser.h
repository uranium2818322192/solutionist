#pragma once
#include "file.h"
#include "dynamicStorage.h"

namespace Parser
{
    extern DynamicStorage::Instance<File::Instance*> fileHandles;

    void init();
    void cleanUp();
    void operate(File::Instance* file);
}