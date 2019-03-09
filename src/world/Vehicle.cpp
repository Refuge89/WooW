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

#include "StdAfx.h"

Vehicle::Vehicle(uint64 guid) : Creature(guid)
{
	m_controllingUnit = nullptr;
	m_vehicleEntry = 0;
	m_passengerCount = 0;
	m_maxPassengers = 0;
	m_seatSlotMax = 0;
	memset( m_vehicleSeats, 0, sizeof(uint32)*8 );
	m_isVehicle = true;
	Initialised = false;
	m_CreatedFromSpell = false;
	m_mountSpell = 0;
	for(uint8 i = 0; i < 8; ++i)
	{
		m_vehicleSeats[i] = nullptr;
		m_passengers[i] = nullptr;
	}
}

Vehicle::~Vehicle()
{
	RemoveAllPassengers();

	m_passengerCount = 0;
	if( IsInWorld() )
		RemoveFromWorld(false, true);
}

void Vehicle::InitSeats(uint32 vehicleEntry, Unit* pRider)
{
	m_maxPassengers = 0;
	m_seatSlotMax = 0;

	SetVehicleEntry(vehicleEntry);

	vehicleData = dbcVehicle.LookupEntry( vehicleEntry );
	if(!vehicleData)
	{
		Log.Warning("Vehicles", "Attempted to create vehicle %u with invalid vehicle_entry, check your creature_proto table.\n", proto->Id);
		return;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( vehicleData->seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( vehicleData->seatID[i] );
			m_seatSlotMax = i + 1; // i plus 1 so that we can count 0. Plus, no vehicle has 0 seats, so this ensures 1.
			if(m_vehicleSeats && m_vehicleSeats[i]->IsUsable())
				++m_maxPassengers;
		}
	}

	Initialised = true;

	if( pRider != nullptr )
		AddPassenger( pRider );
}

void Vehicle::Load(CreatureProto * proto_, MapMgr* mgr, float x, float y, float z, float o /* = 0.0f */)
{
	proto = proto_;
	if(!proto)
		return;

	InitSeats(proto->vehicleid, m_controllingUnit);

	Creature::Load(proto_, mgr, x, y, z, o);
}

bool Vehicle::Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info)
{
	proto = CreatureProtoStorage.LookupEntry(spawn->entry);
	if(!proto)
		return false;

	InitSeats(proto->vehicleid, m_controllingUnit);

	return Creature::Load(spawn, mode, info);
}

void Vehicle::OnPushToWorld()
{
	if(proto->vehicleid == 156)
	{
		vehicleData->powerType = POWER_TYPE_MANA;
	}
	else
		ChangePowerType();
}

//-----------------------------------------------------------------
//This function builds and send the packets required to add spells
//to a vehicle that can be cast by its controlling unit
//-----------------------------------------------------------------
void Vehicle::SendSpells(uint32 entry, Player* plr)
{
	CreatureProto* acc = CreatureProtoStorage.LookupEntry(GetEntry());

	if(!acc)
		return;

	std::vector<uint32>::iterator itr = acc->castable_spells.begin();

	WorldPacket data(SMSG_PET_SPELLS, 78); // uint32 = 4; (10 * 6) + 4 + 4 + 2 + 8 = 78
	data << GetGUID();
	data << uint16(0);
	data << uint32(0);
	data << uint32(0x00000101);

	// Send the actionbar
	for(uint8 i = 0; i < MAX_CREATURE_PROTO_SPELLS; ++i)
	{
		for(;itr != acc->castable_spells.end();)
		{
			SpellEntry const *sp = dbcSpell.LookupEntry( *itr );
			if (!sp)
				continue;

			if(sp->Attributes & ATTRIBUTES_PASSIVE)
			{
				CastSpell(GetGUID(), (*itr), true);
				++itr;
			}			
			else if( ( sp->Attributes & ATTRIBUTES_PASSIVE ) == 0 ) // send origional spells
			{
				data << uint16(*itr) << uint8(0) << uint8(i+8);
				++itr;
			}
		}
	}
	for(uint8 i = MAX_CREATURE_PROTO_SPELLS; i < 10; ++i)
	{
		data << uint16(0) << uint8(0) << uint8(i+8);
	}

	data << uint8(0); // spells count
	data << uint8(0); // cooldowns count

	plr->GetSession()->SendPacket(&data);
}

