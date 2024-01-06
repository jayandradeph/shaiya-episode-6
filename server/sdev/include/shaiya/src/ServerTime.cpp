#include <chrono>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <include/shaiya/include/CGameData.h>
#include <include/shaiya/include/ServerTime.h>
using namespace shaiya;

ULONG ServerTime::GetExpireTime(ULONG makeTime, int days)
{
    if (days <= 0)
        return 0;

    SYSTEMTIME st{};
    ServerTime::ServerTimeToSystemTime(makeTime, &st);

    FILETIME ft{};
    SystemTimeToFileTime(&st, &ft);

    ULARGE_INTEGER li{ ft.dwLowDateTime, ft.dwHighDateTime };

    auto seconds = std::chrono::seconds(std::chrono::days(days)).count();
    li.QuadPart += seconds * std::chrono::microseconds(std::chrono::seconds(10)).count();

    ft.dwLowDateTime = li.LowPart;
    ft.dwHighDateTime = li.HighPart;

    FileTimeToSystemTime(&ft, &st);
    return ServerTime::SystemTimeToServerTime(&st);
}

ULONG ServerTime::GetSystemTime()
{
    typedef ULONG(__stdcall* LPFN)();
    return (*(LPFN)0x4E1A50)();
}

bool ServerTime::IsTimedItem(CGameData::ItemInfo* itemInfo)
{
    if (!itemInfo)
        return false;

    int days = itemInfo->range;
    if (days <= 0)
        return false;

    switch (static_cast<CGameData::ItemType>(itemInfo->type))
    {
    case CGameData::ItemType::Pet:
    case CGameData::ItemType::Costume:
        return true;
    default:
        break;
    }

    return false;
}

ULONG ServerTime::ServerTimeToSystemTime(ULONG time/*eax*/, LPSYSTEMTIME lpst/*ecx*/)
{
    Address u0x4E1CA0 = 0x4E1CA0;

    __asm
    {
        mov eax,time
        mov ecx,lpst
        call u0x4E1CA0
    }
}

ULONG ServerTime::SystemTimeToServerTime(LPSYSTEMTIME lpst/*ecx*/)
{
    Address u0x4E1CF0 = 0x4E1CF0;

    __asm
    {
        mov ecx,lpst
        call u0x4E1CF0
    }
}
