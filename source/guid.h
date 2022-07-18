#pragma once

namespace Guid
{
    typedef char Instance[37];

    void init();
    void generate(char* outBuffer);
}