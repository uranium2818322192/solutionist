#pragma once
#include "guid.h"
#include "string.h"
#include "dynamicStorage.h"

namespace Project
{
    struct PerConfiguration
    {
        DynamicStorage::Instance<String> sourceFiles;
        DynamicStorage::Instance<String> includeFiles;
        DynamicStorage::Instance<String> defines;
        DynamicStorage::Instance<String> linkedLibraries;
    };

    struct Instance
    {
        String name;
        Guid::Instance guid;
        PerConfiguration* perConfiguration;
    };
}