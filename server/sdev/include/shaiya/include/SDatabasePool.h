#pragma once
#include <string>
#include <sdev/include/shaiya/common.h>
#include <sdev/include/shaiya/include/SSyncList.h>

namespace shaiya
{
    struct SDatabase;
    struct SLog;

    #pragma pack(push, 1)
    struct SDatabasePool
    {
        SDatabase* db;              //0x00
        SSyncList<SDatabase> list;  //0x04
        HANDLE semaphore;           //0x30
        long semaphoreCount;        //0x34
        ULONG queryTimeout;         //0x38
        ULONG connectionTimeout;    //0x3C
        // e.g. 127.0.0.1
        Array<char, 256> server;    //0x40
        Array<char, 256> dbName;    //0x140
        Array<char, 256> username;  //0x240
        Array<char, 256> password;  //0x340
        bool unknown;               //0x440
        PAD(3);
        CRITICAL_SECTION cs444;     //0x444
        SLog* log;                  //0x45C
        // 0x460

        static SDatabase* AllocDB();
        static void FreeDB(SDatabase* db);
        static bool Connect(
            int numConnection,
            const std::string& server,
            const std::string& dbName,
            const std::string& username,
            const std::string& password,
            ULONG queryTimeout,
            ULONG connectionTimeout
        );
        static void Disconnect();
        static void Release();
    };
    #pragma pack(pop)

    static SDatabasePool* g_DatabasePool = (SDatabasePool*)0x109BED0;
}
