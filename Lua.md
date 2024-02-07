# Parameters

The developers used Hungarian notation to express the data type of parameters.

# Event Handlers

```lua
OnAttacked(dwTime, dwCharId)
OnAttackable(dwTime, dwCharId)
OnNormalReset(dwTime)
OnDeath(dwTime, dwAttackedCount)
OnReturnHome(dwTime, dwAttackedCount)
OnMoveEnd(dwTime)
```

# Functions

```lua
Init()
WhileCombat(dwTime, dwHPPercent, dwAttackedCount)
```

# Methods

```lua
Mob:LuaGetNumberAggro(dwNumber)
Mob:LuaGetMinAggroEx()
Mob:LuaAttackOrderSurroundMob(dwCharId, dlPosX, dlPosZ)
Mob:LuaSay(szMessage, fDist)
Mob:LuaSayByIndex(wIndex, fDist)
Mob:LuaSayByVoice(szFileName, fDist)
Mob:LuaGetName(dwCharId)
Mob:LuaCreateMob(dwMobId, nCount, dlPosX, dlPosZ)
Mob:LuaAttackAI(dwSkillId, dwCharId)
Mob:LuaSetTarget(dwCharId)
Mob:LuaResetTargetingTerm(dwTerm)
Mob:LuaSetOnlyAttack(bLuaAttack)
Mob:LuaHoldPosion(bHoldPosion)
Mob:LuaGetMobCountByType(dwType, dlPosX, dlPosZ, nAddDist)
Mob:LuaSetUnBeatable(bUnBeatable)
Mob:LuaResetAggro()
Mob:LuaDeleteMob(dwMobId, byCount, dlPosX, dlPosZ)
Mob:LuaFixedMove(nMoveMode, dlPosX, dlPosZ)
Mob:LuaRecallUser(byJob, fDist, wMap, dlPosX, dlPosY, dlPosZ)
Mob:LuaRecover(byType, dlValue)
Mob:LuaGetMobPos(dwId, fPosX, fPosZ)
Mob:LuaGetMobHP(dwId)
Mob:LuaUpdateInZonePortal(nPortalId, nCountry)
```

# Usage

```lua
Mob = LuaMob(CMob)
math.randomseed(os.time())

bOnAttacked = 0
dwNextCreateTime = 0
bMobSay	= 0
bMobCreate = 0

function Init()
end

function OnAttacked(dwTime, dwCharId)
    if (bOnAttacked == 0) then
        szCharName = Mob:LuaGetName(dwCharId)
        Mob:LuaSay(szCharName..", you are a fool to challenge me!", 100.0)	
        bOnAttacked = 1
    end
end

function OnAttackable(dwTime, dwCharId)
end

function OnNormalReset(dwTime)
end

function OnDeath(dwTime, dwAttackedCount)
    Mob:LuaSay("I will have my revenge!", 100.0)
end

function OnReturnHome(dwTime, dwAttackedCount)
    Mob:LuaSay("You will not defeat me!", 100.0)
	
    bOnAttacked = 0
    dwNextCreateTime = 0
    bMobSay = 0
    bMobCreate = 0
end

function OnMoveEnd(dwTime)
end

function WhileCombat(dwTime, dwHPPercent, dwAttackedCount)
    Mob:LuaRecover(1, 0.1)

    if (dwHPPercent > 20) then
        nResult = math.random(1, 20)
		
        if (nResult == 1) then		
            byJob = math.random(0, 5)
            Mob:LuaSay("Leave my presence at once!", 30.0)		
            Mob:LuaResetAggro()
            Mob:LuaRecallUser(byJob, 30.0, 73, 40.077, 4.683, 59.303)
        end
    end
end
```