void Vehicle::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(this, &Vehicle::Despawn, (uint32)0, respawntime, EVENT_CREATURE_RESPAWN, delay, 1,0);
		return;
	}

	if(!IsInWorld())
		return;

	if(respawntime)
	{
		MapCell * pCell = m_mapMgr->GetCellByCoords(m_spawnLocation.x, m_spawnLocation.y);
		if(pCell == nullptr)
			pCell = GetMapCell();

		ARCEMU_ASSERT(pCell != nullptr);
		pCell->_respawnObjects.insert(this);
		sEventMgr.RemoveEvents(this);

		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnVehicle, this, pCell , EVENT_VEHICLE_RESPAWN, respawntime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		Unit::RemoveFromWorld(false);
		m_position = m_spawnLocation;
		m_respawnCell = pCell;

	}
	else
	{
		Unit::RemoveFromWorld(true);
		SafeDelete();
	}
}


void Vehicle::Update(uint32 time)
{
	Creature::Update(time);
}

void Vehicle::SafeDelete()
{
	for(int i = 0; i < m_seatSlotMax; ++i)
    {
        if(m_passengers[i] != nullptr)
        {
            if(m_passengers[i]->IsPlayer())
                // Remove any passengers
                RemovePassenger(m_passengers[i]);
            else
                delete m_passengers[i];
        }
    }

	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(this, &Vehicle::DeleteMe, EVENT_CREATURE_SAFE_DELETE, 1000, 1, EVENT_FLAG_DELETES_OBJECT);
}

void Vehicle::DeleteMe()
{
	if(IsInWorld())
		RemoveFromWorld(false, true);

	delete this;
}

void Vehicle::AddPassenger(Unit* pPassenger)
{
	if(!m_maxPassengers || !m_seatSlotMax) //how is this happening?
	{
		sLog.outError("Vehicle was not correctly initialised, retrying");
		InitSeats(m_vehicleEntry, m_controllingUnit);
	}

	OUT_DEBUG("AddPassenger1: Max Vehicle Slot: %u, Max Passengers: %u", m_seatSlotMax, m_maxPassengers);

	if(pPassenger->m_CurrentVehicle)
		pPassenger->m_CurrentVehicle->RemovePassenger(pPassenger);

	// Find an available seat
	for(uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if(pPassenger->IsPlayer())
		{
			if(!m_passengers[i] && m_vehicleSeats[i] && (m_vehicleSeats[i]->IsUsable() == true)) // Found a slot
			{
				_AddToSlot(pPassenger, i );
				break;
			}
		}
		else
		{
			if(!m_passengers[i] && m_vehicleSeats[i])
			{
				_AddToSlot(pPassenger, i );
				break;
			}
		}
	}
}

void Vehicle::AddPassenger(Unit* pPassenger, uint8 requestedseat, bool force /*= false*/)
{
	// Look at how fancy we are, we get to request a slot for ourselves!
	if(!m_maxPassengers || !m_seatSlotMax) //how is this happening?
	{
		sLog.outError("Vehicle was not correctly initialised, retrying");
		InitSeats(m_vehicleEntry, m_controllingUnit);
	}

	if(pPassenger->m_CurrentVehicle)
		pPassenger->m_CurrentVehicle->RemovePassenger(pPassenger);

	// Player's client sets these
	if( pPassenger->IsCreature() ){
		pPassenger->transporter_info.guid = GetGUID();
		pPassenger->transporter_info.seat = requestedseat;
	}

	OUT_DEBUG("AddPassenger2: Max Vehicle Slot: %u, Max Passengers: %u\n", m_seatSlotMax, m_maxPassengers);

	if(requestedseat > 0)
	{
		if(force)
		{
			if(m_vehicleSeats[requestedseat] && m_vehicleSeats[requestedseat]->IsUsable() == true) // Slot available?
			{
				if(m_passengers[requestedseat])
					RemovePassenger(m_passengers[requestedseat]);

				_AddToSlot(pPassenger, requestedseat);
				return;
			}
		}
		else
		{
			if(!m_passengers[requestedseat] && m_vehicleSeats[requestedseat] && m_vehicleSeats[requestedseat]->IsUsable() == true) // Slot available?
			{
				_AddToSlot(pPassenger, requestedseat );
				return;
			}
		}
	}
	else
	{ // Find us a slot!
		for(uint8 i = 0; i < m_seatSlotMax; ++i)
		{
			if(pPassenger->IsPlayer())
			{
				if(!m_passengers[i] && m_vehicleSeats[i] && (m_vehicleSeats[i]->IsUsable() == true)) // Found a slot
				{
					_AddToSlot(pPassenger, i );
					break;
				}
			}
			else
			{
				if(!m_passengers[i] && m_vehicleSeats[i])
				{
					_AddToSlot(pPassenger, i );
					break;
				}
			}
		}
	}
}

