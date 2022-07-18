#include "keyword.h"
#include "action.h"
#include "console.h"

template<typename ...Args>
constexpr size_t vaCount(Args&&...) { return sizeof...(Args); }

#define ConstructKeyEnum(...)   bool (*functionStack[])(String) = {__VA_ARGS__,null}; \
                                constexpr size_t keyCount = vaCount(__VA_ARGS__);     \
                                HashedKeywords<keyCount> hashedKeywords = generator<keyCount, sizeof(#__VA_ARGS__)>(#__VA_ARGS__);

namespace Keyword
{
    template<size_t size>
    struct HashedKeywords
    {
        constexpr HashedKeywords(const __int64 (&array)[size]) {
            for(size_t i = 0; i < size; i++)
            {
                data[i] = array[i];
            }
        }
        __int64 data[size] = {};
    };

    template<size_t keywordCount, size_t stringLength>
    constexpr HashedKeywords<keywordCount> generator(const char (&string)[stringLength])
    {
        __int64 keywordStorage[keywordCount] = {};
        size_t keywordIndex = 0;
        size_t charIndex = 0;

        for(size_t i = 0; i < stringLength; i++)
        {
            char currentChar = string[i];
            if(currentChar == ',')
            {
                keywordIndex++;
                charIndex = 0;
            }
            else if((currentChar != ' ') && (currentChar != '\t') && (currentChar != '\n') && (currentChar != '\0'))
            {
                keywordStorage[keywordIndex] |= static_cast<__int64>(currentChar) << (8*charIndex);
                charIndex++;
            }
        }

        return HashedKeywords<keywordCount>(keywordStorage);
    }

    __int64 findFunctionIndexForHashedKeyword(__int64 hashKeyword);

    ConstructKeyEnum(kind,
                     link,
                     files,
                     define,
                     config,
                     include,
                     startup,
                     depends,
                     project,
                     cfgscope,
                     solution)
}

#ifdef DEBUG
void checkIfKeywordListSorted()
{
    bool isOrderWrong = false;

    for(size_t i = 0; i < Keyword::keyCount - 1; i++)
    {
        if(Keyword::hashedKeywords.data[i] > Keyword::hashedKeywords.data[i+1])
        {
            isOrderWrong = true;
            break;
        }
    }

    if(isOrderWrong)
    {
        for(;;)
        {
            bool isInOrder = true;

            for(size_t i = 0; i < Keyword::keyCount - 1; i++)
            {
                if(Keyword::hashedKeywords.data[i] > Keyword::hashedKeywords.data[i+1])
                {
                    __int64 biggerOne = Keyword::hashedKeywords.data[i];
                    Keyword::hashedKeywords.data[i] = Keyword::hashedKeywords.data[i+1];
                    Keyword::hashedKeywords.data[i+1] = biggerOne;
                    isInOrder = false;
                }
            }

            if(isInOrder) { break; }
        }

        PRINTOUT("Keyword order is wrong. Should follow this order:\n");
        for(size_t i = 0; i < Keyword::keyCount; i++)
        {
            String keyword;
            keyword.buffer = reinterpret_cast<char*>(&Keyword::hashedKeywords.data[i]);
            keyword.length = 0;
            for(size_t i = 0; i < 8; i++)
            {
                if(keyword.buffer[i]) // Although the original string is not null-terminated, this is okay because __int64 storage is zero initialized
                {
                    keyword.length++;
                }
                else
                {
                    break;
                }
            }

            Console::print(keyword);
            if(i != Keyword::keyCount -1) { PRINTOUT(",\n"); }
        }

        __fastfail(1);
    }
}
#endif

__int64 Keyword::stringToHashedKeyword(String keyword)
{
    __int64 hashedKeyword = 0;

    for(size_t i = 0; i < keyword.length; i++)
    {
        hashedKeyword |= static_cast<__int64>(keyword.buffer[i]) << (8*i);
    }

    return hashedKeyword;
}

__int64 Keyword::findFunctionIndexForHashedKeyword(__int64 hashKeyword)
{
    int lowerIndex = 0;
    int upperIndex = keyCount - 1;

    for(;;)
    {
        int index = (lowerIndex + upperIndex) / 2;

        if (hashedKeywords.data[index] < hashKeyword)
        {
            lowerIndex = index + 1;  
        }
        else if (hashedKeywords.data[index] > hashKeyword)
        {
            upperIndex = index - 1;
        }
        else
        {
            return index;
        }

        if(lowerIndex > upperIndex)
        {
            return keyCount;
        }
    }
}

bool Keyword::callFunctionForHashedKeyword(__int64 hashKeyword, String parameter)
{
    bool (*function)(String) = functionStack[findFunctionIndexForHashedKeyword(hashKeyword)];
    return function(parameter);
}

void Keyword::init()
{
#ifdef DEBUG
    checkIfKeywordListSorted();
#endif
}