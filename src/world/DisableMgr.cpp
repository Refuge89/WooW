/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"

namespace DisableMgr
{

namespace
{
    struct DisableData
    {
        uint8 flags;
        std::set<uint32> params[2];                             // params0, params1
    };

    // single disables here with optional data
    typedef std::map<uint32, DisableData> DisableTypeMap;
    // global disable map by source
    typedef std::map<DisableType, DisableTypeMap> DisableMap;

    DisableMap m_DisableMap;

    uint8 MAX_DISABLE_TYPES = 8;
}

void LoadDisables()
{
    uint32 oldMSTime = getMSTime();

    // reload case
    for (DisableMap::iterator itr = m_DisableMap.begin(); itr != m_DisableMap.end(); ++itr)
        itr->second.clear();

    m_DisableMap.clear();

    QueryResult* result = WorldDatabase.Query("SELECT sourceType, entry, flags, params_0, params_1 FROM disables");

    uint32 total_count = 0;

    if (!result)
    {
        sLog.outDetail("server.loading", ">> Loaded 0 disables. DB table `disables` is empty!");
        return;
    }

    Field* fields;
    do
    {
        fields = result->Fetch();
        DisableType type = DisableType(fields[0].GetUInt32());
        if (type >= MAX_DISABLE_TYPES)
        {
            sLog.outError("sql.sql", "Invalid type %u specified in `disables` table, skipped.", type);
            continue;
        }

        uint32 entry = fields[1].GetUInt32();
        uint8 flags = fields[2].GetUInt8();
        std::string params_0 = fields[3].GetString();
        std::string params_1 = fields[4].GetString();

        DisableData data;
        data.flags = flags;

        switch (type)
        {
            case DISABLE_TYPE_SPELL:
                if (!( dbcSpell.LookupEntryForced(entry) || flags & SPELL_DISABLE_DEPRECATED_SPELL))
                {
                    sLog.outError("sql.sql", "Spell entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }

                if (!flags || flags > MAX_SPELL_DISABLE_TYPE)
                {
                    sLog.outError("sql.sql", "Disable flags for spell %u are invalid, skipped.", entry);
                    continue;
                }

                if (flags & SPELL_DISABLE_MAP)
                {
                    Tokens tokens(params_0, ',');
                    for (uint8 i = 0; i < tokens.size(); )
                        data.params[0].insert(atoi(tokens[++i]));
                }

                if (flags & SPELL_DISABLE_AREA)
                {
                    Tokens tokens(params_1, ',');
                    for (uint8 i = 0; i < tokens.size(); )
                        data.params[1].insert(atoi(tokens[i++]));
                }

                break;
            // checked later
            case DISABLE_TYPE_QUEST:
                break;
            case DISABLE_TYPE_MAP:
            {
                MapEntry const* mapEntry = dbcMap.LookupEntry(entry);
                if (!mapEntry)
                {
                    sLog.outError("sql.sql", "Map entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                bool isFlagInvalid = false;
                switch (mapEntry->map_type)
                {
                    case MAP_COMMON:
                        if (flags)
                            isFlagInvalid = true;
                        break;
                    case MAP_INSTANCE:
                    case MAP_RAID:
                        if (flags & MODE_HEROIC && !GetMapDifficultyData(entry, MODE_HEROIC))
                            flags -= MODE_HEROIC;
                        if (flags & MODE_HEROIC_10MEN && !GetMapDifficultyData(entry, MODE_HEROIC_10MEN))
                            flags -= MODE_HEROIC_10MEN;
                        if (flags & MODE_HEROIC_25MEN && !GetMapDifficultyData(entry, MODE_HEROIC_25MEN))
                            flags -= MODE_HEROIC_25MEN;
                        if (!flags)
                            isFlagInvalid = true;
                        break;
                    case MAP_BATTLEGROUND:
                    case MAP_ARENA:
                        sLog.outError("sql.sql", "Battleground map %u specified to be disabled in map case, skipped.", entry);
                        continue;
                }
                if (isFlagInvalid)
                {
                    sLog.outError("sql.sql", "Disable flags for map %u are invalid, skipped.", entry);
                    continue;
                }
                break;
            }
/*            case DISABLE_TYPE_BATTLEGROUND:
                if (!sBattlemasterListStore.LookupEntry(entry))
                {
                    sLog.outError("sql.sql", "Battleground entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                if (flags)
                    sLog.outError("sql.sql", "Disable flags specified for battleground %u, useless data.", entry);
                break;
            case DISABLE_TYPE_OUTDOORPVP:
                if (entry > MAX_OUTDOORPVP_TYPES)
                {
                    sLog.outError("sql.sql", "OutdoorPvPTypes value %u from `disables` is invalid, skipped.", entry);
                    continue;
                }
                if (flags)
                    sLog.outError("sql.sql", "Disable flags specified for outdoor PvP %u, useless data.", entry);
                break;
            case DISABLE_TYPE_ACHIEVEMENT_CRITERIA:
                if (!sAchievementMgr->GetAchievementCriteria(entry))
                {
                    sLog.outError("sql.sql", "Achievement Criteria entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                if (flags)
                    sLog.outError("sql.sql", "Disable flags specified for Achievement Criteria %u, useless data.", entry);
                break;
*/
            case DISABLE_TYPE_VMAP:
            {
                MapEntry const* mapEntry = dbcMap.LookupEntry(entry);
                if (!mapEntry)
                {
                    sLog.outError("sql.sql", "Map entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                switch (mapEntry->map_type)
                {
                    case MAP_COMMON:
                        if (flags & VMAP::VMAP_DISABLE_AREAFLAG)
                            sLog.outDetail("misc", "Areaflag disabled for world map %u.", entry);
                        if (flags & VMAP::VMAP_DISABLE_LIQUIDSTATUS)
                            sLog.outDetail("misc", "Liquid status disabled for world map %u.", entry);
                        break;
                    case MAP_INSTANCE:
                    case MAP_RAID:
                        if (flags & VMAP::VMAP_DISABLE_HEIGHT)
                            sLog.outDetail("misc", "Height disabled for instance map %u.", entry);
                        if (flags & VMAP::VMAP_DISABLE_LOS)
                            sLog.outDetail("misc", "LoS disabled for instance map %u.", entry);
                        break;
                    case MAP_BATTLEGROUND:
                        if (flags & VMAP::VMAP_DISABLE_HEIGHT)
                            sLog.outDetail("misc", "Height disabled for battleground map %u.", entry);
                        if (flags & VMAP::VMAP_DISABLE_LOS)
                            sLog.outDetail("misc", "LoS disabled for battleground map %u.", entry);
                        break;
                    case MAP_ARENA:
                        if (flags & VMAP::VMAP_DISABLE_HEIGHT)
                            sLog.outDetail("misc", "Height disabled for arena map %u.", entry);
                        if (flags & VMAP::VMAP_DISABLE_LOS)
                            sLog.outDetail("misc", "LoS disabled for arena map %u.", entry);
                        break;
                    default:
                        break;
                }
                break;
            }
            case DISABLE_TYPE_MMAP:
            {
                MapEntry const* mapEntry = dbcMap.LookupEntry(entry);
                if (!mapEntry)
                {
                    sLog.outError("sql.sql", "Map entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                switch (mapEntry->map_type)
                {
                    case MAP_COMMON:
                        sLog.outBasic("misc", "Pathfinding disabled for world map %u.", entry);
                        break;
                    case MAP_INSTANCE:
                    case MAP_RAID:
                        sLog.outBasic("misc", "Pathfinding disabled for instance map %u.", entry);
                        break;
                    case MAP_BATTLEGROUND:
                        sLog.outBasic("misc", "Pathfinding disabled for battleground map %u.", entry);
                        break;
                    case MAP_ARENA:
                        sLog.outBasic("misc", "Pathfinding disabled for arena map %u.", entry);
                        break;
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }

        m_DisableMap[type].insert(DisableTypeMap::value_type(entry, data));
        ++total_count;
    }
    while (result->NextRow());

    sLog.outBasic("server.loading", ">> Loaded %u disables in %u ms", total_count, GetMSTimeDiffToNow(oldMSTime));
}

void CheckQuestDisables()
{
    uint32 oldMSTime = getMSTime();

    uint32 count = m_DisableMap[DISABLE_TYPE_QUEST].size();
    if (!count)
    {
        sLog.outBasic("server.loading", ">> Checked 0 quest disables.");
        return;
    }

    // check only quests, rest already done at startup
    for (DisableTypeMap::iterator itr = m_DisableMap[DISABLE_TYPE_QUEST].begin(); itr != m_DisableMap[DISABLE_TYPE_QUEST].end();)
    {
        const uint32 entry = itr->first;
        if (!objmgr.GetQuestTemplate(entry))
        {
            sLog.outError("sql.sql", "Quest entry %u from `disables` doesn't exist, skipped.", entry);
            m_DisableMap[DISABLE_TYPE_QUEST].erase(itr++);
            continue;
        }
        if (itr->second.flags)
            sLog.outError("sql.sql", "Disable flags specified for quest %u, useless data.", entry);
        ++itr;
    }

    sLog.outBasic("server.loading", ">> Checked %u quest disables in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool IsDisabledFor(DisableType type, uint32 entry, Unit * unit, uint8 flags)
{
    ASSERT(type < MAX_DISABLE_TYPES);
    if (m_DisableMap[type].empty())
        return false;

    DisableTypeMap::iterator itr = m_DisableMap[type].find(entry);
    if (itr == m_DisableMap[type].end())    // not disabled
        return false;

    switch (type)
    {
        case DISABLE_TYPE_SPELL:
        {
            uint8 spellFlags = itr->second.flags;
            if (unit)
            {
                if ((spellFlags & SPELL_DISABLE_PLAYER && unit->GetTypeId() == TYPEID_PLAYER) ||
                    (unit->GetTypeId() == TYPEID_UNIT && ((unit->IsPet() && spellFlags & SPELL_DISABLE_PET) || spellFlags & SPELL_DISABLE_CREATURE)))
                {
                    if (spellFlags & SPELL_DISABLE_MAP)
                    {
                        std::set<uint32> const& mapIds = itr->second.params[0];
                        if (mapIds.find(unit->GetMapId()) != mapIds.end())
                            return true;                                        // Spell is disabled on current map

                        if (!(spellFlags & SPELL_DISABLE_AREA))
                            return false;                                       // Spell is disabled on another map, but not this one, return false

                        // Spell is disabled in an area, but not explicitly our current mapId. Continue processing.
                    }

                    if (spellFlags & SPELL_DISABLE_AREA)
                    {
                        std::set<uint32> & areaIds = itr->second.params[1];
                        if (areaIds.find(unit->GetMapMgr()->GetAreaID(unit->GetPositionX(),unit->GetPositionY())) != areaIds.end())
                            return true;                                        // Spell is disabled in this area
                        return false;                                           // Spell is disabled in another area, but not this one, return false
                    }
                    else
                        return true;                                            // Spell disabled for all maps
                }

                return false;
            }
            else if (spellFlags & SPELL_DISABLE_DEPRECATED_SPELL)    // call not from spellcast
                return true;
            else if (flags & SPELL_DISABLE_LOS)
                return (spellFlags & SPELL_DISABLE_LOS) != 0;

            break;
        }
        case DISABLE_TYPE_MAP:
            if (Player * player = TO_PLAYER(unit))
            {
                MapEntry * mapEntry = dbcMap.LookupEntry(entry);
                if (mapEntry->IsDungeon())
                {
                    uint8 disabledModes = itr->second.flags;
                    Difficulty targetDifficulty = player->GetDifficulty(mapEntry->IsRaid());
              //      sSpellFactoryMgr.GetSpellEntryByDifficulty(entry, targetDifficulty);
                    GetDownscaledMapDifficultyData(entry, targetDifficulty);
                    switch (targetDifficulty)
                    {
                        case MODE_NORMAL:
                            return (disabledModes & MODE_NORMAL) != 0;
                        case MODE_HEROIC:
                            return (disabledModes & MODE_HEROIC) != 0;
                        case MODE_HEROIC_10MEN:
                            return (disabledModes & MODE_HEROIC_10MEN) != 0;
                        case MODE_HEROIC_25MEN:
                            return (disabledModes & MODE_HEROIC_25MEN) != 0;
                    }
                }
                else if (mapEntry->map_type == MAP_COMMON)
                    return true;
            }
            return false;
        case DISABLE_TYPE_QUEST:
            if (!unit)
                return true;
            if (Player const* player = TO_PLAYER(unit))
                if (player->GetSession()->HasGMPermissions())
                    return false;
            return true;
        case DISABLE_TYPE_BATTLEGROUND:
        case DISABLE_TYPE_OUTDOORPVP:
        case DISABLE_TYPE_ACHIEVEMENT_CRITERIA:
        case DISABLE_TYPE_MMAP:
            return true;
        case DISABLE_TYPE_VMAP:
           return (flags & itr->second.flags) != 0;
    }

    return false;
}

bool IsVMAPDisabledFor(uint32 entry, uint8 flags)
{
    return IsDisabledFor(DISABLE_TYPE_VMAP, entry, NULL, flags);
}
/*
bool IsPathfindingEnabled(uint32 mapId)
{
    return sWorld->getBoolConfig(CONFIG_ENABLE_MMAPS)
        && !IsDisabledFor(DISABLE_TYPE_MMAP, mapId, NULL, MMAP_DISABLE_PATHFINDING);
}
*/
} // Namespace