uint8 Vehicle::GetPassengerSlot(Unit* pPassenger)
{
	for(uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if( m_passengers[i] == pPassenger ) // Found a slot
		{
			return i;
			break;
		}
	}
	return 0xFF;
}

void Vehicle::RemovePassenger(Unit* pPassenger)
{
	if(!pPassenger) // We have enough problems that we need to do this :(
		return;

	//vehicle
	uint8 slot = pPassenger->m_inVehicleSeatId;
	pPassenger->m_CurrentVehicle = nullptr;
	pPassenger->m_inVehicleSeatId = 0xFF;

	if(slot >= 8)
		return;

	//Remove the flag which was stopping units
	//from attacking us
//	pPassenger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	pPassenger->RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_NO_REAGANT_COST | UNIT_FLAG_NOT_SELECTABLE));

	//If the vehicle was created from a mount
	//spell, remove the aura
	if( pPassenger->IsPlayer() )
		pPassenger->RemoveAura(static_cast<Player*>(pPassenger)->m_MountSpellId);

	//If a spell was cast on us when we boarded
	//the vehicle, we must make sure it is removed
	if( m_mountSpell )
		pPassenger->RemoveAura( m_mountSpell );

	WorldPacket data(MSG_MOVE_HEARTBEAT, 48);
	data << pPassenger->GetNewGUID();
	data << uint32(MOVEFLAG_ROOTED); // MOVEFLAG_FLYING
	data << uint16(0x40);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << uint32(0);
	pPassenger->SendMessageToSet(&data, false);

	if(pPassenger->IsPlayer())
	{
		Player* plr = static_cast<Player*>(pPassenger);

		if(plr == GetControllingUnit())
		{
			plr->m_CurrentCharm = nullptr;
			data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data << GetNewGUID() << (uint8)0;
			plr->GetSession()->SendPacket(&data);

			// send null spells if needed
			static_cast<Player*>(pPassenger)->SendEmptyPetSpellList();
		}

		RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_PLAYER_CONTROLLED_CREATURE | UNIT_FLAG_PVP_ATTACKABLE));


		plr->SetPlayerStatus(TRANSFER_PENDING);

		//Set the new coordinates for the player
		//server side, so that we don't go out of sync
		plr->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());
		plr->SetPosition(GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());

		//Build a mighty packet which tells the
		//client we have left the vehicle and
		//we have teleported off it.
		SendMsgMoveTeleportAck(data, plr);

		//Clear the far sight field, this
		//will set our vision back to the
		//normal player radius
		plr->SetUInt64Value( PLAYER_FARSIGHT, 0 );	

		// if we are on a flying vehicle, add a parachute!
		if(this->HasAuraWithName(SPELL_AURA_ENABLE_FLIGHT) || this->HasAuraWithName(SPELL_AURA_ENABLE_FLIGHT2))
			pPassenger->CastSpell(pPassenger, 45472, false);

		//Play the sound to the player
		//when they exit the vehicle
		data.Initialize(SMSG_PET_DISMISS_SOUND);
		data << uint32(m_vehicleSeats[slot]->exitUISoundID);
		data << plr->GetPosition();
		plr->GetSession()->SendPacket(&data);

		//Send an empty spells packet to
		//clear the action bar and give the
		//player their normal bar back
		data.Initialize(SMSG_PET_SPELLS);
		data << uint64(0);
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);
	}

	//Handle any work that needs to be done
	//if the controlling unit left, slot 0
	//denotes the passenger that controls,
	//moves, and casts spells
	if(slot == 0)
	{
		//Clear the redirection of spell cast
		//results, and send them to the vehicle
		//again, now that it is vacated the player
		//doesn't want to get these
		m_redirectSpellPackets = nullptr;

		//Remove any attackers for now
		//so that there aren't any faction
		//conflicts, we will set the correct
		//faction later
		CombatStatus.Vanished();

		//Reset the current charm field
		pPassenger->SetUInt64Value( UNIT_FIELD_CHARM, 0 );
		SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);

		//Set the faction back to the vehicles
		//original faction
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetCharmTempVal());
		
		//Now that we have set the correct
		//faction we can update it and allow
		//units to attack.
		_setFaction();
		UpdateOppFactionSet();

		//Clear out any attackers and targets
		//that we had before the controlling
		//unit left and give the vehicle its
		//brain back!
		GetAIInterface()->WipeHateList();
		GetAIInterface()->WipeTargetList();
		EnableAI();

		SetControllingUnit(nullptr);

		RemoveAllPassengers();
	}

	m_passengers[slot] = nullptr;

	//note: this is not blizz like we should despawn
	//and respawn at spawn point.
	//Well actually this is how blizz wanted it
	//but they couldnt get it to work xD
	bool haspassengers = false;
	for(uint8 i = 0; i < m_seatSlotMax; i++)
	{
		if(m_passengers[i] != nullptr)
		{
			haspassengers = true;
			break;
		}
	}


	
	if(!haspassengers && !m_CurrentVehicle) // Passenger and accessory checks.
	{
		if( m_spawn != nullptr)
		{
		/*	if(m_spawn->vehicle == 123 | m_spawn->vehicle == 200 )
			{
				if (pPassenger->HasAura(52280))
				pPassenger->RemoveAura(52280);
				else
				pPassenger->RemoveAura(52268);
			}
			if(m_spawn->vehicle == 123 | m_spawn->vehicle == 200 )
			{
				Unroot();
				GetAIInterface()->MoveTo(m_spawn->x, m_spawn->y, m_spawn->z, m_spawn->o);
			//	SetSpeed(RUN, m_runSpeed);
			}
			else */ if(m_spawn->vehicle == 79)
			{
				Unroot();
				GetAIInterface()->MoveTo(m_spawn->x, m_spawn->y, m_spawn->z, m_spawn->o);
				sEventMgr.AddEvent(TO_UNIT(this), &Creature::Root, EVENT_CREATURE_UPDATE, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			}else
				Unroot();
		//	else 
		//	GetAIInterface()->MoveTo(m_spawn->x, m_spawn->y, m_spawn->z, m_spawn->o);
		}
        else //we're a temp spawn
		//	Unit::RemoveFromWorld( true);
            SafeDelete();
	}

	pPassenger->Unroot();

	// We need to null this out, its changed automatically.

	// We need to null this out, its changed automatically.
//	if(m_TransporterGUID) // Vehicle has a transport guid?
//		TO_PLAYER(pPassenger)->m_TransporterGUID = m_TransporterGUID;
//	else
//		TO_PLAYER(pPassenger)->m_TransporterGUID = nullptr;

	if(pPassenger->IsPlayer())
		--m_passengerCount;

	if(!IsFull())
	{
		if(this->IsPlayer())
			this->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
		else
			this->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
	}

	if( pPassenger->IsPlayer() )
		static_cast< Player* >( pPassenger )->SpawnActivePet();

	if( pPassenger->IsCreature() ){
		Creature *c = static_cast< Creature* >( pPassenger );

		if( c->GetScript() != nullptr ){
			c->GetScript()->OnExitVehicle();
		}
	}
	if( this->IsCreature() ){
		Creature *c = static_cast< Creature* >( this );

		if( c->GetScript() != nullptr ){
			if( m_passengerCount == 0 )
				c->GetScript()->OnLastPassengerLeft( pPassenger );
		}else{
			// The passenger summoned the vehicle, and we have no script to remove it, so we remove it here
			if( ( m_passengerCount == 0 ) && ( c->GetSummonedByGUID() == pPassenger->GetGUID() ) )
				c->Despawn( 1 * 1000, 0 );
		}
	}
}

