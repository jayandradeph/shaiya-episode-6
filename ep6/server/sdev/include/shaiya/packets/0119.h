#pragma once
#include <include/shaiya/common.h>

namespace shaiya
{
    #pragma pack(push, 1)
    struct CharNameAvailableRequest
    {
        UINT16 opcode{ 0x119 };
        Array<char, 19> name;
    };

    struct CharNameAvailableResponse
    {
        UINT16 opcode{ 0x119 };
        bool available;
    };
    #pragma pack(pop)
}
