#include "action.h"
#include "console.h"
#include "file.h"
#include "parser.h"
#include "memory.h"
#include "solution.h"

unsigned __int64 characterValidationMask[2] = { 0x23FF388200000000, 0x6FFFFFFFEFFFFFFF };
#define IsCharValid(char) characterValidationMask[char < 64 ? 0 : 1] & (1ULL << (char % 64))

bool kind(String parameter)
{
    bool didParsedWithoutError = true;
    PRINTOUT("kind : ");
    Console::print(parameter);
    PRINTOUT("\n");
    return didParsedWithoutError;
}

bool config(String parameter)
{
    if(Solution::projects.size != 0)
    {
        PRINTOUT("All configs has to be defined before any project declaration.\n");
        return false;
    }

    String configuration;
    bool isInElementScope = false;

    for(size_t i = 0; i < parameter.length; i++)
    {
        char currentChar = parameter.buffer[i];
        bool isLastChar = i == parameter.length - 1;

        if((currentChar == ',') || isLastChar)
        {
            if(Solution::configurations += configuration)
            {
                isInElementScope = false;
                configuration.length = 0;
            }
            else
            {
                PRINTOUT("Configuration limit exceeded.\n");
                return false;
            }
        }
        else if(IsCharValid(currentChar))
        {
            if(!isInElementScope)
            {
                if(configuration.length != 0)
                {
                    PRINTOUT("Unexpected character encounter.\n");
                    return false;
                }

                isInElementScope = true;
                configuration.buffer = parameter.buffer + i;
            }

            configuration.length++;
        }
        else
        {
            isInElementScope = false;
        }
    }

    return true;
}

bool files(String parameter)
{
    bool didParsedWithoutError = true;
    PRINTOUT("files : ");
    Console::print(parameter);
    PRINTOUT("\n");
    return didParsedWithoutError;
}

bool link(String parameter)
{
    bool didParsedWithoutError = true;
    PRINTOUT("link : ");
    Console::print(parameter);
    PRINTOUT("\n");
    return didParsedWithoutError;
}

bool cfgscope(String parameter)
{
    bool didParsedWithoutError = true;
    PRINTOUT("cfgscope : ");
    Console::print(parameter);
    PRINTOUT("\n");
    return didParsedWithoutError;
}

bool include(String parameter)
{
    size_t fallbackPreviousDirectoryLength = File::currentDirectory.length;
    File::appendToCurrentDirectory(parameter, true);

    size_t fallbackDirectoryLength = File::currentDirectory.length;
    File::appendToCurrentDirectory(File::hintFileName, false);

    File::Instance* hintFile = File::openAndRead(File::currentDirectory);
    File::currentDirectory.length = fallbackDirectoryLength;

    if(!hintFile)
    {
        PRINTOUT("There is no hint file at the specified directory: ");
        Console::print(File::currentDirectory);
        PRINTOUT("\n");
        return false;
    }

    if(Parser::fileHandles += hintFile)
    {
        Parser::operate(hintFile);
    }
    else
    {
        PRINTOUT("File include limit exceeded.\n");
        return false;
    }

    File::currentDirectory.length = fallbackPreviousDirectoryLength;

    return true;
}

bool startup(String parameter)
{
    bool didParsedWithoutError = true;
    PRINTOUT("startup : ");
    Console::print(parameter);
    PRINTOUT("\n");
    return didParsedWithoutError;
}

bool project(String parameter)
{
    Project::Instance* project = Memory::allocate<Project::Instance>();
    project->perConfiguration = Memory::allocate<Project::PerConfiguration>(Solution::configurations.size);

    bool isInElementScope = false;
    for(size_t i = 0; i < parameter.length; i++)
    {
        char currentChar = parameter.buffer[i];

        if(IsCharValid(currentChar))
        {
            if(!isInElementScope)
            {
                if(project->name.length != 0)
                {
                    PRINTOUT("Unexpected character encounter.\n");
                    return false;
                }

                isInElementScope = true;
                project->name.buffer = parameter.buffer + i;
            }

            project->name.length++;
        }
        else
        {
            isInElementScope = false;
        }
    }

    if(Solution::projects += project)
    {
        return true;
    }
    else
    {
        PRINTOUT("Project limit exceeded.\n");
        return false;
    }
}

bool solution(String parameter)
{
    if(Solution::name.length != 0)
    {
        PRINTOUT("There can't be multiple solutions.\n");
        return false;
    }

    bool isInElementScope = false;
    for(size_t i = 0; i < parameter.length; i++)
    {
        char currentChar = parameter.buffer[i];

        if(IsCharValid(currentChar))
        {
            if(!isInElementScope)
            {
                if(Solution::name.length != 0)
                {
                    PRINTOUT("Unexpected character encounter.\n");
                    return false;
                }

                isInElementScope = true;
                Solution::name.buffer = parameter.buffer + i;
            }

            Solution::name.length++;
        }
        else
        {
            isInElementScope = false;
        }
    }

    return true;
}

bool depends(String parameter)
{
    bool didParsedWithoutError = true;
    PRINTOUT("depends : ");
    Console::print(parameter);
    PRINTOUT("\n");
    return didParsedWithoutError;
}

bool define(String parameter)
{
    bool didParsedWithoutError = true;
    PRINTOUT("define : ");
    Console::print(parameter);
    PRINTOUT("\n");
    return didParsedWithoutError;
}

bool null(String parameter)
{
    PRINTOUT("Unknown keyword.\n");
    return false;
}