//-----------------------------------------------------------------
// This function checks if the vehicle has the passenger aboard
// that is passed as a parameter
//-----------------------------------------------------------------
bool Vehicle::HasPassenger(Unit* pPassenger)
{
	//Iterate over all possible slots in the vehicle
	for(uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		//If we find a slot that matches the pointer
		//to the passenger we were passed then return
		//true
		if( m_passengers[i] == pPassenger )
			return true;
	}
	//If we hit this then it means the passenger
	//is not in this vehicle
	return false;
}

void Vehicle::_AddToSlot(Unit* pPassenger, uint8 slot)
{
	if(slot >= 8 || slot > m_maxPassengers)
		return;

	if(!m_vehicleSeats[slot] || !GetAIInterface())
		return;

	if(m_passengers[slot] != nullptr)
	{
		RemovePassenger(m_passengers[slot]);
	}

	pPassenger->RemoveAllAuraType( SPELL_AURA_MOUNTED );

	if( pPassenger->IsPlayer() )
		static_cast< Player* >( pPassenger )->DismissActivePets();

	if(pPassenger->m_CurrentVehicle != nullptr)
		pPassenger->m_CurrentVehicle->RemovePassenger(pPassenger);

	m_passengers[ slot ] = pPassenger;

	pPassenger->m_inVehicleSeatId = slot;

	if( m_mountSpell )
		pPassenger->CastSpell( pPassenger, m_mountSpell, true );

	pPassenger->m_CurrentVehicle = this;

	if(slot == 0)
	{
		SetControllingUnit(pPassenger);
		CombatStatus.Vanished();
	}

	pPassenger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);;

	// Player's client sets these
	if(pPassenger->IsCreature())
	{
		pPassenger->transporter_info.guid = GetGUID();
		pPassenger->transporter_info.seat = slot;
	}

	if( pPassenger->IsPlayer() )
	{
		Player* pPlayer = static_cast<Player*>(pPassenger);
		if(pPlayer->m_MountSpellId)
			pPlayer->RemoveAura(pPlayer->m_MountSpellId);

		//Remove morph spells
		if(pPlayer->GetUInt32Value(UNIT_FIELD_DISPLAYID)!= pPlayer->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
		{
			pPlayer->RemoveAllAuraType(SPELL_AURA_TRANSFORM);
			pPlayer->RemoveAllAuraType(SPELL_AURA_MOD_SHAPESHIFT);
		}

		if(pPlayer->GetSummon())
		{
			if(pPlayer->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
				pPlayer->GetSummon()->Dismiss();
			else
				pPlayer->GetSummon()->Remove( true, true);
		}

		if(slot == 0)
		{
			DisableAI();
			m_redirectSpellPackets = pPlayer;
			pPlayer->SetUInt64Value(PLAYER_FARSIGHT, GetGUID());

			SetSpeed(RUN, m_runSpeed);
			SetSpeed(FLY, m_flySpeed);

			WorldPacket data2(SMSG_CLIENT_CONTROL_UPDATE, 10);
			data2 << GetNewGUID() << uint8(1);
			pPlayer->GetSession()->SendPacket(&data2);

			pPlayer->m_CurrentCharm = this;
			pPlayer->SetUInt64Value(UNIT_FIELD_CHARM, GetGUID());

			SetCharmTempVal(GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt64Value(UNIT_FIELD_CHARMEDBY, pPlayer->GetGUID());

			SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

			_setFaction();
			UpdateOppFactionSet();

			SendSpells(GetEntry(), pPlayer);
		}
		SendMonsterMoveTransport(pPassenger, slot);
	}
//	SendMonsterMoveTransport(pPassenger, slot);

	if(IsFull())
	{
		this->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
		this->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
	}

	if( pPassenger->IsCreature() ){
		Creature *c = static_cast< Creature* >( pPassenger );
		
		if( c->GetScript() != nullptr ){
			c->GetScript()->OnEnterVehicle();
		}
	}

	if( this->IsCreature() ){
		Creature *c = static_cast< Creature* >( this );

		if( c->GetScript() != nullptr ){
			if( m_passengerCount == 1 )
				c->GetScript()->OnFirstPassengerEntered( pPassenger );

			if( !IsFull() )
				c->GetScript()->OnVehicleFull();
		}
	}
}

void Vehicle::MoveVehicle(float x, float y, float z, float o) //thanks andy
{
	SetPosition(x, y, z, o, false);
	for(uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if(m_passengers[i] != nullptr)
		{
			m_passengers[i]->SetPosition(x,y,z,o,false);
		}
	}
}

void Vehicle::setDeathState(DeathState s)
{
	Creature::setDeathState(s);

	for (uint8 i = 0; i < m_seatSlotMax; ++i)
	{
		if (m_passengers[i] != nullptr)
			RemovePassenger(m_passengers[i]);
	}

	if( s == JUST_DIED && m_CreatedFromSpell)
		SafeDelete();
}

void Vehicle::SetSpeed(PlayerSpeedType SpeedType, float value)
{
	if( value < 0.1f )
		value = 0.1f;

	WorldPacket data(SMSG_FORCE_RUN_SPEED_CHANGE, 200);

	data << GetNewGUID();
	data << uint32(0);

	switch(SpeedType)
	{
	case RUN:
		{
			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			data << uint8(1);
			m_runSpeed = value;
		}break;
	case RUNBACK:
		{
			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			m_backWalkSpeed = value;
		}break;
	case SWIM:
		{
			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			m_swimSpeed = value;
		}break;
	case SWIMBACK:
		{
			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			data << uint8(0);
			data << uint32(getMSTime());
			data << m_position.x;
			data << m_position.y;
			data << m_position.z;
			data << m_position.o;
			data << uint32(0);
			m_backSwimSpeed = value;
		}break;
	case FLY:
		{
			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = value;
		}break;
	default:
		return;
		break;
	}

	data << value;

	SendMessageToSet(&data , true);
}

void Vehicle::RemoveAllPassengers()
{
	for(uint32 i = 0; i < 8; ++i)
	{
		if(m_passengers[i] != nullptr)
			RemovePassenger(m_passengers[i]);
	}
}

void Vehicle::SendMonsterMoveTransport( Unit* pPassenger, uint8 slot )
{
	WorldPacket data(SMSG_MONSTER_MOVE_TRANSPORT, 60);
	data << pPassenger->GetNewGUID();
	data << GetNewGUID();
	data << int8(slot);
	data << uint8(0);                                       // 3.1
	data << GetPositionX() - pPassenger->GetPositionX();
	data << GetPositionY() - pPassenger->GetPositionY();
	data << GetPositionZ() - pPassenger->GetPositionZ();
	data << uint32(getMSTime());

	data << uint8(4);                                       // unknown
	data << float(0);                                       // facing angle

	data << uint32(0x800000);

	data << uint32(0);                                      // Time in between points
	data << uint32(1);										// number of way points
	data << m_vehicleSeats[slot]->attachmentOffsetX;
	data << m_vehicleSeats[slot]->attachmentOffsetY;
	data << m_vehicleSeats[slot]->attachmentOffsetZ;
	SendMessageToSet(&data, true);
}

void Vehicle::SendMsgMoveTeleportAck( WorldPacket &data, Player* plr )
{
	data.Initialize(MSG_MOVE_TELEPORT_ACK);
	data << plr->GetNewGUID();
	data << plr->m_teleportAckCounter++;
	data << uint32(MOVEFLAG_ROOTED); // MOVEFLAG_FLYING
	data << uint16(0x40);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << uint32(0);
	plr->GetSession()->SendPacket(&data);
}

void Vehicle::ChangePowerType()
{
	if(vehicleData == nullptr)
		return;

	switch(vehicleData->powerType)
	{
	case POWER_TYPE_MANA:
		{
			SetPowerType( POWER_TYPE_ENERGY );
			SetUInt32Value(UNIT_FIELD_POWER1, 100);
			SetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY, 100 );
			SetPower( POWER_TYPE_ENERGY, 100 );
		}break;
	case POWER_TYPE_ENERGY:
		{
			SetPowerType( POWER_TYPE_ENERGY );
			SetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY, 100 );
			SetPower( POWER_TYPE_ENERGY, 100 );
		}break;
	case POWER_TYPE_STEAM:
	case POWER_TYPE_HEAT:
	case POWER_TYPE_BLOOD:
	case POWER_TYPE_OOZE:
	case POWER_TYPE_WRATH:
		{
			SetPowerType( POWER_TYPE_ENERGY );
			SetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY, 100 );
			SetPower( POWER_TYPE_ENERGY, 100 );
		}break;
	case POWER_TYPE_PYRITE:
		{
			SetPowerType(POWER_TYPE_ENERGY);
			SetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY,50);
			SetPower( POWER_TYPE_ENERGY, 50 );
			m_interruptRegen = true;
		}break;
	default:
		{
			sLog.outError("Vehicle %u, Vehicle Entry %u has an unknown powertype %u.", GetEntry(), GetVehicleEntry(), vehicleData->powerType);
		}break;
	}
	Root();
}

