# Introduction

The reason I started this project was to replace the [pet](https://www.elitepvpers.com/forum/shaiya-pserver-guides-releases/4102262-release-shen1ls-wings.html) library with an open source alternative. See [this](https://forum.shaiya.gg/threads/an-open-source-effort-to-make-episode-5-servers-compatible-with-episode-6-clients.10/) post in the Shaiya.gg forum for a list of contributors that helped me in some way or another.

## Releases

Navigate to the [actions](https://github.com/kurtekat/shaiya-episode-6/actions) tab and download the artifacts uploaded during workflow runs. Two workflows will run the 1st of each month to build release versions of the 5.4 library and the 6.4 PT server libraries.

## Development

The project is no longer in development. Please use the issues tab for questions or bug reports. You can also contact me (Bowie) via the Shaiya.gg forum.

# How-to-use

## Environment

Windows 10

Visual Studio 2022

C++ 23

## Prerequisites

[Microsoft Visual C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x86.exe)

# Libraries

## Injection

Cheat Engine isn't guaranteed to work for everyone. It's not an issue with the application or the libraries. The libraries have an export defined in the main headers.

## sdev-db

This library is for the **dbAgent** service. It is required for episode 6 clients, except versions like [0x12092000](https://github.com/kurtekat/shaiya-episode-6/tree/main/client/0x12092000). It adds support for the character list packet and the name availability packet.

## sdev

This library is for the **game** service. Macros are used to represent the differences in the episode 6 packets, etc.

### Item Mall

The procedures in the [sql](https://github.com/kurtekat/shaiya-episode-6/tree/main/server/sdev/sql) directory need to be installed. If you skip this step, the item mall will not work. If you receive an error, change `ALTER` to `CREATE` and try again. This solution does not require a connection to SQL Server.

### Macros

To use this library with episode 5 clients, comment the episode 6 macros. This is the default.

```cpp
// sdev/include/shaiya/common.h
//#define SHAIYA_EP6
//#define SHAIYA_EP6_3_US
//#define SHAIYA_EP6_3_PT
//#define SHAIYA_EP6_4_PT
```

For episode 6.4 clients, expose the `SHAIYA_EP6_4_PT` macro.

```cpp
// sdev/include/shaiya/common.h
//#define SHAIYA_EP6
//#define SHAIYA_EP6_3_US
//#define SHAIYA_EP6_3_PT
#define SHAIYA_EP6_4_PT
```

# Binaries

## ps_game

File **v1** is very close to the original code. File **v2** has been modified to support the episode 6 NpcQuest format. The **game** service uses C functions to do file I/O, so it will fail silently if the **SData** format is incorrect.

# Features

## NpcQuest

The episode 6 format has 6 quest results, each containing up to 3 items. The library reads the item count from memory. The original code passes 1 to **CUser::QuestAddItem**, which is why increasing the item count has no effect.

## Item Duration

Use the **Range** column in the **Items** table to store the number of days. The client stores the duration as seconds.

| Days    | Seconds    |
|---------|------------|
| 1       | 86400      |
| 7       | 604800     |
| 30      | 2592000    |

The **ExtDuration** field (see Parsec) should be 1 or 0 (true or false). It determines whether or not the following text appears in the item description.

```
// US
889    "Duration [<v>day(s)]"
890    "Duration [<v>day(s)]"

// PT
889    "Duração [<v>dia(s)][expansível]"
890    "Duração [<v>dia(s)][expansível]"
```

Make sure `[dbo].[usp_Read_Char_Items_Simple_R]` hasn't been modified to delete items. The library does not delete items without sending a notification to the client. A copy of the original procedure can be found [here](https://github.com/kurtekat/shaiya-episode-6/blob/main/server/sdev/sql/%5Bdbo%5D.%5Busp_Read_Char_Items_Simple_R%5D.sql). Items will not be removed from guild warehouses.

## Revenge Mark

The kill count will determine which effect will be rendered. Notice effects are not supported.

| KillCount    | Effect                     |
|--------------|----------------------------|
| 0            | N/A                        |
| 1            | RevengeMark_Loop_01.EFT    |
| 2            | RevengeMark_Loop_02.EFT    |
| 3            | RevengeMark_Loop_03.EFT    |
| 4            | RevengeMark_Loop_04.EFT    |
| 5            | RevengeMark_Loop_05.EFT    |
| 6            | RevengeMark_Loop_06.EFT    |
| 7            | RevengeMark_Loop_07.EFT    |
| 8 >          | RevengeMark_Loop_08.EFT    |

The 6.4 PT clients in this repository support message 508, but do not support message 509.

```
508    "Your revenge to <t> has succeeded!"
509    "<t> killed  you <v> time(s)."
```

## Frenzied Skills

Skill ability 70 will be used for evaluation. The following skills are supported.

| SkillId    | SkillName           |
|------------|---------------------|
| 398        | Frenzied Force      |
| 399        | Frenzied Focus      |
| 400        | Frenzied Finesse    |
| 401        | Frenzied Fortune    |

## Mailbox

This feature will not be implemented.

## Chaotic Squares

### ItemSynthesis.ini

Use the following example to get started.

```ini
;PSM_Client\Bin\Data\ItemSynthesis.ini

[ItemSynthesis_0]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
CreateType=30
CreateTypeID=6
CreateCount=1

[ItemSynthesis_1]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=12,12,12,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
CreateType=30
CreateTypeID=13
CreateCount=1

[ItemSynthesis_2]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=19,19,19,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
CreateType=30
CreateTypeID=20
CreateCount=1

[ItemSynthesis_3]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=26,26,26,26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
CreateType=30
CreateTypeID=27
CreateCount=1

[ItemSynthesis_4]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=33,33,33,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
CreateType=30
CreateTypeID=34
CreateCount=1

[ItemSynthesis_5]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=40,40,40,40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
CreateType=30
CreateTypeID=41
CreateCount=1
```

### Free Combination

This feature will not be implemented.

### Money

The client uses the following math to determine the maximum gold input.

```
goldPerPercentage * 4 + goldPerPercentage
```

Set the minimum and maximum constants in **Synthesis.h** to change the result. The minimum will be used as a divisor (see packet_gem.cpp line 606), so don't set it to zero.

```cpp
constexpr int synthesis_min_money = 1000;
constexpr int synthesis_max_money = 5000;
```

### Crafting Hammers

The item effect is expected to be 102. Use the **ReqVg** column in the **Items** table to store the success rate. The library will multiply the value by 100.

| ItemID    | ReqVg    |
|-----------|----------|
| 102074    | 5        |
| 102075    | 10       |

## Synergy

The library expects **SetItem.SData** to be in the **PSM_Client/Bin/Data** directory. I chose binary format because it's less error-prone than fetching the values from a database.

### Client Format

The client expects the file to be encrypted.

![Capture2](https://github.com/kurtekat/shaiya-episode-6/assets/142125482/01b93010-05a5-4323-b8d5-e890551ed4b7)

#### Description

`" Sinergia [5]\n- LUC +20\n- DEX +50\n- STR +70"`


### Server Format

The library expects the file to be decrypted. The bonus text is expected to be 12 comma-separated values.

![Capture](https://github.com/kurtekat/shaiya-episode-6/assets/142125482/a0a0c116-c5a0-4443-953e-35077e29f065)

The values are signed 32-bit integers, expected to be in the following order.

```cpp
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
```