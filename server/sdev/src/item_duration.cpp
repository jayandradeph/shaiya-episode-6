#include <chrono>
#include <ranges>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <include/main.h>
#include <include/util.h>
#include <include/shaiya/packets/0200.h>
#include <include/shaiya/packets/0700.h>
#include <include/shaiya/packets/dbAgent/0700.h>
#include <include/shaiya/packets/gameLog/0400.h>
#include <include/shaiya/include/CGameData.h>
#include <include/shaiya/include/CItem.h>
#include <include/shaiya/include/CObjectMgr.h>
#include <include/shaiya/include/CUser.h>
#include <include/shaiya/include/CWorld.h>
#include <include/shaiya/include/ItemDuration.h>
#include <include/shaiya/include/SConnection.h>
#include <include/shaiya/include/SConnectionTBaseReconnect.h>
#include <include/shaiya/include/ServerTime.h>
using namespace shaiya;

namespace item_duration
{
    void send_delete_notice(CUser* user, CItem* item, std::uint8_t bag, std::uint8_t slot)
    {
        ItemExpireNoticeOutgoing packet{};
        packet.type = item->type;
        packet.typeId = item->typeId;

        if (bag == warehouse_bag)
        {
            packet.noticeType = ItemExpireNoticeType::DeletedFromWarehouse;

            UserItemBankToBankIncoming packet{ 0x706, user->userId, slot, item->count, slot, 0 };
            SConnectionTBaseReconnect::Send(g_pClientToDBAgent, &packet, sizeof(UserItemBankToBankIncoming));

            GameLogItemRemoveIncoming log{};
            CUser::SetGameLogMain(user, &log);

            log.itemUid = item->uniqueId;
            log.itemId = item->itemInfo->itemId;
            log.itemName = item->itemInfo->itemName;
            log.gems = item->gems;
            log.makeTime = item->makeTime;
            log.craftName = item->craftName;
            log.bag = bag;
            log.slot = slot;
            log.count = item->count;
            SConnectionTBaseReconnect::Send(g_pClientToGameLog, &log, sizeof(GameLogItemRemoveIncoming));

            CObjectMgr::FreeItem(item);
            user->warehouse[slot] = nullptr;

            ItemBankToBankOutgoing outgoing{};
            outgoing.srcItem.bag = bag;
            outgoing.srcItem.slot = slot;
            outgoing.destItem.bag = bag;
            outgoing.destItem.slot = slot;
            SConnection::Send(&user->connection, &outgoing, sizeof(ItemBankToBankOutgoing));
        }
        else
        {
            packet.noticeType = ItemExpireNoticeType::DeletedFromInventory;
            CUser::ItemRemove(user, bag, slot);
        }

        SConnection::Send(&user->connection, &packet, sizeof(ItemExpireNoticeOutgoing));
    }

    void send_expire_notice(CUser* user, CItem* item, std::uint8_t bag, std::uint8_t slot)
    {
        auto seconds = std::chrono::seconds(std::chrono::days(item->itemInfo->range)).count();

        auto expireTime = ServerTime::Add(item->makeTime, seconds);
        if (!expireTime)
            return;

        SYSTEMTIME st{};
        ServerTime::ServerTimeToSystemTime(expireTime, &st);
        ItemDuration duration(st);

        if (duration.expired())
        {
            send_delete_notice(user, item, bag, slot);
            return;
        }

        if (!duration.days)
        {
            ItemExpireNoticeOutgoing packet{};
            packet.type = item->type;
            packet.typeId = item->typeId;

            if (!duration.hours)
            {
                packet.timeValue = static_cast<std::uint8_t>(duration.minutes);

                if (bag == warehouse_bag)
                    packet.noticeType = ItemExpireNoticeType::MinutesLeftWarehouse;
                else
                    packet.noticeType = ItemExpireNoticeType::MinutesLeftInventory;
            }
            else
            {
                packet.timeValue = static_cast<std::uint8_t>(duration.hours);

                if (bag == warehouse_bag)
                    packet.noticeType = ItemExpireNoticeType::HoursLeftWarehouse;
                else
                    packet.noticeType = ItemExpireNoticeType::HoursLeftInventory;
            }

            SConnection::Send(&user->connection, &packet, sizeof(ItemExpireNoticeOutgoing));
        }
    }

    void send(CUser* user, CItem* item, std::uint8_t bag, std::uint8_t slot)
    {
        if (ItemHasDuration(item->itemInfo))
        {
            auto seconds = std::chrono::seconds(std::chrono::days(item->itemInfo->range)).count();

            auto expireTime = ServerTime::Add(item->makeTime, seconds);
            if (!expireTime)
                return;

            ItemDurationOutgoing packet{};
            packet.bag = bag;
            packet.slot = slot;
            packet.fromDate = item->makeTime;
            packet.toDate = expireTime;
            SConnection::Send(&user->connection, &packet, sizeof(ItemDurationOutgoing));

            send_expire_notice(user, item, bag, slot);
        }
    }

    void send_bag_and_bank(CUser* user)
    {
        for (const auto& [bag, items] : std::views::enumerate(
            std::as_const(user->inventory)))
        {
            for (const auto& [slot, item] : std::views::enumerate(
                std::as_const(items)))
            {
                if (!item)
                    continue;

                send(user, item, bag, slot);
            }
        }

        for (const auto& [slot, item] : std::views::enumerate(
            std::as_const(user->warehouse)))
        {
            if (!item)
                continue;

            send(user, item, warehouse_bag, slot);
        }
    }