void Vehicle::ChangeSeats(Unit* unit, uint8 seatid)
{
	if(seatid == unit->m_inVehicleSeatId)
    {
        OUT_DEBUG("Return, Matching Seats. Requsted: %u, current: %u", seatid, unit->m_inVehicleSeatId);
        return;
    }

    if(unit->m_CurrentVehicle != nullptr)
        unit->m_CurrentVehicle->RemovePassenger(unit);

//    TO_PLAYER(unit)->m_TransporterGUID = GetGUID();
    AddPassenger(unit, seatid);
}

void WorldSession::HandleVehicleDismiss(WorldPacket & recv_data)
{
	if(GetPlayer() == nullptr || GetPlayer()->m_CurrentVehicle == nullptr)
		return;

	if(recv_data.rpos() != recv_data.wpos())
		HandleMovementOpcodes(recv_data);

	GetPlayer()->m_CurrentVehicle->RemovePassenger(GetPlayer());
}

void WorldSession::HandleSpellClick( WorldPacket & recv_data )
{

	if(GetPlayer() == nullptr || GetPlayer()->m_CurrentVehicle || GetPlayer()->GetMapMgr() == nullptr)
		return;

	CHECK_PACKET_SIZE(recv_data, 8);

	uint64 guid = 0;
	recv_data >> guid;

	Vehicle* pVehicle = GetPlayer()->GetMapMgr()->GetVehicle(GET_LOWGUID_PART(guid));
	if(!pVehicle)
		return;

	if(pVehicle->HasPassenger(_player))
		pVehicle->RemovePassenger(_player);
	pVehicle->AddPassenger(_player);

	uint32 creature_id = pVehicle->GetEntry();
	uint32 cast_spell_id = 0;

	if(!_player->HasAurasWithNameHash(SPELL_HASH_LIGHTWELL_RENEW) && pVehicle->RemoveAura(59907))
	{
		SpellClickSpell *sp = SpellClickSpellStorage.LookupEntry( creature_id );
		if( sp == nullptr ){
			if( pVehicle->IsCreature() ){
				Creature *c = TO< Creature* >( pVehicle );
				
				sChatHandler.BlueSystemMessage( this, "NPC Id %u ( %s ) has no spellclick spell associated with it.", c->GetProto()->Id, c->GetCreatureInfo()->Name  );
				LOG_DETAIL("Spellclick packet received for creature %u but there is no spell associated with it.", creature_id );
				return;
			}
		}

		cast_spell_id = sp->SpellID;

		pVehicle->CastSpell(_player, cast_spell_id, true);

		if(!pVehicle->HasAura(59907))
			TO_CREATURE(pVehicle)->Despawn(0, 0); //IsCreature() check is not needed, refer to r2387 and r3230

		return;
	}
	
	SpellClickSpell *sp = SpellClickSpellStorage.LookupEntry( creature_id );
	if( sp == nullptr ){
		if( pVehicle->IsCreature() ){
			Creature *c = TO< Creature* >( pVehicle );

			sChatHandler.BlueSystemMessage( this, "NPC Id %u ( %s ) has no spellclick spell associated with it.", c->GetProto()->Id, c->GetCreatureInfo()->Name  );
			LOG_DETAIL("Spellclick packet received for creature %u but there is no spell associated with it.", creature_id );
			return;
		}
	}

	cast_spell_id = sp->SpellID;

	if(cast_spell_id == 0)
		return;

	SpellEntry* spellInfo = dbcSpell.LookupEntryForced(cast_spell_id);
	if(spellInfo == nullptr)
		return;
	Spell* spell = sSpellFactoryMgr.NewSpell(_player, spellInfo, false, nullptr);
	SpellCastTargets targets(guid);
	spell->prepare(&targets);
}

