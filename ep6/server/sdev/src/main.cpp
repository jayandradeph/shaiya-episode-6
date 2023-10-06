#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <include/main.h>
#include <include/util.h>
#include <include/shaiya/include/CUser.h>
#include <include/shaiya/include/Synergy.h>
using namespace shaiya;

void enter_world_hook(CUser* user)
{
    CUser::UpdateKCStatus(user);
}

void leave_world_hook(CUser* user)
{
    g_appliedSynergies.erase(user->id);
}

unsigned u0x455B06 = 0x455B06;
void __declspec(naked) naked_0x455B00()
{
    __asm
    {
        pushad

        push ecx // user
        call enter_world_hook
        add esp,0x4

        popad

        // original
        sub esp,0x8C
        jmp u0x455B06
    }
}

unsigned u0x455C46 = 0x455C46;
void __declspec(naked) naked_0x455C40()
{
    __asm
    {
        pushad

        push ecx // user
        call leave_world_hook
        add esp,0x4

        popad

        // original
        push ebp
        mov ebp,esp
        and esp,-0x8
        jmp u0x455C46
    }
}

unsigned u0x45516B = 0x45516B;
void __declspec(naked) naked_0x455165()
{
    __asm
    {
        // original
        mov [edi+0x6264],ebx

        // initialize memory
        mov [edi+0x1534],ebx
        mov [edi+0x1538],ebx
        mov [edi+0x153C],ebx
        mov [edi+0x1540],ebx
        mov [edi+0x15E4],ebx

        jmp u0x45516B
    }
}

void Main(HMODULE hModule)
{
    DisableThreadLibraryCalls(hModule);
    // CUser::EnterWorld
    util::detour((void*)0x455B00, naked_0x455B00, 6);
    // CUser::LeaveWorld
    util::detour((void*)0x455C40, naked_0x455C40, 6);
    // CUser::CUser
    util::detour((void*)0x455165, naked_0x455165, 6);

    hook::packet_exchange();
    hook::packet_shop();

    #ifdef SHAIYA_EP6
    hook::item_duration();
    hook::item_effect();
    hook::packet_character();
    hook::packet_gem();
    hook::packet_myshop();
    hook::user_equipment();
    hook::user_shape();
    hook::user_status();
    #endif

    #ifdef SHAIYA_EP6_4
    hook::npc_quest();
    hook::packet_market();
    hook::toggle_skill();
    Synergy::init();
    #endif
}
