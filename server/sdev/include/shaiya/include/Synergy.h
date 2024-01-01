#pragma once
#include <array>
#include <fstream>
#include <map>
#include <vector>

#include <include/shaiya/common.h>

namespace shaiya
{
    FWDDECL CUser;

    struct SynergyAbility
    {
        int strength;
        int dexterity;
        int intelligence;
        int wisdom;
        int reaction;
        int luck;
        int health;
        int mana;
        int stamina;
        int attackPower;
        int rangedAttackPower;
        int magicPower;

        bool isNull();
    };

    struct Synergy
    {
        short id;
        std::array<SynergyAbility, 13> ability{};
        std::array<ItemId, 13> set{};

        static void init();
        static void applySynergies(CUser* user);
        static void removeSynergies(CUser* user);

    private:
        static void getWornSynergies(CUser* user, std::vector<SynergyAbility>& synergies);
        static void parseAbility(std::ifstream& ifs, SynergyAbility& ability);
    };

    inline std::vector<Synergy> g_synergies;
    inline std::map<CharId, std::vector<SynergyAbility>> g_appliedSynergies;
}