uint16 Vehicle::GetMoveFlags2() const{
    uint16 flags2 = 0;

    if (vehicleData->flags & VEHICLE_FLAG_NO_STRAFE)
        flags2 |= MOVEFLAG2_NO_STRAFING;

    if (vehicleData->flags & VEHICLE_FLAG_NO_JUMPING)
        flags2 |= MOVEFLAG2_NO_JUMPING;
    if (vehicleData->flags & VEHICLE_FLAG_FULLSPEEDTURNING)
        flags2 |= MOVEFLAG2_FULLSPEED_TURNING;
    if (vehicleData->flags & VEHICLE_FLAG_ALLOW_PITCHING)
        flags2 |= MOVEFLAG2_ALLOW_PITCHING;
    if (vehicleData->flags & VEHICLE_FLAG_FULLSPEEDPITCHING)
        flags2 |= MOVEFLAG2_FULLSPEED_PITCHING;

    return flags2;
}

void WorldSession::HandleLeaveVehicle(WorldPacket& recv_data)
{
    if (_player->m_CurrentVehicle == nullptr)
        return;

    _player->m_CurrentVehicle->RemovePassenger(_player);
}

void WorldSession::HandleEnterVehicle(WorldPacket& recv_data)
{
    uint64 guid;

    recv_data >> guid;

    Unit* v = _player->GetMapMgr()->GetUnit(guid);
    if (v == nullptr)
        return;

    if (!_player->isInRange(v, MAX_INTERACTION_RANGE))
        return;

    if (v->m_CurrentVehicle == nullptr)
        return;

    v->m_CurrentVehicle->AddPassenger(_player);
}

