#pragma once
#include "string.h"

namespace Keyword
{
    void init();
    __int64 stringToHashedKeyword(String keyword);
    bool callFunctionForHashedKeyword(__int64 hashKeyword, String parameter);
}