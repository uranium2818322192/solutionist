#include "solution.h"
#include "memory.h"

namespace Solution
{
    String name;
    Guid::Instance guid;
    DynamicStorage::Instance<String> configurations;
    DynamicStorage::Instance<Project::Instance*> projects;
}

void Solution::init()
{
    Guid::generate(&guid[0]);
    DynamicStorage::allocate(&configurations);
    DynamicStorage::allocate(&projects);
}

void Solution::cleanUp()
{
    DynamicStorage::deallocate(&configurations);
    DynamicStorage::deallocate(&projects);
}