    void send_bag_to_bank(CUser* user, Packet buffer)
    {
        auto slot = util::deserialize<std::uint8_t>(buffer, 37);
        if (slot >= user->warehouse.size())
            return;

        auto& item = user->warehouse[slot];
        if (!item)
            return;

        if (ItemHasDuration(item->itemInfo))
        {
            auto seconds = std::chrono::seconds(std::chrono::days(item->itemInfo->range)).count();

            auto expireTime = ServerTime::Add(item->makeTime, seconds);
            if (!expireTime)
                return;

            ItemDurationOutgoing packet{};
            packet.bag = warehouse_bag;
            packet.slot = slot;
            packet.fromDate = item->makeTime;
            packet.toDate = expireTime;
            SConnection::Send(&user->connection, &packet, sizeof(ItemDurationOutgoing));
        }
    }

    void send_item_create(CUser* user, CItem* item, Packet buffer)
    {
        if (ItemHasDuration(item->itemInfo))
        {
            auto seconds = std::chrono::seconds(std::chrono::days(item->itemInfo->range)).count();

            auto expireTime = ServerTime::Add(item->makeTime, seconds);
            if (!expireTime)
                return;

            ItemDurationOutgoing packet{};
            packet.bag = util::deserialize<std::uint8_t>(buffer, 2);
            packet.slot = util::deserialize<std::uint8_t>(buffer, 3);
            packet.fromDate = item->makeTime;
            packet.toDate = expireTime;
            SConnection::Send(&user->connection, &packet, sizeof(ItemDurationOutgoing));
        }
    }

    void world_thread_update()
    {
        auto now = std::chrono::system_clock::now();
        if (now < g_world_thread_update_time_point)
            return;

        g_world_thread_update_time_point = now + std::chrono::minutes(1);

        for (const auto& charId : g_users)
        {
            auto user = CWorld::FindUser(charId);
            if (!user)
            {
                std::erase(g_users, charId);
                continue;
            }

            if (user->logoutType != UserLogoutType::None)
                continue;

            for (const auto& [bag, items] : std::views::enumerate(
                std::as_const(user->inventory)))
            {
                for (const auto& [slot, item] : std::views::enumerate(
                    std::as_const(items)))
                {
                    if (!item)
                        continue;

                    if (ItemHasDuration(item->itemInfo))
                    {
                        auto seconds = std::chrono::seconds(std::chrono::days(item->itemInfo->range)).count();

                        auto expireTime = ServerTime::Add(item->makeTime, seconds);
                        if (!expireTime)
                            continue;

                        SYSTEMTIME st{};
                        ServerTime::ServerTimeToSystemTime(expireTime, &st);
                        ItemDuration duration(st);

                        if (duration.expired())
                            send_delete_notice(user, item, bag, slot);
                    }
                }
            }

            for (const auto& [slot, item] : std::views::enumerate(
                std::as_const(user->warehouse)))
            {
                if (!user->doubleWarehouse && slot >= min_warehouse_slot)
                    break;

                if (!item)
                    continue;

                if (ItemHasDuration(item->itemInfo))
                {
                    auto seconds = std::chrono::seconds(std::chrono::days(item->itemInfo->range)).count();

                    auto expireTime = ServerTime::Add(item->makeTime, seconds);
                    if (!expireTime)
                        continue;

                    SYSTEMTIME st{};
                    ServerTime::ServerTimeToSystemTime(expireTime, &st);
                    ItemDuration duration(st);

                    if (duration.expired())
                        send_delete_notice(user, item, warehouse_bag, slot);
                }
            }
        }
    }
}

unsigned u0x492500 = 0x492500;
unsigned u0x47BC54 = 0x47BC54;
void __declspec(naked) naked_0x47BC4F()
{
    __asm
    {
        // original
        call u0x492500

        pushad

        push ebp // user
        call item_duration::send_bag_and_bank
        add esp,0x4

        popad
     
        jmp u0x47BC54
    }
}

unsigned u0x4ED0E0 = 0x4ED0E0;
unsigned u0x469934 = 0x469934;
void __declspec(naked) naked_0x46992F()
{
    __asm
    {
        // original
        call u0x4ED0E0

        pushad

        lea edx,[esp+0x40]

        push edx // packet
        push edi // user
        call item_duration::send_bag_to_bank
        add esp,0x8

        popad
     
        jmp u0x469934
    }
}

unsigned u0x46C22F = 0x46C22F;
void __declspec(naked) naked_0x46C22A()
{
    __asm
    {
        // original
        call u0x4ED0E0

        pushad

        lea edx,[esp+0xB4]
        mov ecx,[esp+0x38]

        push edx // packet
        push ebp // item
        push ecx // user
        call item_duration::send_item_create
        add esp,0xC

        popad
     
        jmp u0x46C22F
    }
}

// CWorldThread::UpdateKillCount
unsigned u0x4042A0 = 0x4042A0;
unsigned u0x404076 = 0x404076;
void __declspec(naked) naked_0x404071()
{
    __asm
    {
        // original
        call u0x4042A0

        pushad

        call item_duration::world_thread_update

        popad
     
        jmp u0x404076
    }
}

void hook::item_duration()
{
    // CUser::PacketUserDBChar
    util::detour((void*)0x47BC4F, naked_0x47BC4F, 5);
    // CUser::ItemBagToBank
    util::detour((void*)0x46992F, naked_0x46992F, 5);
    // CUser::ItemCreate
    util::detour((void*)0x46C22A, naked_0x46C22A, 5);
    // CWorldThread::Update
    util::detour((void*)0x404071, naked_0x404071, 5);
}
