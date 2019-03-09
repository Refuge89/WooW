/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org>
 * Copyright (C) 2009-2012 ArcEmu Team <http://www.arcemu.org>
 * Copyright (C) 2007-2008 Moon++ Team <http://moonplusplus.info>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include "StdAfx.h"
#include "Setup.h"
#include "../Common/EasyFunctions.h"
#include "../Common/Base.h"

bool PoweringOurDefenses(uint32 i, Spell * pSpell)
{
	if(!pSpell->u_caster->IsPlayer())
    return true;

	Player * plr = TO_PLAYER(pSpell->u_caster);
	if( !plr )
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry( 8490 );
	if( !qle )
		return true;

	// Angelis : Need to script the scourge attack

	if( qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0] )
	{
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
	return true;
}

///////////////////////////////////////////////////////
//Quest: The Drwarfen Spy
//ID: 8486

// Anvilward say
#define ANVILWARD_SAY_1 "Very well. Let's see what you have to show me."
#define ANVILWARD_SAY_2 "What manner of trick is this, blood elf? If you seek to ambush me, I warn you I will not go down quietly!"

enum eGossipTexts
{
    ANVILWARD_1 = 8239,
    ANVILWARD_2 = 8240,
};

class SCRIPT_DECL ProspectorAnvilwardGossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* Plr);
    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* EnteredCode);
    void GossipEnd(Object* pObject, Player* Plr) { Plr->CloseGossip(); }
};

void ProspectorAnvilwardGossip::GossipHello(Object* pObject, Player * Plr)
{
    GossipMenu* Menu;

    Creature* pCreature = static_cast<Creature*>(pObject);
    if (pCreature == NULL)
        return;

    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), ANVILWARD_1, Plr);

    if (Plr->HasQuest(8483))
    //  Menu->AddItem(ICON_CHAT, Plr->GetSession()->LocalizedGossipOption(460), 1);     // I need a moment of your time, Sir.
		Menu->AddItem(0, "I need a moment of your time, Sir", 1);

    Menu->SendTo(Plr);
}

void ProspectorAnvilwardGossip::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
    GossipMenu* Menu;
//  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 8240, Plr);
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), ANVILWARD_2, Plr);

    if(!pObject->IsCreature())
        return;

    Creature* pCreature = static_cast<Creature*>(pObject);

    switch (IntId)
    {
		case 0:
		{
			pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me!");

			GossipEnd(pObject, Plr);
			pCreature->GetAIInterface()->StopMovement(1);
			pCreature->GetAIInterface()->setMoveType(WALK);
		}break;
        case 1:
        {
        //  Menu->AddItem(ICON_CHAT, Plr->GetSession()->LocalizedGossipOption(461), 2);     // Why... yes, of course. I've something to show you right inside this building. Mr. Anvilward.
			Menu->AddItem(0, "Why... yes, of course. I've something to show you right inside this building. Mr. Anvilward.", 2);
            Menu->SendTo(Plr);
        }break;
        case 2:
        {
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, ANVILWARD_SAY_1);
            Plr->Gossip_Complete();
      //    pCreature->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHENSTOP);
            pCreature->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
            //Every Gossip NPC has a "StopMovement(30000)" by default.... lets overwrite it.
            pCreature->GetAIInterface()->StopMovement(10);
        }break;
    }
};

class ProspectorAnvilward : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ProspectorAnvilward);
    ProspectorAnvilward(Creature* pCreature) : CreatureAIScript(pCreature)
    {
        pCreature->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
    }

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
        if (iWaypointId == 9)
        {
            _unit->SetFaction(38);
            _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
            _unit->Despawn(10 * 60 * 1000, 1000); //if failed allow other players to do quest from beggining
            _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, ANVILWARD_SAY_2);
       //   _unit->GetAIInterface()->setMoveType(Flag_Run);
            _unit->GetAIInterface()->setMoveType(RUN);
            _unit->GetAIInterface()->getNextTarget();
        }
        if (iWaypointId == 10)
        {
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
        }
    }

	void AIUpdate()
	{
		if(!_unit->CombatStatus.IsInCombat())
		{
			RemoveAIUpdateEvent();
			_unit->GetAIInterface()->SetWaypointMap(_unit->m_custom_waypoint_map);
		}
	}

	void OnDied(Unit* mKiller)
	{
		RemoveAIUpdateEvent();
		if(_unit->GetAIInterface()->GetWaypointMap() != NULL)
			_unit->GetAIInterface()->SetWaypointMap(NULL);
		else if(_unit->m_custom_waypoint_map != NULL)
		{
			for(WayPointMap::iterator itr = _unit->m_custom_waypoint_map->begin(); itr != _unit->m_custom_waypoint_map->end(); ++itr)
				delete(*itr);
			delete _unit->m_custom_waypoint_map;
		}
		_unit->m_custom_waypoint_map = NULL;

		_unit->SetFaction(35);
	}
};

void SetupEversongWoods(ScriptMgr* mgr)
{
	mgr->register_dummy_spell(28247, &PoweringOurDefenses);

    mgr->register_gossip_script(15420, new ProspectorAnvilwardGossip);
    mgr->register_creature_script(15420, ProspectorAnvilward::Create);
}
