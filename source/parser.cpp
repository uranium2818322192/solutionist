#include "parser.h"
#include "memory.h"
#include "console.h"
#include "keyword.h"

// TODO: if parse error occurs in parameters, line position will be misleading

#define ParseError(lineIndex, caretIndex) PRINTOUT("Parsing error occoured at the directory: "); \
                                          Console::print(File::currentDirectory);                \
                                          PRINTOUT(", line: ");                                  \
                                          Console::print(lineIndex);                             \
                                          PRINTOUT(", caret: ");                                 \
                                          Console::print(caretIndex);                            \
                                          PRINTOUT("\n");                                        \
                                          __fastfail(1);

namespace Parser
{
    DynamicStorage::Instance<File::Instance*> fileHandles;
}

void Parser::init()
{
    DynamicStorage::allocate(&fileHandles);

    File::Instance* rootFile = File::openAndRead(File::hintFileName);

    if(!rootFile)
    {
        PRINTOUT("There is no hint file for solution creation at the running directory: ");
        Console::print(File::runningDirectory);
        __fastfail(1);
    }

    fileHandles += rootFile;
    operate(rootFile);
}

void Parser::cleanUp()
{
    for(size_t i = 0; i < fileHandles.size; i++)
    {
        Memory::deallocate(fileHandles[i]->data);
        Memory::deallocate(fileHandles[i]);
    }

    DynamicStorage::deallocate(&fileHandles);
}

void Parser::operate(File::Instance* file)
{
    bool isInKeywordScope = false;
    bool isInConfigScope = false;
    bool isInCommentScope = false;

    size_t lineIndex = 1;
    size_t caretIndex = 1;

    String parameter = {};
    String keyword = { Memory::allocate<char>(8), 0 };

    __int64 hashedKeyword = 0;

    for(size_t i = 0; i < file->size; i++)
    {
        char currentChar = *(file->data + i);

        if(currentChar == '\n')
        {
            lineIndex++;
            caretIndex = 1;
            isInCommentScope = false;
        }
        else
        {
            caretIndex++;
        }

        if(isInKeywordScope)
        {
            if(currentChar == ')')
            {
                bool didParsedWithoutError = Keyword::callFunctionForHashedKeyword(hashedKeyword, parameter);
                if(!didParsedWithoutError)
                {
                    ParseError(lineIndex, caretIndex)
                }
                parameter.length = 0;
                isInKeywordScope = false;
            }
            else
            {
                parameter.length++;
            }
        }
        else
        {
            if(isInCommentScope)
            {
                continue;
            }
            else if(currentChar == '(')
            {
                if(keyword.length != 0)
                {
                    if(keyword.length > 8)
                    {
                        PRINTOUT("Keywords cannot be longer than 8 characters\n");
                        ParseError(lineIndex, caretIndex);
                    }

                    hashedKeyword = Keyword::stringToHashedKeyword(keyword);
                    keyword.length = 0;
                    isInKeywordScope = true;
                    parameter.buffer = file->data + i + 1;

                    if(hashedKeyword == 7309464668150916707) // Check for config scope keyword
                    {
                        isInConfigScope = true;
                    }
                }
            }
            else if(currentChar == ')')
            {
                if(isInConfigScope)
                {
                    isInConfigScope = false; // TODO: reset the config scope
                }
                else
                {
                    ParseError(lineIndex, caretIndex)
                }
            }
            else if(currentChar == '#')
            {
                isInCommentScope = true;
            }
            else if((currentChar != ' ') && (currentChar != '\t') && (currentChar != '\r') && (currentChar != '\n'))
            {
                keyword.buffer[keyword.length] = currentChar;
                keyword.length++;
            }
        }
    }

    Memory::deallocate(keyword.buffer);
}