void WorldSession::HandleRemoveVehiclePassenger(WorldPacket& recv_data)
{
    Vehicle* v = nullptr;
    if (_player->IsVehicle())
        v = _player->m_CurrentVehicle;

    if (v == nullptr)
        return;

    uint64 guid = 0;
    recv_data >> guid;

    if (guid == 0)
        return;

    Unit* passenger = _player->GetMapMgr()->GetUnit(guid);
    if (passenger == nullptr)
        return;

    v->RemovePassenger(passenger);
}

/* This function handles the packet sent from the client when we
change a seat on a vehicle. If the seat has a unit passenger and unit
is a vehicle, we will enter the passenger.*/
void WorldSession::HandleChangeVehicleSeat( WorldPacket & recv_data )
{
	WoWGuid Vehicleguid;
	uint8 RequestedSeat;

	if(recv_data.GetOpcode() == CMSG_REQUEST_VEHICLE_SWITCH_SEAT)
	{
		recv_data >> Vehicleguid;
		recv_data >> RequestedSeat;
	}
	else
	{
		HandleMovementOpcodes(recv_data);
		recv_data >> Vehicleguid;
		recv_data >> RequestedSeat;
	}

	uint64 guid = Vehicleguid.GetOldGuid();
	Vehicle* vehicle = GetPlayer()->GetMapMgr()->GetVehicle(GET_LOWGUID_PART(guid));

	if(vehicle = GetPlayer()->m_CurrentVehicle)
	{
		if(RequestedSeat == GetPlayer()->m_inVehicleSeatId)
		{
			OUT_DEBUG("Return, Matching Seats. Requsted: %u, current: %u", RequestedSeat, GetPlayer()->m_inVehicleSeatId);
			return;
		}
	}

	Unit* existpassenger = vehicle->GetPassenger(RequestedSeat);
	if(existpassenger && (existpassenger != GetPlayer()))
	{
		if(existpassenger->IsVehicle())
		{
			Vehicle* vehpassenger = TO_VEHICLE(existpassenger);
			if(vehpassenger->IsFull())
				return;
			else
				vehpassenger->AddPassenger(GetPlayer());
		}
		else
			return;
	}

	vehicle->ChangeSeats(GetPlayer(), RequestedSeat);
}
