#include "file.h"
#include "memory.h"
#include "console.h"
#include "initializer.h"

CRT_SECTIONS()

#define MaxPathLimit 32767

namespace File
{
    String runningDirectory;
    String currentDirectory;
    String hintFileName(ConstantString("solutionist.hint"));

    void rangeCheckForCurrentDirectory();
}

void File::init()
{
    CRT_INIT()

    runningDirectory.length = GetCurrentDirectory(0, nullptr);
    runningDirectory.buffer = Memory::allocate<char>(runningDirectory.length);
    GetCurrentDirectory(runningDirectory.length, runningDirectory.buffer);
    runningDirectory.length--;

    currentDirectory.length = 0;
    currentDirectory.buffer = Memory::allocate<char>(MaxPathLimit);
    appendToCurrentDirectory(runningDirectory, true);
}

void File::cleanUp()
{
    Memory::deallocate(runningDirectory.buffer);
    Memory::deallocate(currentDirectory.buffer);
}

void File::rangeCheckForCurrentDirectory()
{
    if(currentDirectory.length == (MaxPathLimit - 1))
    {
        PRINTOUT("Working directory exceeds 32767 character limit :");
        Console::print(currentDirectory);
        __fastfail(1);
    }
}

void File::appendToCurrentDirectory(String string, bool isSubdirectory)
{
    for(size_t i = 0; i < string.length; i++)
    {
        rangeCheckForCurrentDirectory();

        currentDirectory.buffer[currentDirectory.length] = string.buffer[i];
        currentDirectory.length++;
    }

    if(isSubdirectory)
    {
        rangeCheckForCurrentDirectory();

        currentDirectory.buffer[currentDirectory.length] = '\\';
        currentDirectory.length++;
    }

    currentDirectory.buffer[currentDirectory.length] = '\0';
}

File::Instance* File::openAndRead(String fileNameWithRelativePath)
{
    File::Instance* fileInstance = Memory::allocate<File::Instance>();

    HANDLE handle = CreateFile(fileNameWithRelativePath.buffer,
                               FILE_READ_DATA,
                               FILE_SHARE_READ,
                               nullptr,
                               OPEN_EXISTING,
                               FILE_FLAG_SEQUENTIAL_SCAN,
                               nullptr);

    if(GetLastError() != ERROR_SUCCESS)
    {
        Memory::deallocate(fileInstance);
        return nullptr;
    }

    fileInstance->size = GetFileSize(handle, nullptr);
    fileInstance->data = Memory::allocate<char>(fileInstance->size);

    bool readResult = ReadFile(handle, fileInstance->data, fileInstance->size, nullptr, nullptr);
    CloseHandle(handle);

    if(!readResult)
    {
        Memory::deallocate(fileInstance->data);
        Memory::deallocate(fileInstance);
        return nullptr;
    }

    return fileInstance;
}

HANDLE File::openFileForWrite(const char* fileNameWithRelativePath)
{
    HANDLE filehandle = CreateFile(fileNameWithRelativePath,
                                   FILE_WRITE_DATA,
                                   FILE_SHARE_READ, //might fail because of existing visual studio instances at the time
                                   nullptr,
                                   CREATE_ALWAYS,
                                   FILE_FLAG_WRITE_THROUGH,
                                   nullptr);

    return filehandle; 
}