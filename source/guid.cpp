#include "guid.h"
#include <bcrypt.h>

namespace Guid
{
    constexpr char length = 37;
    BCRYPT_ALG_HANDLE algorithm;
}

void Guid::init()
{
    BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_RNG_ALGORITHM, nullptr, 0);
}

void Guid::generate(char* outBuffer)
{
    BCryptGenRandom(algorithm, reinterpret_cast<unsigned char*>(outBuffer), length, 0);

    for(size_t i = 0; i < length; i++)
    {
        outBuffer[i] = "0123456789ABCDEF"[(outBuffer[i]+128) % 16];
    }

    outBuffer[9] = outBuffer[14] = outBuffer[19] = outBuffer[24] = '-';
}