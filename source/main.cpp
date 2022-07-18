#include "memory.h"
#include "console.h"
#include "file.h"
#include "guid.h"
#include "keyword.h"
#include "parser.h"
#include "solution.h"

int __stdcall wmain(int argc, wchar_t* argv[])
{
    Memory::init();
    Console::init();
    File::init();
    Guid::init();
    Keyword::init();
    Solution::init();
    Parser::init();

    File::cleanUp();
    Solution::cleanUp();
    Parser::cleanUp();
    return 0;
}