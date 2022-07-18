#pragma once
#include "guid.h"
#include "project.h"
#include "string.h"
#include "dynamicStorage.h"

namespace Solution
{
    constexpr const char arch[] = "x64";
    extern String name;
    extern Guid::Instance guid;
    extern DynamicStorage::Instance<String> configurations;
    extern DynamicStorage::Instance<Project::Instance*> projects;

    void init();
    void cleanUp();
}