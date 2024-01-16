#pragma once
#include <include/shaiya/common.h>

// CUser::PacketPC

namespace shaiya
{
    enum struct MotionType : UINT8
    {
        None,
        Walk,
        Run,
        WalkBack,
        WalkLeft,
        WalkRight,
        Wade,
        Swim,
        Jump,
        Death,
        SitDown,
        StandUp,
        Break,
        JumpBack,
        JumpLeft,
        JumpRight,
        Idle1,
        Idle2,
        Climb,
        SelectionScreen,
        VehicleRunMode,
        Vehicle,
        Snowboard,
        TwoHandedAtkMode,
        TwoHandedAttack1,
        TwoHandedAttack2,
        TwoHandedAttack3,
        TwoHandedAttack4,
        TwoHandedDamage,
        TwoHandedRunMode,
        BowAtkMode,
        BowAttack,
        BowDamage,
        BowRunMode,
        OneHandedAtkMode,
        OneHandedAttack1,
        OneHandedAttack2,
        OneHandedAttack3,
        OneHandedAttack4,
        OneHandedDamage,
        OneHandedRunMode,
        DualWieldAtkMode,
        DualWieldAttack1,
        DualWieldAttack2,
        DualWieldAttack3,
        DualWieldAttack4,
        DualWieldDamage,
        DualWieldRunMode,
        SpearAtkMode,
        SpearAttack1,
        SpearAttack2,
        SpearAttack3,
        SpearAttack4,
        SpearDamage,
        SpearRunMode,
        JavelinAtkMode,
        JavelinAttack,
        JavelinDamage,
        JavelinRunMode,
        StaffAtkMode,
        StaffAttack1,
        StaffAttack2,
        StaffDamage,
        StaffRunMode,
        ReverseAtkMode,
        ReverseAttack1,
        ReverseAttack2,
        ReverseAttack3,
        ReverseAttack4,
        ReverseDamage,
        ReverseRunMode,
        KnuckleAtkMode,
        KnuckleAttack1,
        KnuckleAttack2,
        KnuckleAttack3,
        KnuckleAttack4,
        KnuckleDamage,
        KnuckleRunMode,
        DaggerAtkMode,
        DaggerAttack1,
        DaggerAttack2,
        DaggerAttack3,
        DaggerAttack4,
        DaggerDamage,
        DaggerRunMode,
        Cast85,
        Cast86,
        UseSkill87,
        Cast88,
        Cast89,
        UseSkill90,
        Cast91,
        Cast92,
        UseSkill93,
        Cast94,
        Cast95,
        UseSkill96,
        Cast97,
        Cast98,
        UseSkill99,
        Skill100,
        Skill101,
        Skill102,
        Skill103,
        Skill104,
        Skill105,
        Skill106,
        Skill107,
        Skill108,
        Skill109,
        Skill110,
        Beg = 116,
        Victory,
        Laugh,
        Love,
        Greet,
        Clap,
        Defeat,
        Start,
        Insult,
        Provoke
    };

    enum struct SkillUseFrenzyState : UINT8
    {
        Triggered = 1,
        Stopped = 3
    };

    #pragma pack(push, 1)
    struct UserMoveIncoming
    {
        UINT16 opcode{ 0x501 };
        // CUser->144D
        UINT8 unknown;
        UINT16 direction;
        float x;
        float y;
        float z;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct UserMoveOutgoing
    {
        UINT16 opcode{ 0x501 };
        ULONG charId;
        // CUser->144D
        UINT8 unknown;
        UINT16 direction;
        float x;
        float y;
        float z;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct RecoverAddOutgoing
    {
        UINT16 opcode{ 0x505 };
        ULONG charId;
        int health;
        int mana;
        int stamina;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct UserMotionIncoming
    {
        UINT16 opcode{ 0x506 };
        MotionType motionType;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct UserMotionOutgoing
    {
        UINT16 opcode{ 0x506 };
        ULONG charId;
        MotionType motionType;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct UserEquipmentOutgoing
    {
        UINT16 opcode{ 0x507 };
        ULONG charId;
        UINT8 slot;
        UINT8 type;
        UINT8 typeId;
        UINT8 enchantStep;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct SkillApplyOutgoing
    {
        UINT16 opcode{ 0x50D };
        // CSkill->id
        ULONG id;
        UINT16 skillId;
        UINT8 skillLv;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct SkillRemoveOutgoing
    {
        UINT16 opcode{ 0x50E };
        // CSkill->id
        ULONG id;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct SkillKeepOutgoing
    {
        UINT16 opcode{ 0x50F };
        ULONG charId;
        UINT16 skillId;
        UINT8 skillLv;
        UINT16 health;
        UINT16 mana;
        UINT16 stamina;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct SkillUseIncoming
    {
        UINT16 opcode{ 0x511 };
        UINT8 skillIndex;
        // 0 = caster
        CharId targetId;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct SkillUseOutgoing
    {
        UINT16 opcode{ 0x511 };
        UINT8 targetType;
        CharId senderId;
        CharId targetId;
        UINT16 skillId;
        UINT8 skillLv;
        UINT16 health;
        UINT16 stamina;
        UINT16 mana;
        #ifdef SHAIYA_EP6_0511
        SkillUseFrenzyState frenzyState;
        #endif
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct UserShapeTypeOutgoing
    {
        UINT16 opcode{ 0x51D };
        ULONG charId;
        ShapeType shapeType;
        #ifdef SHAIYA_EP6_4_PT
        UINT32 vehicleType;
        UINT32 vehicleTypeId;
        #endif
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct RecoverSetOutgoing
    {
        UINT16 opcode{ 0x521 };
        UINT32 health;
        UINT32 mana;
        UINT32 stamina;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct UserKillCountOutgoing
    {
        UINT16 opcode{ 0x522 };
        ULONG charId;
        UINT32 kills;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct UserStatusOutgoing
    {
        UINT16 opcode{ 0x526 };
        UINT32 strength;
        UINT32 reaction;
        UINT32 intelligence;
        UINT32 wisdom;
        UINT32 dexterity;
        UINT32 luck;
        UINT32 minAttackPower;
        UINT32 maxAttackPower;
        UINT32 minMagicPower;
        UINT32 maxMagicPower;
        UINT32 defense;
        UINT32 resistance;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct ItemRepairIncoming
    {
        UINT16 opcode{ 0x554 };
        UINT8 bag;
        UINT8 slot;
        ULONG npcId;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct ItemRepairOutgoing
    {
        UINT16 opcode{ 0x554 };
        UINT8 bag;
        UINT8 slot;
        UINT32 money;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct ItemTownScrollIncoming
    {
        UINT16 opcode{ 0x55A };
        UINT8 bag;
        UINT8 slot;
        UINT8 gateIndex;
    };
    #pragma pack(pop)
}
