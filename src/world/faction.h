/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2010 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the AscentNG Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the AscentNG is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef __FACTION_H
#define __FACTION_H

//#include "../Data/Unit.h"
#include "Unit.h"
#define NUM_SANCTUARIES 2
static const uint32 SANCTUARY_ZONES[NUM_SANCTUARIES] = { 3703, 4395 }; //Shatrath city, Dalaran

SERVER_DECL bool isHostile(Object* objA, Object* objB); // B is hostile for A?
SERVER_DECL bool isAttackable(Object* objA, Object* objB, bool CheckStealth = true); // A can attack B?
SERVER_DECL bool isCombatSupport(Object* objA, Object* objB); // B combat supports A?;
SERVER_DECL bool isAlliance(Object* objA); // A is alliance?

//////////////////////////////////////////////////////////////////
//bool isNeutral( Object *a, Object *b )
//  Tells if 2 Objects are neutral to each others based on
//  their faction.
//
//Parameters
//  Object *a  -  Pointer to an Object
//  Object *b  -  Pointer to an Object
//
//Return Value
//  Returns true if they are neutral.
//  Returns false otherwise.
//
/////////////////////////////////////////////////////////////////
SERVER_DECL bool isNeutral(Object* a, Object* b);

ARCEMU_INLINE bool isFriendly(Object* objA, Object* objB)// B is friendly to A if its not hostile
{
	if(!(objA->m_phase & objB->m_phase))	//We have to return prematurely, because isHostile would return false (phase difference!!!), and it would result in a true return value here.
		return false;			//We must do this, as it affects AoE spell targets, thus required for them to function properly (so you won't heal out of phase friends...).
	return !isHostile(objA, objB);
}

ARCEMU_INLINE bool isSameFaction(Object* objA, Object* objB)
{
	// shouldn't be necessary but still
	if( objA->m_faction == NULL || objB->m_faction == NULL )
		return false;

	return (objB->m_faction->Faction == objA->m_faction->Faction);
}
ARCEMU_INLINE  Player* GetPlayerFromObject(Object* obj); 
#endif
