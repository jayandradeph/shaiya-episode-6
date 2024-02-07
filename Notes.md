# Introduction

This page will document some things that may be of interest. Some of the notes are based on modifications.

## StatusDefs

### Method

```
0x4D8210 CGameData::LoadStatus
```

### Data

| Column    | Size    |
|-----------|---------|
| Level     | 2       |
| Job       | 1       |
| HP        | 2       |
| SP        | 2       |
| MP        | 2       |

### Math

```cpp
// requires 36 bytes per level
(numJobs * (sizeof(hp) + sizeof(sp) + sizeof(mp))) * (maxLevel + 1)
```

```
(6 * 6) * 71 = 2556 (0x9FC)
(6 * 6) * 81 = 2916 (0xB64)
```

### Static Memory

```cpp
struct Status
{
    ushort hp;
    ushort sp;
    ushort mp;
};

struct StatusDefs
{
    array<Status, 71> job0;
    array<Status, 71> job1;
    array<Status, 71> job2;
    array<Status, 71> job3;
    array<Status, 71> job4;
    array<Status, 71> job5;
    // 0x9FC
};
```

#### Max Level 70

```
004D83B2 imul eax,eax,47
004D83D1 imul eax,eax,47
004D83F0 imul eax,eax,47
```

| Job  | Start       | End         | Size     |
|------|-------------|-------------|----------|
| 0    | 0x588808    | 0x5889B2    | 0x1AA    |
| 1    | 0x5889B2    | 0x588B5C    | 0x1AA    |
| 2    | 0x588B5C    | 0x588D06    | 0x1AA    |
| 3    | 0x588D06    | 0x588EB0    | 0x1AA    |
| 4    | 0x588EB0    | 0x58905A    | 0x1AA    |
| 5    | 0x58905A    | 0x589204    | 0x1AA    |

#### Max Level 80

```
004D83B2 imul eax,eax,51
004D83D1 imul eax,eax,51
004D83F0 imul eax,eax,51
```

| Job  | Start       | End         | Size     |
|------|-------------|-------------|----------|
| 0    | 0x588808    | 0x5889E8    | 0x1E0    |
| 1    | 0x5889E8    | 0x588BC8    | 0x1E0    |
| 2    | 0x588BC8    | 0x588DA8    | 0x1E0    |
| 3    | 0x588DA8    | 0x588F88    | 0x1E0    |
| 4    | 0x588F88    | 0x589168    | 0x1E0    |
| 5    | 0x589168    | 0x589348    | 0x1E0    |

The priest/oracle memory will overrun (324 bytes) at address `0x589204`. The data for level 72 and above will be overwritten by **CGameData::LoadExp** when it reads the **ExpDefs** table and copies the values to memory.

## ExpDefs

### Method

```
0x4D8440 CGameData::LoadExp
```

### Data

| Column    | Size    |
|-----------|---------|
| Level     | 2       |
| Grow      | 1       |
| Exp       | 4       |

### Math

```cpp
// requires 16 bytes per level
(numModes * sizeof(exp)) * (maxLevel + 1)
```

```
16 * 71 = 1136 (0x470)
16 * 81 = 1296 (0x510)
```

### Static Memory

```cpp
struct ExpDefs
{
    array<uint, 71> easy;
    array<uint, 71> normal;
    array<uint, 71> hard;
    array<uint, 71> ultimate;
    // 0x470
};
```

#### Max Level 70

```
004D8543 imul eax,eax,47
004D859B add edi,11C
```

| Grow  | Start       | End         | Size     |
|-------|-------------|-------------|----------|
| 0     | 0x589204    | 0x589320    | 0x11C    |
| 1     | 0x589320    | 0x58943C    | 0x11C    |
| 2     | 0x58943C    | 0x589558    | 0x11C    |
| 3     | 0x589558    | 0x589674    | 0x11C    |

#### Max Level 80

```
004D8543 imul eax,eax,51
004D859B add edi,144
```

| Grow  | Start       | End         | Size     |
|-------|-------------|-------------|----------|
| 0     | 0x589204    | 0x589348    | 0x144    |
| 1     | 0x589348    | 0x58948C    | 0x144    |
| 2     | 0x58948C    | 0x5895D0    | 0x144    |
| 3     | 0x5895D0    | 0x589714    | 0x144    |

The memory will overrun (160 bytes) at address `0x589674`. The data for level 71 and above will be overwritten by **CGameData::LoadConstItem** when it reads the **Items** table and copies the values to memory.