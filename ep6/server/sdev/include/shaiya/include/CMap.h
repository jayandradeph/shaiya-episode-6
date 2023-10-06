#pragma once
#include <include/shaiya/common.h>
#include <include/shaiya/include/SVector.h>

namespace shaiya
{
    FWDDECL CDoor;

    typedef Array<char, 256> SvMapName;

    #pragma pack(push, 1)
    enum struct WarType : UINT32
    {
        Default,
        Peace    // P
    };

    enum struct MapType : UINT32
    {
        Dungeon, // D
        Field    // F
    };

    enum struct CreateType : UINT32
    {
        Dungeon,    // D
        Static,     // S
        Field,      // F
        Party,      // P
        Guild,      // G
        Restricted, // R
        House       // H
    };

    struct CreatTime
    {
        UINT16 day;
        // 24-hr format
        UINT16 hour;
        // hours
        UINT16 duration;
    };

    struct RebirthMap
    {
        UINT32 mapId;
        SVector pos;
    };

    struct CMap
    {
        PAD(4);
        UINT32 size;             //0x04
        PAD(12);
        UINT32 numMobAreas;      //0x14
        PAD(12);
        UINT32 weatherState;     //0x24
        UINT32 weatherPower;     //0x28
        UINT32 weatherRate;      //0x2C
        // value * 60000
        ULONG weatherDelay;      //0x30
        // value * 60000
        ULONG weatherNoneDelay;  //0x34
        PAD(8);
        UINT32 numNpcs;          //0x40
        PAD(12);
        UINT32 numPortals;       //0x50
        CDoor* portal;           //0x54
        UINT32 numSpawnAreas;    //0x58
        PAD(4);
        UINT32 numLadders;       //0x60
        PAD(12);
        UINT32 numNamedAreas;    //0x70
        PAD(4);
        ULONG id;                //0x78
        WarType warType;         //0x7C
        MapType mapType;         //0x80
        SvMapName svMapName;     //0x84
        RebirthMap rebirth1;     //0x184
        RebirthMap rebirth2;     //0x194
        CreateType createType;   //0x1A4
        CreatTime createTime;    //0x1A8
        PAD(38);
        // value * 60000 + 30000
        TickCount expireTime;    //0x1D4
        UINT32 minNumUsers;      //0x1D8
        UINT32 maxNumUsers;      //0x1DC
        PAD(40);
        // 0x208

        static CDoor* GetPortal(CMap* map/*ecx*/, int index/*eax*/);
        static CDoor* GetPortalById(CMap* map/*ecx*/, int insZoneId/*edi*/);
        static bool IsStatic(CMap* map/*ecx*/);
    };
    #pragma pack(pop)
}
