/*
Please read the copyright and license file in the root directory.
*/

#include "Setup.h"
#include "../Common/Base.h"
#include "../Common/EasyFunctions.h"
//#include "Management/TaxiMgr.h"
#include "TaxiMgr.h"

class GossipScourgeGryphon : public Arcemu::Gossip::Script
{
public:

    void OnHello(Object* pObject, Player* plr) override
    {
        if (plr->HasQuest(12670) || plr->HasFinishedQuest(12670))
        {
            if (TaxiPath* path = sTaxiMgr.GetTaxiPath(pObject->GetEntry() == 29488 ? 1053 : 1054))
                plr->TaxiStart(path, 26308, 0);
        }
    }
};
//Chapter One


class QuestInServiceOfLichKing : public QuestScript
{
public:

    void OnQuestStart(Player* mTarget, QuestLogEntry* /*qLogEntry*/)
    {
        // Play first sound
        mTarget->PlaySound(14734);

        // Play second sound after 22.5 seconds
        sEventMgr.AddEvent(mTarget, &Player::PlaySound, (uint32)14735, EVENT_UNK, 22500, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

        // Play third sound after 48.5 seconds
        sEventMgr.AddEvent(mTarget, &Player::PlaySound, (uint32)14736, EVENT_UNK, 48500, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
    }
};

//class ScourgeGryphonThree : public GossipScript
class GossipScourgeGryphonThree : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr) override
		{
			if (plr->HasQuest(12701) || plr->HasFinishedQuest(12701))
			{
				if (TaxiPath* path = sTaxiMgr.GetTaxiPath(pObject->GetEntry() == 28864 ? 1053 : 1054))
					plr->TaxiStart(path, 28108, 0);
			}
		}

};

// QuestID for Praparation for the Battle
enum QUEST_12842_ENUM
{
    QUEST_PREPARATION = 12842,

    SPELL_RUNE_I = 53341, // Spell Rune of Cinderglacier
    SPELL_RUNE_II = 53343, // Spell Rune of Razorice
    SPELL_PREPERATION_FOR_BATTLE_CREDIT = 54586
};

//bool PreparationForBattleEffect(uint8_t /*effectIndex*/, Spell* pSpell)
bool PreparationForBattleEffect(uint32 i, Spell* pSpell)
{
    Player* pCaster = pSpell->p_caster;
    if (pCaster == nullptr)
        return false;

    // Apply spell if caster has quest and still heven't completed it yet
    if (pCaster->HasQuest(QUEST_PREPARATION) && !pCaster->HasFinishedQuest(QUEST_PREPARATION))
        pCaster->CastSpell(pCaster, SPELL_PREPERATION_FOR_BATTLE_CREDIT, true);

    return true;
}

/* The Endless Hunger script for both GO and Unworthy Initiate */

#define CN_INITIATE_1				29519
#define CN_INITIATE_2				29565
#define CN_INITIATE_3				29567
#define CN_INITIATE_4				29520
#define CN_INITIATE_5				29566

    //SPELL_CHAINED_PESANT_LH        54602,              // not used. possible it determine side, where to go get "weapon"
    //SPELL_CHAINED_PESANT_RH        54610,
#define    SPELL_CHAINED_PESANT_CHEST      54612
#define    SPELL_CHAINED_PESANT_BREATH     54613
#define    SPELL_INITIATE_VISUAL           51519

#define    SPELL_BLOOD_STRIKE              52374
#define    SPELL_DEATH_COIL                52375
#define    SPELL_ICY_TOUCH                 52372
#define    SPELL_PLAGUE_STRIKE             52373

#define    NPC_ANCHOR                      29521
#define    FACTION_MONSTER                 16

#define    PHASE_INACTIVE                  0
#define    PHASE_STANDUP                   1
#define    PHASE_REACH_ARMORY              2
#define    PHASE_ACTIVATE                  3
#define    PHASE_ATTACK_PLAYER             4
#define    PHASE_ATTACKING                 5

struct DisplayToSpell
{
	uint32 displayid;
	uint32 spellid;
};

#define SAY_START_1	"You have made a grave error, fiend!"
#define SAY_START_2 "I was a soldier of the Light once... Look at what I have become..."
#define SAY_START_3 "You are hopelessly outmatched, $R."
#define SAY_START_4 "They brand me unworthy? I will show them unmorthy!"
#define SAY_START_5 "You will allow me a weapon and armor, yes?"
#define SAY_START_6 "I will win my freedom and leave this cursed place!"
#define SAY_START_7 "I will dismantle this festering hellhole!"
#define SAY_START_8 "There can be only one survivor!"

#define SAY_ATTACK_1 "To battle!"
#define SAY_ATTACK_2 "Let your fears consume you!"
#define SAY_ATTACK_3 "HAH! You can barely hold a blade! Yours will be a quick death."
#define SAY_ATTACK_4 "And now you die"
#define SAY_ATTACK_5 "To battle!"
#define SAY_ATTACK_6 "There is no hope for our future..."
#define SAY_ATTACK_7 "Sate your hunger on cold steel, $R"
#define SAY_ATTACK_8 "It ends here!"
#define SAY_ATTACK_9 "Death is the only cure!"

DisplayToSpell m_DisplayToSpell[] =
{
    {25354, 51520},                                         // human M
    {25355, 51534},                                         // human F
    {25356, 51538},                                         // dwarf M
    {25357, 51541},                                         // draenei M
    {25358, 51535},                                         // nelf M
    {25359, 51539},                                         // gnome M
    {25360, 51536},                                         // nelf F
    {25361, 51537},                                         // dwarf F
    {25362, 51540},                                         // gnome F
    {25363, 51542},                                         // draenei F
    {25364, 51543},                                         // orc M
    {25365, 51546},                                         // troll M
    {25366, 51547},                                         // tauren M
    {25367, 51549},                                         // forsaken M
    {25368, 51544},                                         // orc F
    {25369, 51552},                                         // belf F
    {25370, 51545},                                         // troll F
    {25371, 51548},                                         // tauren F
    {25372, 51550},                                         // forsaken F
    {25373, 51551}                                          // belf M
};

static Location m_ArmoryLocations[] = 
{
	{ 2461.047363f, -5570.428223f, 415.664490f, 1.735095f },
	{ 2472.961670f, -5575.300781f, 415.429169f, 0.368502f },
	{ 2474.302246f, -5576.693848f, 415.394287f, 1.012529f },
	{ 2480.435547f, -5584.607422f, 415.663696f, 0.328443f },
	{ 2482.882324f, -5590.556152f, 415.663605f, 0.383420f },
	{ 2448.824219f, -5612.786621f, 415.664154f, 3.540719f },
	{ 2447.006104f, -5611.062500f, 415.664154f, 4.298627f },
	{ 2440.834229f, -5604.432617f, 415.664154f, 4.243647f },
	{ 2437.441895f, -5590.024414f, 415.663879f, 2.822077f },
	{ 2439.216797f, -5585.857422f, 415.663879f, 2.802442f },
};

class AcherusSoulPrison : GameObjectAIScript
{
	public:
		AcherusSoulPrison(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript * Create(GameObject* GO)
		{
			return new AcherusSoulPrison(GO);
		}

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 12848 );
			if( !en || en->GetMobCount(0) != 0 )
				return;

			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();

			Creature * pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ);

			if(!pCreature || !pCreature->isAlive())
				return;

			if(pCreature->GetEntry() == CN_INITIATE_1 || 
			   pCreature->GetEntry() == CN_INITIATE_2 ||
			   pCreature->GetEntry() == CN_INITIATE_3 || 
			   pCreature->GetEntry() == CN_INITIATE_4 ||
			   pCreature->GetEntry() == CN_INITIATE_5 )
			{
				pPlayer->SendChatMessage(CHAT_MSG_SAY, LANG_UNIVERSAL, "I give you the key to your salvation");
				// this way we have refference to our player
				pCreature->SetSummonedByGUID( pPlayer->GetGUID() );

			switch(rand()%8)
			{
			case 0: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_1); break;
			case 1: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_2); break;
			case 2: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_3); break;
			case 3: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_4); break;
			case 4: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_5); break;
			case 5: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_6); break;
			case 6: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_7); break;
			case 7: pCreature->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_START_8); break;
			}

				// do all other things directly in creature's AI class, it's more comfortable
				pCreature->GetScript()->RegisterAIUpdateEvent(1000);
			}
		}
};

class UnworthyInitiate : public MoonScriptCreatureAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(UnworthyInitiate, MoonScriptCreatureAI);
		UnworthyInitiate(Creature * c) : MoonScriptCreatureAI(c)
		{
			AddSpell(SPELL_BLOOD_STRIKE, Target_Current, 85.0f, 0.0f, 4000);
			AddSpell(SPELL_DEATH_COIL, Target_Current, 85.f, 0.0f, 6000);
			AddSpell(SPELL_ICY_TOUCH, Target_Current, 85.f, 0.0f, 2000);
			AddSpell(SPELL_PLAGUE_STRIKE, Target_Current, 85.f, 0.0f, 5000);
		}

		void OnLoad()
		{
			RegisterAIUpdateEvent(500);
			_unit->SetUInt32Value(UNIT_FIELD_FLAGS, 33024); // from database
			_unit->SetFaction(7);
			_unit->SetStandState( uint8(STANDSTATE_KNEEL) );
			SetDisplayWeapon(false, false);
			state = -1;
			anchorGuid = 0;
			ParentClass::OnLoad();
		}

		void CheckForAnchor()
		{
			// attach nearest prison and cast spell on it
			Creature * anchor = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), 0.0f, NPC_ANCHOR);
			if( anchor )
			{
				anchor->SetChannelSpellTargetGUID( _unit->GetGUID() );
				anchor->SetChannelSpellId( SPELL_CHAINED_PESANT_BREATH );
				anchor->CastSpell(_unit, SPELL_CHAINED_PESANT_BREATH, false);
				anchorGuid = anchor->GetGUID();
			}
			
			_unit->CastSpell(_unit, SPELL_CHAINED_PESANT_CHEST, false);
		}

		void OnCombatStart(Unit* mTarget)
			{
				switch(rand()%8)
				{
				case 0: Emote(SAY_ATTACK_1); break;
				case 1: Emote(SAY_ATTACK_2); break;
				case 2: Emote(SAY_ATTACK_3); break;
				case 3: Emote(SAY_ATTACK_4); break;
				case 4: Emote(SAY_ATTACK_5); break;
				case 5: Emote(SAY_ATTACK_6); break;
				case 6: Emote(SAY_ATTACK_7); break;
				case 7: Emote(SAY_ATTACK_8); break;
				case 9: Emote(SAY_ATTACK_9); break;
				}
			 }

		void AIUpdate()
		{
			if( anchorGuid == 0 )
			{
				CheckForAnchor();
				if( anchorGuid != 0 )
				{
					state = PHASE_INACTIVE;
					RemoveAIUpdateEvent();
					return;
				}
			}

			if( state == PHASE_INACTIVE )
			{
				_unit->SetStandState( uint8(STANDSTATE_STAND) );
				_unit->RemoveAllAuras();

//				Player * plr = _unit->GetMapMgrPlayer( _unit->GetSummonedByGUID() );
//				if( plr )
//					SetFacingToObject( plr );

				Creature * anchor = _unit->GetMapMgrCreature( anchorGuid );
				if( anchor )
				{
					anchor->SetChannelSpellTargetGUID( 0 );
					anchor->SetChannelSpellId( 0 );
					//anchor->GetCurrentSpell()->cancel();
				}
		
				timer = AddTimer(1000);
				state = PHASE_STANDUP;
			}
			else if( state == PHASE_STANDUP && IsTimerFinished(timer) )
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have committed a big mistake, demon");
				timer = AddTimer(5000);
				state = PHASE_REACH_ARMORY;
			}
			else if( state == PHASE_REACH_ARMORY && IsTimerFinished(timer) )
			{
				// test for nearest coords of armory
				uint32 nearest_entry = 0;
				uint32 nearest_distance = 1000;
				for(uint32 i = 0; i < sizeof(m_ArmoryLocations) / sizeof(Location); i++)
				{
					uint32 distance = _unit->CalcDistance(m_ArmoryLocations[i].x, m_ArmoryLocations[i].y, m_ArmoryLocations[i].z);

					if( distance < nearest_distance )
					{
						nearest_distance = distance;
						nearest_entry = i;
					}
				}
				
				// now go to nearest armory
				MoveTo(m_ArmoryLocations[nearest_entry].x, m_ArmoryLocations[nearest_entry].y, m_ArmoryLocations[nearest_entry].z, true);
				timer = AddTimer(2000);
				state = PHASE_ACTIVATE;
			}
			else if( state == PHASE_ACTIVATE && IsTimerFinished(timer) )
			{
				// face off the player
//				Player * plr = _unit->GetMapMgrPlayer( _unit->GetSummonedByGUID() );
//				if( plr )
//					SetFacingToObject( plr );

				// select suitable spell
				uint32 spell_to_cast = 0;
				for(uint32 i = 0; i < sizeof(m_DisplayToSpell) / sizeof(DisplayToSpell); i++)
				{
					if( m_DisplayToSpell[i].displayid == _unit->GetDisplayId() )
					{
						spell_to_cast = m_DisplayToSpell[i].spellid;
						break;
					}
				}

				// change look
				_unit->CastSpell(_unit, spell_to_cast, false);
				_unit->CastSpell(_unit, 48266, false); // blood presence
				timer = AddTimer(2000);
				state = PHASE_ATTACK_PLAYER;
				// he equips same weapon as normal DK?
				SetDisplayWeaponIds(38707, 0);
			}
			else if( state == PHASE_ATTACK_PLAYER && IsTimerFinished(timer) )
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "And now you die!");
				state = PHASE_ATTACKING; // we are fighting
				timer = AddTimer(1000);
			}
			else if( state == PHASE_ATTACKING && IsTimerFinished(timer) )
			{
				_unit->SetFaction( 16 );

				Player * plr = _unit->GetMapMgrPlayer( _unit->GetSummonedByGUID() );
				if( plr )
				{
					SetCanEnterCombat( true );
					_unit->GetAIInterface()->AttackReaction( plr, 500, 0 );
					_unit->GetAIInterface()->setNextTarget( plr );
					_unit->GetAIInterface()->SetAIState(STATE_ATTACKING);
				//	_unit->GetAIInterface()->EventEnterCombat( plr, 0 );
					_unit->GetAIInterface()->HandleEvent(EVENT_ENTERCOMBAT, plr, 0 );
				}

				state = -1;
				//_unit->SetUInt32Value( UNIT_FIELD_FLAGS, 0 );
			}

			ParentClass::AIUpdate();
		}

		void OnDied(Unit * mKiller)
		{
			if( !mKiller->IsPlayer() )
				return;

			// don't credit if any other player kills my initiate
			if( mKiller->GetGUID() != _unit->GetSummonedByGUID() )
				return;

			Player * plr = TO_PLAYER(mKiller);
			QuestLogEntry * qle = plr->GetQuestLogForEntry(12848);
			if( !qle )
				return;

			qle->SetMobCount(0, 1);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
			qle->SendQuestComplete();

			ParentClass::OnDied(mKiller);
		}

		void OnCombatStop(Unit * pTarget)
		{
			_unit->Despawn(1000, 1000);
		}

		int32 timer;
		int8 state;
		uint64 anchorGuid;
};


static LocationExtra Cities[]=
{
	{ 1814.59f, -5988.65f, 125.497f, 28525 },
	{ 1590.81f, -5731.66f, 143.869f, 28543 },
	{ 1650.26f, -6004.33f, 157.072f, 28542 },
	{ 1385.93f, -5702.06f, 146.305f, 28544 },
};

class EyeOfAcherusControll : public QuestScript
{
	public:
		void OnQuestStart(Player* pPlayer, QuestLogEntry* qLogEntry)
		{
			GameObject* target1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2344.52f, -5698.25f, 426.02f, 191609);
			GameObject* target2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2320.20f, -5662.39f, 426.02f, 191609);

			if (target1 != NULL)
			{
				target1->SetUInt32Value( GAMEOBJECT_FLAGS,0);
			}
			if (target2 != NULL)
			{
				target2->SetUInt32Value( GAMEOBJECT_FLAGS,0);
			}

		}

		void OnQuestComplete(Player* pPlayer, QuestLogEntry* pQuest)
		{
			Summon* summoner = ((Summon*)pPlayer);
			static_cast<Unit*>(summoner)->RemoveAurasDueToSpell(530);
			((Summon*)pPlayer)->UnPossess();
		}
};

class EyeofAcherusControl : public GameObjectAIScript
{
	public:
		EyeofAcherusControl(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new EyeofAcherusControl(GO); }

		void OnActivate(Player* pPlayer)
		{
			if (!pPlayer->GetQuestLogForEntry(12641))
				return;
			
		//	pPlayer->CastSpell((pPlayer)->Object::event_GetInstanceID(), 51888, true); // eye loaded twice
			pPlayer->CastSpell((pPlayer)->Object::event_GetInstanceID(), 51852, true); // eye loaded once
		//	pPlayer->CastSpell(pPlayer, 51888, true); // eye loaded twice
		//	pPlayer->CastSpell(pPlayer, 51852, true); // eye loaded once
			_gameobject->SetUInt32Value( GAMEOBJECT_FLAGS,6553636);
		}
};

class TheEyesOfAcherus : public MoonScriptCreatureAI
{
	public:
	MOONSCRIPT_FACTORY_FUNCTION(TheEyesOfAcherus, MoonScriptCreatureAI);
	TheEyesOfAcherus(Creature* pCreature) : MoonScriptCreatureAI(pCreature) 
	{
			Active = false;
	}
	
		void OnLoad()
		{

		//	char * text = "The Eye of Acherus is in your control";
		//	Announce(text);
			_unit->SendChatMessage( CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL, "The Eye of Acherus launches towards its destination" );
		//	_unit->CreateWayPoint(1, 609, 1711.0f, -5820.0f, 147.0f, 0.0f);  // middle of New Avalon
			_unit->CreateWayPoint(1, 609,  1822.98f, -5927.86f, 154.35f, 4.48f);  //Forge
			Active = true;
		}

	public:
		bool Active;
};

bool SiphonofAcherus(uint32 i, Spell * pSpell) //Done almost working
{
	Unit* pObject = pSpell->u_caster;
	
//	if(pObject != NULL)
//	return true;
	
	PossessedSummon* summoner = ((PossessedSummon*)pObject->GetPlayerOwner());

	Player* Plr = ((Player*)summoner);
	if (Plr == NULL)	
		return true;

	QuestLogEntry* qle = Plr->GetQuestLogForEntry(12641);

	if (qle == NULL)
		return true;
	for(uint32 i = 0; i < sizeof(Cities) / sizeof(LocationExtra); i++)
	{
		Creature* Mark = summoner->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Cities[i].x, Cities[i].y, Cities[i].z, Cities[i].addition);

		if (Mark != NULL)
		{
			uint32 Dist = summoner->CalcDistance(pObject, Mark);
		//	uint32 Dist = pObject->GetDistanceSq(Cities[i].mX, -Cities[i].mY, Cities[i].mZ); //Forge 28525
			if (Dist <= 10 && qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
			{
					Plr->AddQuestKill(12641, i);
			}
		//	return true;
		}
	//	return true;
	}
	return true;
}

// 52694 - Recall Eye of Acherus

//CHAPTER TWO

//Script this quest fully at the moment auto completed :)
class IntoTheRealmOfShadows : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			mTarget->CastSpell(mTarget, 52693, false);

			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			Unit* Salanar = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 28653);
			if(Salanar == NULL)
				return;

			Salanar->SetUInt32Value(UNIT_NPC_FLAGS, 3);
		}

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			mTarget->PlaySound(12985);
		//	mTarget->RemoveAura( 52693, mTarget->GetGUID());
			mTarget->RemoveAura( 52693);
		}
};

bool AcherusDeathcharger(uint32 i, Spell* pSpell)
{
	Unit* pObject = pSpell->u_caster;

	Player* plr = pObject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(pObject->GetPositionX(),pObject->GetPositionY(),pObject->GetPositionZ()+2);
	
	uint32 POS = pObject->GetDistanceSq(2351.83f, -5701.29f, 153.92f);

	//Are we near the Quest giver?
	if (POS <= 10)
	{
		//Get Questlog
		QuestLogEntry* qle = plr->GetQuestLogForEntry(12687);

		if (qle == NULL)
			return true;
			
		//Complete Quest
		if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			uint32 NewCount = qle->GetMobCount(0) + 1;
			qle->SetMobCount(0, NewCount);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	//	plr->RemoveAura(52693);
	}
	return true;
}


class Salanar : public Arcemu::Gossip::Script
{
public:
	void OnHello(Object* pObject, Player* plr)
	{
		Arcemu::Gossip::Menu menu(pObject->GetGUID(), 13355);
		QuestLogEntry* pQuest = plr->GetQuestLogForEntry(12687);
		if(pQuest && pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0])

			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Salanar, I must return to the Realm of Shadows.", 2);		// or ,2 could be the working ,0
			
			menu.Send(plr);

	}

	void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char * Code)
    {
		if(plr->HasAura(52693))
			return;

        switch(Id)
        {
        case 0:
			OnHello( pObject, plr);
			break;
        case 2:
			{
				TO_UNIT(plr)->CastSpell(plr, 52693, false); // i am unsure if its not the: 52275 but i think no
            }break;
		}
    }
};

//CHAPTER TWO
void DeathsChallenge(Player * Winner, Player * Looser)
{
	QuestLogEntry * pQuest = Winner->GetQuestLogForEntry( 12733 );
	if(	pQuest )
	{
		if( pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
		}
	}
	return;
}

class DKInitiateAI : public CreatureAIScript
{
public:
        ADD_CREATURE_FACTORY_FUNCTION(DKInitiateAI);
		DKInitiateAI(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit * mTarget)
		{
		    if(mTarget->IsPlayer())
			{
			    RegisterAIUpdateEvent(500);
			}
		}

		void AIUpdate()
		{
		    Unit * u = _unit->GetAIInterface()->getNextTarget();
			if( u == NULL )
			        return;

			// Player is looser
			if(u->GetHealthPct() < 25)
			{
			    RemoveAIUpdateEvent();
				if( u->GetTypeId() != TYPEID_PLAYER )
				        return;

				Player * plr = TO_PLAYER(u);
				// plr stuff (stop attacking etc.)
				plr->smsg_AttackStop(_unit);
				plr->GetSession()->OutPacket( SMSG_CANCEL_COMBAT );
				plr->EventAttackStop();
				plr->Emote(EMOTE_ONESHOT_BEG);

				// NPC stuff (stop attacking, despawn)
				_unit->smsg_AttackStop(plr);
			//	_unit->SetFaction(35); // he will be despawned so make him just friendly OR
				_unit->SetFaction( 2084); // he will be despawned so make him just friendly
				_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_UNKNOWN_10 | UNIT_FLAG_UNKNOWN_16 ); // he will be despawned so make him just friendly
				_unit->Despawn(3000, 30000);

				// duel arbiter
				GameObject * arbiter = plr->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(plr->GetUInt64Value(PLAYER_DUEL_ARBITER)));
				if( arbiter != NULL )
				{
				    arbiter->RemoveFromWorld( true );
					delete arbiter;
				}

				plr->SetUInt64Value(PLAYER_DUEL_ARBITER, 0); // reset players arbiter

			}

			// Player is winner
			else if(_unit->GetHealthPct() < 25) // guessed
			{
			    if( u->GetTypeId() != TYPEID_PLAYER )
				       return;

				Player * plr = TO_PLAYER(u);

				// plr stuff (stop attacking etc.)
				plr->smsg_AttackStop(_unit);
				plr->GetSession()->OutPacket( SMSG_CANCEL_COMBAT );
				plr->EventAttackStop();

				QuestLogEntry * qle = plr->GetQuestLogForEntry(12733);
				if(qle && qle->GetMobCount(0) < 5)
				{
				    qle->SetMobCount(0, qle->GetMobCount(0)+1); // add +1 kill
					qle->SendUpdateAddKill(0);
					qle->UpdatePlayerFields();
				}

				if( !_unit->isAlive() ) // god we killed him probably O.O
				    return;
				RemoveAIUpdateEvent();

				// NPC stuff (stop attacking, beg, despawn)
				_unit->Root();
				_unit->smsg_AttackStop(plr);
				_unit->SetFaction( 2084); // he will be despawned so make him just friendly
				_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_UNKNOWN_10 | UNIT_FLAG_UNKNOWN_16 ); // he will be despawned so make him just friendly
				_unit->Emote(EMOTE_ONESHOT_BEG);
				_unit->Despawn(5000, 30000);

                // duel arbiter
				GameObject * arbiter = plr->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(plr->GetUInt64Value(PLAYER_DUEL_ARBITER)));
				if( arbiter != NULL )
				{
				    arbiter->RemoveFromWorld( true );
					delete arbiter;
				}

				plr->SetUInt64Value(PLAYER_DUEL_ARBITER, 0); // reset players arbiter
			}
		}
};

class DKInitiateGossipScript : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 13433);
			QuestLogEntry* pQuest = plr->GetQuestLogForEntry(12733);
			if(pQuest && pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0])
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "I challenge you, death knight!", 0);
			
			menu.Send(plr);

		}

		void OnSelectOption(Object * pObject, Player * plr, uint32 Id,  const char * Code)
		{
		   Creature *  pCreature = pObject->GetTypeId() == TYPEID_UNIT ? TO_CREATURE(pObject) : NULL;

		   if(pCreature == NULL)
		   return;

		   switch(Id)
		   {
		          case 0: // Attack
					  {
					  DKInitiateAI* pCreature = static_cast< DKInitiateAI*> (TO_CREATURE(pObject)->GetScript());
					  }
		          case 1: // Attack
                    {
                        plr->Gossip_Complete();
						// At first change their orientation after gossip ends
						pCreature->SetFacing( plr->GetOrientation()+3.14f );

						// Spawn duel flag - taken from source from normal duel
						float dist = plr->CalcDistance(pCreature);
						dist = dist * 0.5f; //half way
						float x = (plr->GetPositionX() + pCreature->GetPositionX()*dist)/(1+dist) + cos(plr->GetOrientation()+(float(M_PI)/2))*2;
						float y = (plr->GetPositionY() + pCreature->GetPositionY()*dist)/(1+dist) + sin(plr->GetOrientation()+(float(M_PI)/2))*2;
						float z = (plr->GetPositionZ() + pCreature->GetPositionZ()*dist)/(1+dist);

						//Create flag/arbiter
						GameObject * pGameObj = plr->GetMapMgr()->CreateGameObject(194306);
						pGameObj->CreateFromProto(194306,plr->GetMapId(), x, y, z, 0,plr->GetOrientation()+3.14f,0,0,0);
						pGameObj->SetInstanceID(plr->GetInstanceID());

						//Spawn the Flag
						pGameObj->SetUInt64Value(OBJECT_FIELD_CREATED_BY, plr->GetGUID());
						pGameObj->SetFaction(plr->GetFaction());
						pGameObj->_setFaction();
						pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, plr->getLevel());
						pGameObj->PushToWorld(plr->GetMapMgr());

						plr->SetUInt64Value(PLAYER_DUEL_ARBITER, pGameObj->GetGUID()); // used later for removing flag

						uint8 msg = RandomUInt(9);
					//	uint8 msg = rand()%9;
						string say;

						switch(msg)
						{
						   case 0:
						        {
								    say = "No potions!";

								}break;

						case 1:
						        {
								    say = "Remember this day,";
									say += plr->GetName();
									say += ", for it is the day that you will be thoroughly owned.";
								}break;
						case 2:
						        {
								   say = "I'm going to tear your heart out, cupcake!";
								}break;
						case 3:
						        {
								   say = "Don't make me laugh.";
								}break;
						case 4:
						        {
								  say = "Here come the tears...";
								}break;
						case 5:
						        {
								  say = "You have challenged death itself!";
								}break;
						case 6:
						        {
								  say = "The Lich King will see his true champion on this day!";
								}break;
						case 7:
						        {
								  say = "You're going down!";
								}break;
						case 8:
						        {
								  say = "You don't stand a chance,";
							      say += plr->GetName();
								}break;
					    }

						/*************************************/
				//		uint32 msgtype = CHAT_MSG_RAID_WARNING; // FIXME: This one is BAD! find a real type or packet
						/*************************************/

						pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str() );

						
						pCreature->SendChatMessage((uint8)CHAT_MSG_RAID_BOSS_EMOTE, (uint32)LANG_UNIVERSAL, "The duel will begin in...", 1000);
 
						pCreature->SendChatMessage((uint8)CHAT_MSG_RAID_BOSS_EMOTE, (uint32)LANG_UNIVERSAL, "3...", 2000);
 
						pCreature->SendChatMessage((uint8)CHAT_MSG_RAID_BOSS_EMOTE, (uint32)LANG_UNIVERSAL, "2...", 3000);
 
						pCreature->SendChatMessage((uint8)CHAT_MSG_RAID_BOSS_EMOTE, (uint32)LANG_UNIVERSAL, "1...", 4000);
 
						//sEventMgr.AddEvent(TO_UNIT(pCreature), &Unit::SendChatMessage, uint8(CHAT_MSG_MONSTER_SAY), uint32(LANG_UNIVERSAL), message, EVENT_UNIT_CHAT_MSG, 4000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						//sEventMgr.AddEvent(TO_UNIT(pCreature), &Unit::SetFaction, uint32(14), EVENT_CREATURE_UPDATE, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						sEventMgr.AddEvent(TO_UNIT(pCreature), &Creature::SetFaction, uint32(16), EVENT_CREATURE_UPDATE, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					//	pCreature->SetFaction(16);
						TO_CREATURE(pObject)->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
						TO_CREATURE(pObject)->SetUInt32Value(UNIT_NPC_FLAGS, 0);
						sEventMgr.AddEvent(pCreature, &Creature::EventAttack, TO_UNIT(plr), EVENT_UNK, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					}
				break;
			}
		}

		void Destroy()
		{
		    delete this;
		};
};

class Massakre2 : public MoonScriptCreatureAI
{
	public:
	MOONSCRIPT_FACTORY_FUNCTION(Massakre2, MoonScriptCreatureAI);
	Massakre2(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnDied(Unit* mKiller)
	{

	Player* plr =NULL;
	QuestLogEntry *qle	= NULL;

		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); itr++) 
		{
			if((*itr)->GetTypeId() == TYPEID_PLAYER)
			{
				plr = TO_PLAYER((*itr));
				if (plr != NULL)
				{
				qle = plr->GetQuestLogForEntry(12701);
				if(qle == NULL)
					return ;
	
					Unit* target = _unit->GetAIInterface()->getNextTarget();
					for(int32 i = 0; i < 100; i++)
					{
						target = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 28834);
						plr->AddQuestKill(12701, i);
					}	return;
				}
			}
		}
	}
};

bool StolenHorse(uint32 i, Spell* pSpell)
{
	Unit* pObject = pSpell->u_caster;

	Player* plr = pObject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(pObject->GetPositionX(),pObject->GetPositionY(),pObject->GetPositionZ()+2);
	
	uint32 POS = pObject->GetDistanceSq(2351.83f, -5701.29f, 153.92f);

	//Are we near the Quest giver?
	if (POS <= 5)
	{
		//Get Questlog
		QuestLogEntry* qle = plr->GetQuestLogForEntry(12680);

		if (qle == NULL)
			return true;
			
		//Complete Quest
		if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			uint32 NewCount = qle->GetMobCount(0) + 1;
			qle->SetMobCount(0, NewCount);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
		}
	}
	return true;
}

class GiftoftheHarvester : public GameObjectAIScript
{
	public:
		GiftoftheHarvester(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new GiftoftheHarvester(GO); }

		void OnSpawn()
		{
			Creature* pObject = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 28822 );
			if (pObject == NULL)
				return;

			uint8 chance = RandomUInt(3);

			if (chance == 0)
			{
				pObject->CastSpell(pObject, 47769, false);
			}

			if (chance == 3)
			{
				pObject->Despawn(1000, 100000);

				Player* Plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords( _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());
				
				if (!Plr)
					return;

				Plr->CastSpell(Plr, 52479, true);	
			}
		}
};

bool GabedesErnters(uint32 i, Spell * pSpell)
{
	Player * p_caster = pSpell->p_caster;
	CreatureProto * proto = CreatureProtoStorage.LookupEntry(28845);
	if ( !proto )
		return true;

	uint32 summonpropid = pSpell->m_spellInfo->EffectMiscValue[ i ];
	SummonPropertiesEntry * spe = dbcSummonProperties.LookupEntry( summonpropid );
	if ( !spe )
		return true;

	p_caster->DismissActivePets();
	p_caster->RemoveFieldSummon();

	Summon * s = p_caster->GetMapMgr()->CreateSummon(proto->Id, SUMMONTYPE_COMPANION);
	if (s == NULL)
		return true;

	LocationVector v(p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), 3.776f);

	s->Load(proto, p_caster, v, pSpell->m_spellInfo->Id, spe->Slot - 1);
	s->SetCreatedBySpell(pSpell->m_spellInfo->Id);
	s->PushToWorld(p_caster->GetMapMgr());

	return true;
}

class GhulAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GhulAI);
		GhulAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(2000);
		};

		void AIUpdate()
		{

			uint32 POS = _unit->GetDistanceSq(2348.47f, -5758.85f, 153.92f);

			if (POS <= 15)
			{
				Player* Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(),_unit->GetPositionY(), _unit->GetPositionZ());
				if (!Plr)
					return;

					QuestLogEntry* qle = Plr->GetQuestLogForEntry(12698);

					if (qle == NULL)
						return;

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
						{
							uint32 NewCount = qle->GetMobCount(0) + 1;
							qle->SetMobCount(0, NewCount);
							qle->SendUpdateAddKill(0);
							qle->UpdatePlayerFields();
							_unit->Despawn(2000, 0);
						}
						RemoveAIUpdateEvent();
						return;
			}
		};
};
/*
class Massakre : public QuestScript//Auto Complete because of Vehicle Bug
{
	public:
		void OnQuestStart(Player* Plr, QuestLogEntry* qle)
		{
			qle->Complete();
		}
};*/

class Massakre : public MoonScriptCreatureAI
{
	public:
	MOONSCRIPT_FACTORY_FUNCTION(Massakre, MoonScriptCreatureAI);
	Massakre(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnDied(Unit* mKiller)
	{

	Player* plr =NULL;
	QuestLogEntry *qle	= NULL;

		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); itr++) 
		{
			if((*itr)->GetTypeId() == TYPEID_PLAYER)
			{
				plr = TO_PLAYER((*itr));
				if (plr != NULL)
				{
				qle = plr->GetQuestLogForEntry(12701);
				if(qle == NULL)
					return ;
	
				Unit* target = _unit->GetAIInterface()->getNextTarget();
					for(uint32 i = 0; i <= 100; i++)
					{
						target = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 28834);
						if( qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
						{
							plr->AddQuestKill(12701, i);
							break;
						}	return;
					}	return;
				}
			}
		}
	}
};

class TheWillOfLichKing : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			mTarget->Phase(PHASE_SET ,4);
		}
};

class crusade_persuaded : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(crusade_persuaded);
		crusade_persuaded(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(500);
		};

		void AIUpdate()
		{

			Aura* a = _unit->FindAura(52781);

			if (a != NULL)
			{
				Unit* Caster = a->GetUnitCaster();

				//printf("Got the Aura\n");
				uint8 chance = RandomUInt(6);

				switch (chance)
				{
					case 0:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I'll tell you everything! STOP! PLEASE!");
						break;

					case 1:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "We... We have only been told that the Crimson Dawn is an awakening. You see, the Light speaks to the High General. It is the Light...");
						break;

					case 2:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The Light that guides us. The movement was set in motion before you came... We... We do as we are told. It is what must be done.");
						break;

					case 3:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I know very little else... The High General chooses who may go and who must stay behind. There's nothing else... You must believe me!");
						break;

					case 4:
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "LIES! The pain you are about to endure will be talked about for years to come!");
						break;

					default:	// for higher chance 5 + 6

						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "NO! PLEASE! There is one more thing that I forgot to mention... A courier comes soon... From Hearthglen. It...");
						//printf("Quest Complete\n");

						QuestLogEntry* qle = TO_PLAYER(Caster)->GetQuestLogForEntry(12720);
						if (qle == NULL)
							return;

						sEventMgr.AddEvent(TO_UNIT(Caster), &Unit::EventCastSpell, TO_UNIT(Caster), dbcSpell.LookupEntry(58111), EVENT_CREATURE_UPDATE, 3000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						break;
				}

				RemoveAIUpdateEvent();
			}
		}
};

static Location KoltiraWalk[] =
{
	{ 0, 0, 0, 0 },								//1
	{ 1653.35f, -6038.11f, 127.58f, 1.471332f },//2
	{ 1653.84f, -6034.27f, 127.58f, 1.471332f },//3
	{ 1653.83f, -6036.59f, 127.58f, 4.079270f },//4
	{ 1653.83f, -6036.59f, 127.58f, 4.079270f },//5
	{ 1653.84f, -6034.27f, 127.58f, 1.471332f },//6
	{ 1653.84f, -6034.27f, 127.58f, 1.471332f },//7
	{ 1659.84f, -6039.27f, 127.58f, 1.471332f } //8
};

//Todo Add correct npc text and script the boss
class koltira_deathweaver_AI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(koltira_deathweaver_AI);

		koltira_deathweaver_AI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 1000, WALK));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 2000, WALK));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 1000, WALK));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, WALK));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, WALK));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, WALK));
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, WALK));

			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			phase = 0;
		}

		
		void OnReachWP(uint32 i, bool usl)
		{
			switch(i)
			{
				case 1:
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(2);
					break;

				case 2:
					_unit->SetStandState(STANDSTATE_KNEEL);
					_unit->CastSpell(_unit, 52899, true);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(3);
					break;

				case 3:
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(4);
					break;

				case 4:
//					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX() ,_unit->GetPositionY() ,_unit->GetPositionZ() );
					_unit->PlaySpellVisual(_unit->GetGUID(), 52894);
					_unit->CastSpell(_unit, 52894, true);
					phase++;
					sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					break;

				case 7:
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
					_unit->GetAIInterface()->m_canMove = false;
					break;

				case 1000:
					sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->GetAIInterface()->setWaypointToMove(1);
					break;

				default:
					if (i > 1 && i < 7)
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(i + 1);
					}
					break;
			}
		}

		void AIUpdate()
		{

			Player * Plr = NULL;

			switch(phase)
			{
				case 0:
					_unit->GetAIInterface()->StopMovement(0);
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(1);
					break;

				case 1:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX() ,_unit->GetPositionY() ,_unit->GetPositionZ() );
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					phase++;
					sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					break;

				case 2:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX() ,_unit->GetPositionY() ,_unit->GetPositionZ() );
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					phase++;
					sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					break;

				case 3:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX() ,_unit->GetPositionY() ,_unit->GetPositionZ() );
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					sEAS.SpawnCreature(Plr, 29007, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					phase++;
					sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, 25000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					break;
				case 4:
					Plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX() ,_unit->GetPositionY() ,_unit->GetPositionZ() );
					sEAS.SpawnCreature(TO_PLAYER(Plr), 29001, 1642.329f, -6045.818f, 127.583f, 0.0f, 60000, Plr->GetPhase());
					phase++;
					sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, 25000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					break;
				case 5:
					_unit->Despawn(1000,2000);
					break;
			}
		}

		inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
		{
			WayPoint* wp = _unit->CreateWaypointStruct();
			wp->id = id;
			wp->x = KoltiraWalk[id].x;
			wp->y = KoltiraWalk[id].y;
			wp->z = KoltiraWalk[id].z;
			wp->o = KoltiraWalk[id].o;
			wp->waittime = waittime;
			wp->flags = flags;
			wp->forwardemoteoneshot = false;
			wp->forwardemoteid = 0;
			wp->backwardemoteoneshot = false;
			wp->backwardemoteid = 0;
			wp->forwardskinid = 0;
			wp->backwardskinid = 0;
			return wp;
		}

	protected:
		uint32 phase;
};

class Breakout : public QuestScript
{
	public:
		void OnQuestStart(Player* Plr, QuestLogEntry* qLogEntry)
		{
			Creature* Koltira = Plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Plr->GetPositionX(), Plr->GetPositionY(), Plr->GetPositionZ(), 28912);

			if (!Koltira == NULL)
			{
				TO_CREATURE(Koltira)->GetScript()->OnReachWP(1000, 0);
			}
		}
};

class Valroth_AI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Valroth_AI);
		Valroth_AI(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnDied(Unit* mKiller)
		{
			_unit->CastSpell(_unit, 52929, true);
		}
};

class npc_a_special_surprise_AI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(npc_a_special_surprise_AI);
		npc_a_special_surprise_AI(Creature* pCreature) : CreatureAIScript(pCreature) 
		{
			RegisterAIUpdateEvent(1000);
		};

		void OnLoad()
		{
			phase = 0;
		}

		void AIUpdate()
		{

			Player* player		= NULL;
			QuestLogEntry* qle	= NULL;
			Creature* Plag		= NULL;
			
			switch (phase)
			{
				case 1:		//Orc
					phase = 1000;	//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have you?", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I want to die like an orc...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 6000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize that face anywhere... What... What have they done to you, $N?",9000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You don't recognize me, do you? Blasted Scourge... They've tried to drain you of everything that made you a righteous force of reckoning. Every last ounce of good... Everything that made you an orc!", 12000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think, $N. Think back. Try and remember Durotar, $Gbrother:sister;! Remember the sacrifices our heroes made so that we could be free of the blood curse. Harken back to the Valley of Trials, where we were reborn into a world without demonic influence. We found the splendor of life, $N. Together! This isn't you. You were a champion of the Horde once!", 15000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N. You must fight against the Lich King's control. He is a monster that wants to see this world - our world - in ruin. Don't let him use you to accomplish his goals. You were once a hero and you can be again. Fight, damn you! Fight his control!", 18000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. Finish me off, $N. Do it or they'll kill us both. $N... For the Horde! This world is worth saving.", 22000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 10000);


					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );

					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12748);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;
					
				case 2:		//Human

					phase = 1000;//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have you?", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I'd like to stand for...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 8000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize that face anywhere... What... What have they done to you, $N?",10000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You don't remember me, do you? Blasted Scourge...They've tried to drain of everything that made you a righteous force of reckoning. Every last ounce of good...Everything that made you a human!", 15000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think, $N. Think back. Try and remember the hills and valleys of Elwynn, where you were born. Remember the splendor of life, $Gbrother:sister;. You were a champion of the Alliance once! This isn't you.", 19000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N. You must fight against the Lich King's control. He is a monster that wants to see this world - our world - in ruin. Don't let him use you to accomplish his goals. You were once a hero and you can be again. Fight, damn you! Fight his control!", 22000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 26000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. FInish me off, $N. Do it or they'll kill us both. $N...Remember Elwynn. This world is worth saving.", 29000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 30000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 32000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 35000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 36000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 37000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 5000);
				
					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					
					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12742);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;
					
				case 3:		//Dwarf

					phase = 1000;//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have ye", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Well this son o' Ironforge would like...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 6000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize that face anywhere... What... What have they done to ye, $Glad:lass;?",9000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ye don't remember me, do ye? Blasted Scourge... They've tried to drain ye o' everything that made ye a righteous force o' reckoning. Every last ounce o' good... Everything that made you a $Gson:daughter; of Ironforge!", 12000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think, $N. Think back. Try and remember the snow capped mountains o' Dun Morogh! Ye were born there, $Glad:lass;. Remember the splendor o' life, $N! Ye were a champion o' the dwarves once! This isn't ye!", 15000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N Ye must fight against the Lich King's control. He's a monster that wants to see this world - our world - in ruin. Don't let him use ye to accomplish his goals. Ye were once a hero and ye can be again. Fight, damn ye! Fight his control!", 18000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. Finish me off, $N. Do it or they'll kill us both $N... For KHAAAAAAAAZZZ MODAAAAAANNNNNN!!!", 22000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 5000);

					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					
					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12744);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;
					
				case 4:		//Nightelf

					phase = 1000;//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have you?", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I'd like to stand for...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 6000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize that face anywhere... What... What have they done to you, $N?", 12000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You don't remember me? When you were a child your mother would leave you in my care while she served at the Temple of the Moon. I held you in my arms and fed you with honey and sheep's milk to calm you until she would return. You were my little angel. Blasted Scourge... What have they done to you, $N?", 15000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You must remember the splendor of life, $Gbrother:sister;. You were a champion of the Kaldorei once! This isn't you!", 18000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N. You must fight against the Lich King's control. He is a monster that wants to see this world - our world - in ruin. Don't let him use you to accomplish his goals. You were once a hero and you can be again. Fight, damn you! Fight his control!", 17000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. Finish me off, $N. Do it or they'll kill us both. $N... Remember Teldrassil, our beloved home. This world is worth saving.", 22000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 5000);

					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					
					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12743);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;

				case 5:		//Undead

					phase = 1000;//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have you?", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ironic, isn't it? To be killed...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 4000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize that decay anywhere... What... What have they done to you, $N?", 6000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You don't remember me, do you? We were humans once - long, long ago - until Lordaeron fell to the Scourge. Your transformation to a Scourge zombie came shortly after my own. Not long after that, our minds were freed by the Dark Lady.", 9000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "A pact was made, $Gbrother:sister;! We vowed vengeance against the Lich King! For what he had done to us! We battled the Scourge as Forsaken, pushing them back into the plaguelands and freeing Tirisfal! You and I were champions of the Forsaken!", 13000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N. You must fight against the Lich King's control. He is a monster that wants to see this world - our world - in ruin. Don't let him use you to accomplish his goals AGAIN. You were once a hero and you can be again. Fight, damn you! Fight his control!", 16000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. Finish me off, $N. Do it or they'll kill us both. $N... Remember Tirisfal! This world is worth saving!", 22000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 5000);

					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					
					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12750);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;

				case 6:		//Tauren

					phase = 1000;//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have you?", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You'll look me in the eyes when...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 4000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize those horns anywhere... What have they done to you, $N?", 6000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You don't remember me, do you? Blasted Scourge... They've tried to drain you of everything that made you a righteous force of reckoning. Every last ounce of good... Everything that made you a tauren!", 9000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think, $N. Think back. Try and remember the rolling plains of Mulgore, where you were born. Remember the splendor of life, $Gbrother:sister;. You were a champion of the tauren once! This isn't you.", 13000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N. You must fight against the Lich King's control. He is a monster that wants to see this world - our world - in ruin. Don't let him use you to accomplish his goals. You were once a hero and you can be again. Fight, damn you! Fight his control!", 16000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. Finish me off, $N. Do it or they'll kill us both. $N... Remember Mulgore. This world is worth saving.", 22000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 5000);

					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					
					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12739);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;

				case 7:		//Gnome

					phase = 1000;//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have you?", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "If you'd allow me just one...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 4000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize that face anywhere... What... What have they done to you, $N?", 6000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You don't remember me, do you? Blasted Scourge... They've tried to drain you of everything that made you a pint-sized force of reckoning. Every last ounce of good... Everything that made you a gnome!", 9000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think, $N. Think back. Try and remember Gnomeregan before those damned troggs! Remember the feel of an [arclight spanner] $Gbrother:sister;. You were a champion of gnome-kind once! This isn't you.", 13000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N. You must fight against the Lich King's control. He is a monster that wants to see this world - our world - in ruin. Don't let him use you to accomplish his goals. You were once a hero and you can be again. Fight, damn you! Fight his control!", 16000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. Finish me off, $N. Do it or they'll kill us both. $N... Remember Gnomeregan! This world is worth saving.", 22000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 5000);

					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					
					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12745);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;

				case 8:			//Troll

						phase = 1000;//Haxxed
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come ta finish da job, mon?", 0);
						_unit->SetStandState(STANDSTATE_STAND);
                        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Dis troll gonna stand for da...", 3000);
                        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N? Mon?", 4000);
                        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize dem tusks anywhere... What... What have dey done ta you, mon?", 6000);
                        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You don't remember me, mon? Damn da Scourge! Dey gone ta drain you of everytin dat made ya a mojo masta. Every last ounce of good... Everytin' dat made ya a troll hero, mon!", 9000);
                        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "TINK $N. Tink back, mon! We be Darkspear, mon! Bruddas and sistas! Remember when we fought the Zalazane and done took he head and freed da Echo Isles? MON! TINK! You was a champion of da Darkspear trolls!", 13000);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen ta me, $Gbrudda:sista;. You must fight against da Lich King's control. He be a monstar dat want ta see dis world - our world - be ruined. Don't let he use you ta accomplish he goals. You be a hero once and you be a hero again! Fight it, mon! Fight he control!", 16000);

						Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

						Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
						
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Der... Der's no more time for me. I be done for. Finish me off $N. Do it or they'll kill us both. $N... Remember Sen'jin Village, mon! Dis world be worth saving!", 22000);
						sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
                        _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
						sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						_unit->Despawn(30000, 5000);

						player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
						
						if (player == NULL)
							return;

						qle = player->GetQuestLogForEntry(12749);

						if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
						{
							uint32 NewCount = qle->GetMobCount(0) + 1;
							qle->SetMobCount(0, NewCount);
							qle->SendUpdateAddKill(0);
							qle->UpdatePlayerFields();
						}

					break;

				case 9:		//Bloodelf

					phase = 1000;//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have you?", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You'll look me in the eyes when...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 4000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize that face anywhere... What... What have they done to you, $N?", 6000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think, $N. Think back. Try and remember the majestic halls of Silvermoon City, where you were born. Remember the splendor of life, $Gbrother:sister;. You were a champion of the sin'dorei once! This isn't you.", 9000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N. You must fight against the Lich King's control. He is a monster that wants to see this world - our world - in ruin. Don't let him use you to accomplish his goals. You were once a hero and you can be again. Fight, damn you! Fight his control!", 14000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. Finish me off, $N. Do it or they'll kill us both. $N... Remember Silvermoon. This world is worth saving!", 22000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 5000);

					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					
					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12747);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;

				case 10:		//Drenai

					phase = 1000;//Haxxed
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to finish the job, have you?", 0);
					_unit->SetStandState(STANDSTATE_STAND);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You'll look me in the eyes when...", 3000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N?", 4000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "$N, I'd recognize those face tentacles anywhere... What... What have they done to you, $N?", 6000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You don't remember me, do you? Blasted Scourge... They've tried to drain you of everything that made you a righteous force of reckoning. Every last ounce of good... Everything that made you a draenei!", 9000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Think, $N. Think back. Try and remember the proud mountains of Argus, where you were born. Remember the splendor of life, $Gbrother:sister;. You were a champion of the draenei once! This isn't you.", 12000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Listen to me, $N. You must fight against the Lich King's control. He is a monster that wants to see this world - our world - in ruin. Don't let him use you to accomplish his goals. You were once a hero and you can be again. Fight, damn you! Fight his control!", 16000);

					Plag = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 1371.66f , -5719.72f , 136.504f , 29053);

					Plag->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's going on in there? What's taking so long, $N?", 20000);
					
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "There... There's no more time for me. I'm done for. Finish me off, $N. Do it or they'll kill us both. $N... Remember Argus. Don't let that happen to this world.", 22000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState,(uint8)STANDSTATE_KNEEL , EVENT_CREATURE_UPDATE, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Do it, $N! Put me out of my misery!", 24000);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "%s dies from his wounds.", 25000);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::setDeathState ,(JUST_DIED) , EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetHealth ,(uint32)0, EVENT_CREATURE_UPDATE, 27000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					_unit->Despawn(30000, 5000);

					player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					
					if (player == NULL)
						return;

					qle = player->GetQuestLogForEntry(12746);

					if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
					{
						uint32 NewCount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, NewCount);
						qle->SendUpdateAddKill(0);
						qle->UpdatePlayerFields();
					}

					break;
			}

			player = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if (player == NULL)
				return;

			if (!phase == 0)
				return;

			switch (_unit->GetEntry())
			{
				case 29061:                                     // Ellen Stanbridge

					if (!player->HasQuest(12742))
						return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 2;

					break;
				
				case 29072:                                     // Kug Ironjaw

					if (!player->HasQuest(12748))
						return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 1;

					break;
					
				case 29067:                                     // Donovan Pulfrost

                    if (!player->HasQuest(12744))
                        return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 3;

                    break;
  
				case 29065:										// Yazmina Oakenthorn

                    if (!player->HasQuest(12743))
                        return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 4;

                    break;

				case 29071:                                     // Antoine Brack

                    if (!player->HasQuest(12750))
                        return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 5;

                    break;
					
				case 29032:                                     // Malar Bravehorn

                    if (!player->HasQuest(12739))
                        return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 6;

                    break;
					
				case 29068:                                     // Goby Blastenheimer

                    if (!player->HasQuest(12745))
                        return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 7;

                    break;
					
				case 29073:                                     // Iggy Darktusk

                    if (!player->HasQuest(12749))
                        return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 8;

                    break;
					
				case 29074:                                     // Lady Eonys

                    if (!player->HasQuest(12747))
                        return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 9;

                    break;
					
				case 29070:                                     // Valok the Righteous

                    if (!player->HasQuest(12746))
                        return;

					if (player->GetDistanceSq(_unit) >= 2)
						return;

					phase = 10;

                    break;
			}
		}
		
		protected:
			uint32 phase;
};

class mob_scarlet_courier_AI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(mob_scarlet_courier_AI);
		mob_scarlet_courier_AI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			
		};

		void OnLoad()
		{
			_unit->SetMount(14338);
			_unit->SetFaction(2089);
			_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);
			_unit->RemoveFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_TAGGED_BY_OTHER);
			_unit->SetFlag(UNIT_DYNAMIC_FLAGS , U_DYN_FLAG_LOOTABLE);
			_unit->generateLoot();
		};

		void OnDeath()
		{
			_unit->generateLoot();
		}
};

class Conversation_1 : public QuestScript
{
	public:
		void OnQuestStart(Player* Plr, QuestLogEntry* qLogEntry)
		{
			Plr->SetFaction(2089);
			Plr->SetDisplayId(25897);
		}

		void OnQuestComplete(Player* Plr, QuestLogEntry* qLogEntry)
		{
			Plr->SetFaction(35);
		}
};

class Conversation_2 : public QuestScript
{
	public:
		void OnQuestComplete(Player* Plr, QuestLogEntry* qLogEntry)
		{
			Plr->SetFaction(2051);
			Plr->DeMorph();
		}
};

class Phase200 : public QuestScript
{
	public:
		void OnQuestStart(Player* Plr, QuestLogEntry* qLogEntry)
		{
			Plr->Phase(PHASE_SET ,200);
		}
};

class Frostbrood : public QuestScript
{
	public:
		void OnQuestStart(Player* Plr, QuestLogEntry* qLogEntry)
		{
	//		qLogEntry->Complete();
		}
};

class morgen : public QuestScript
{
	public:
		void OnQuestStart(Player* Plr, QuestLogEntry* qle)
		{
			if (qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
				{
					uint32 NewCount = qle->GetMobCount(0) + 1;
					qle->SetMobCount(0, NewCount);
					qle->SendUpdateAddKill(0);
					qle->UpdatePlayerFields();
				}
		}
};

class WhereKingsWalk : public QuestScript
{
	public:
		void OnQuestStart(Player* Plr, QuestLogEntry* qle)
		{
			Plr->Phase(PHASE_SET ,1);
		}
};
/*
class SalanarGossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			//Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 13355);
			//QuestLogEntry* pQuest = plr->GetQuestLogForEntry(12687);
			//if(plr->GetQuestLogForEntry(12687))
			if(plr->HasQuest(12687))
			//if(pQuest && pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0])
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Salanar, I must return to the Realm of Shadows.", 2);		// or ,2 could be the working ,0
			
			menu.Send(plr);

		}


	void OnSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if(pObject->GetTypeId() != TYPEID_UNIT)
			return;

        switch(Id)
        {
        case 0:
			OnHello( pObject, plr);
			break;
        case 2:
			{
				TO_UNIT(pObject)->CastSpell(plr, 52693, false); // i am unsure if its not the: 52275 but i think no
            }break;
		Arcemu::Gossip::Menu::Complete(plr);
		}
    }

	void Destroy()
	{
		delete this;
	};
};
*/
// called by spell 52363 which is used in Q 12687 (spell is from vehicle)
class SalanarSec : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SalanarSec);
	SalanarSec(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SendChatMessage(CHAT_MSG_SAY, LANG_UNIVERSAL, "Impressive, death knight. Return to me in the world of living for your reward.");
		_unit->Despawn(10000, 0); // despawn after 10 seconds
	}
};

class CitizenAI : public MoonScriptCreatureAI
{
	public:
	MOONSCRIPT_FACTORY_FUNCTION(CitizenAI, MoonScriptCreatureAI);
	CitizenAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnLoad()
	{
		_unit->GetAIInterface()->m_canCallForHelp = false;
	}
	void OnCombatStart(Unit * mTarget)
	{
		float chance = 40.0f; // base chance set as 40 %
		if(_unit->getGender() == 1) // women
			chance *= 2; // double chance

		// cover only if its player, they fight with other npcs normally
		if(Rand(chance) && mTarget->GetTypeId() == TYPEID_PLAYER)
		{
			if(mTarget->GetPhase() == 1) // LK voices can be only in phase 1
			{
				uint8 msg = RandomUInt(15);
				uint32 sound = 14771+msg;
				string whisp;

				switch(msg)
				{
				case 0:
					{
						whisp = "No mercy.";
					}break;
				case 1:
					{
						whisp = "Kill them all.";
					}break;
				case 2:
					{
						whisp = "Mercy is for the weak.";
					}break;
				case 3:
					{
						whisp = "End it.";
					}break;
				case 4:
					{
						whisp = "Finish it.";
					}break;
				case 5:
					{
						whisp = "No survivors.";
					}break;
				case 6:
					{
						whisp = "Kill or be killed, ";
						whisp += TO_PLAYER(mTarget)->GetName();
						whisp += ".";
					}break;
				case 7:
					{
						whisp = "Do not hesitate.";
					}break;
				case 8:
					{
						whisp = "Harness your rage and focus.";
					}break;
				case 9:
					{
						whisp = "Strike it down.";
					}break;
				case 10:
					{
						whisp = "Suffering to the conquered.";
					}break;
				case 11:
					{
						whisp = "Show it the meaning of terror.";
					}break;
				case 12:
					{
						whisp = "End it's miserable life.";
					}break;
				case 13:
					{
						whisp = "Give into the darkness, Death Knight.";
					}break;
				case 14:
					{
						whisp = "Do you think it would hesitate to kill you, a monster like you?";
					}break;
				case 15:
					{
						whisp = "Living or dead, you will serve me.";
					}break;
				}

				/* this is probably best way around
				   it spawns invisible lich king near you for 1 sec */
				Creature * cr = sEAS.SpawnCreature(TO_PLAYER(mTarget), 16980, mTarget->GetPositionX(), mTarget->GetPositionY(), mTarget->GetPositionZ(), 0.0f, 1000, 1);
			
				if(cr)
				{
					cr->SendChatMessage( CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, whisp.c_str() );
					TO_PLAYER(mTarget)->PlaySound(sound);
				}
			}

			// this will make creature stop attacking you and stay at 1 place with emote
			_unit->smsg_AttackStop(mTarget);

			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->_UpdateCombat(0);
			_unit->Root();
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "No! Please! I.. I have children! I... I...");
			_unit->GetAIInterface()->WipeHateList();
			_unit->SetEmoteState(431);
		}
		else
		{
			uint8 say = RandomUInt(1);

			switch(say)
			{
				case 0:
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come then, death knight!");
					}break;
				case 1:
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You may take my life, but you won't take my freedom!");
					}break;
			}

			// Enrage! :P
			_unit->CastSpell(_unit, 52262, false);
		}
	}

	void OnReachWP(uint32 WPId, bool bForwards)
	{
		// Despawn them when they reach their last WP, they are not running backwards
		if(WPId == _unit->GetAIInterface()->GetWayPointsCount())
			_unit->Despawn(0, 10000);
	}
};

class PeasantAI : public MoonScriptCreatureAI
{
	public:
	MOONSCRIPT_FACTORY_FUNCTION(PeasantAI, MoonScriptCreatureAI);
	PeasantAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnCombatStart(Unit * mTarget)
	{
		RegisterAIUpdateEvent(1000);
	}

	void AIUpdate()
	{
		if(_unit->GetHealthPct() < 75) // they covered around 75% of HP
		{
			uint8 msg = RandomUInt(4);
			string say;

			switch(msg)
			{
				case 0:
					{
						say = "I... I got a sick grandma at home... I... I'm all she's got, mister ";
					}break;
				case 1:
					{
						say = "I picked the wrong week to quit drinkin'!";
					}break;
				case 2:
					{
						say = "You don't have to do this! Nobody has to die!";
					}break;
				case 3:
					{
						say = "Ungh... I... I think I pooped...";
					}break;
				case 4:
					{
						say = "I've got five kids, man! They'll die without me!";
					}break;
			}

			Unit * u = _unit->GetAIInterface()->GetMostHated();
			if( u )
				_unit->smsg_AttackStop(u);

			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->_UpdateCombat(0);

			RemoveAIUpdateEvent();
			_unit->Root();
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str() );
			_unit->GetAIInterface()->WipeHateList();
			_unit->SetEmoteState(431);
		}
	}
};

class MinerAI : public MoonScriptCreatureAI
{
	public:
	MOONSCRIPT_FACTORY_FUNCTION(MinerAI, MoonScriptCreatureAI);
	MinerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnReachWP(uint32 WPId, bool bForwards)
	{
		Creature * car = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 28821);
		// Despawn them when they reach their last WP, they are not running backwards
		if(WPId == _unit->GetAIInterface()->GetWayPointsCount())
		{
			_unit->Despawn(0, 10000);
			if( car != NULL )
				car->Despawn(0, 9900);
		}
	}
};

class CanonAI : public MoonScriptCreatureAI
{
	public:
	MOONSCRIPT_FACTORY_FUNCTION(CanonAI, MoonScriptCreatureAI);
	CanonAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnLoad()
	{
		_unit->GetAIInterface()->m_canMove = false;
	}
	void AIUpdate()
	{
		_unit->GetAIInterface()->m_canMove = false;
	}
};

// use 28957 Scarlet Crusader Test Dummy Guy to start it
//#define    GO_INCONSPICUOUS_TREE		191144
/*
bool AmbushAtTheOverlook(uint32 i, Aura* pAura, bool apply)
{
	if(pAura == NULL)
		return true;

	Player* pPlayer = pAura->GetPlayerCaster();
	if(pPlayer == NULL)
		return true;

	Creature* ScarletCourierCheck = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY() -15.0f, pPlayer->GetPositionZ(), 29076);
	if(ScarletCourierCheck == NULL)
		return true;

	QuestLogEntry* pQuest = pPlayer->GetQuestLogForEntry(12754);
	if(pQuest == NULL)
	{
		pQuest = pPlayer->GetQuestLogForEntry(10596);
		if(pQuest == NULL)
			return true;
	}

	if(apply)
	{

		GameObject* pGameObject = pPlayer->GetMapMgr()->CreateGameObject(191144);
		if(pGameObject == NULL || !pGameObject->CreateFromProto(191144, pPlayer->GetMapMgr()->GetMapId(), pPlayer->GetPositionX(), pPlayer->GetPositionY() -3.0f, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f))
			return NULL;

		pGameObject->SetInstanceID(pPlayer->GetInstanceID());
		pGameObject->Phase(PHASE_SET, pPlayer->GetPhase());
		pGameObject->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.65f);
		pGameObject->PushToWorld(pPlayer->GetMapMgr());

		sEventMgr.AddEvent(pGameObject, &GameObject::_Expire, EVENT_GAMEOBJECT_EXPIRE, 5000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	}
	return true;
}
*/


bool AmbushAtTheOverlook(uint32 i, Aura* pAura, bool apply)
{
	if(!pAura->GetPlayerCaster()) return true;


	float SSX = pAura->GetPlayerCaster()->GetPositionX();
	float SSY = pAura->GetPlayerCaster()->GetPositionY() -4.0f ;
	float SSZ = pAura->GetPlayerCaster()->GetPositionZ();
	float SSO = pAura->GetPlayerCaster()->GetOrientation();


	if(apply)
	{
		pAura->GetPlayerCaster()->GetMapMgr()->GetInterface()->SpawnGameObject(191144, SSX, SSY, SSZ, SSO, true, false, 0, pAura->GetPlayerCaster()->GetPhase());
	}else
	{
		pAura->GetPlayerCaster()->GetMapMgr()->GetInterface()->GetPlayerNearestCoords( SSX, SSY +8.0f , SSZ, 191144);
	}
	return true;

}
class AtTheOverlook : GameObjectAIScript
{
	public:
/*		ADD_GAMEOBJECT_FACTORY_FUNCTION(AtTheOverlook)
		AtTheOverlook(GameObject * goinstance) : GameObjectAIScript(goinstance) {}		*/
		AtTheOverlook(GameObject * goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript * Create(GameObject* GO)
		{
			return new AtTheOverlook(GO);
		}

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry * en = pPlayer->GetQuestLogForEntry( 12754 );
			if( !en || en->GetMobCount(0) != 0 )
				return;

			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();

			Creature * pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ);

			if(!pCreature || !pCreature->isAlive())
				return;

			if(pCreature->GetEntry() == 29076 )
			{

				pCreature->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, pPlayer->GetGUID() );
				pPlayer->CastSpell((pCreature), 52818, true);

			}
		}

		void OnSpawn()
		{
			Creature* ScarletCourier = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature( 29076, _gameobject->GetPositionX(), _gameobject->GetPositionY() -15.0f, _gameobject->GetPositionZ(), 0.0f, true, true, 0, 0, _gameobject->GetPhase());
			// do all other things directly in creature's AI class, it's more comfortable
			ScarletCourier->Root();
			ScarletCourier->SetFaction( 7);

			ScarletCourier->CreateWayPoint(uint32(1), _gameobject->GetMapId(),_gameobject->GetPositionX(), _gameobject->GetPositionY()-4.0f , _gameobject->GetPositionZ(), _gameobject->GetOrientation());
			ScarletCourier->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Hmm. What a strange tree, I Must Investigate", 3000);
			sEventMgr.AddEvent(TO_UNIT(ScarletCourier), &Creature::Unroot, EVENT_CREATURE_UPDATE, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			ScarletCourier->GetAIInterface()->StopMovement(3000);
			ScarletCourier->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What's this!? This's not a tree at all. Guards! Guards! ", 6000);
			sEventMgr.AddEvent(_gameobject, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			sEventMgr.AddEvent(TO_UNIT(ScarletCourier), &Creature::SetFaction, uint32(16), EVENT_CREATURE_UPDATE, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);


			ScarletCourier->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
			if(ScarletCourier->GetAIInterface() != NULL)
			{
				ScarletCourier->GetAIInterface()->SetAllowedToEnterCombat(true);
			} 
		}
	/*	void OnDespawn()
		{
			float SSX = _gameobject->GetPositionX();
			float SSY = _gameobject->GetPositionY() +4.0f;
			float SSZ = _gameobject->GetPositionZ();

			Player* pPlayer = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(SSX, SSY, SSZ,191144 );
			pPlayer->RemoveAura(53061, pPlayer->GetGUID());
		};*/
};

class Scarlet_Ballista_destroyed : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Scarlet_Ballista_destroyed);
	Scarlet_Ballista_destroyed(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnDied(Unit* mKiller)
	{

		Player* plr = NULL;
		QuestLogEntry *qle	= NULL;

		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); itr++) 
		{
			if((*itr)->GetTypeId() == TYPEID_PLAYER)
			{
				plr = TO_PLAYER((*itr));
				if (plr != NULL)
				{
				qle = plr->GetQuestLogForEntry(12779);
				if(qle == NULL)
					return ;
	
				//	Unit* target = _unit->GetAIInterface()->GetNextTarget();
					for(int32 i = 0; i < 10; i++)
					{
				//		target = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 29104);
						SpellEntry * info = dbcSpell.LookupEntry(53114);
						if(info != NULL)
						{
							Spell * pSpell (new Spell(plr, info, false, NULL));
							if(pSpell->p_caster != NULL)
							{
								if( qle->GetQuest()->required_mob[1] == 29104 && qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
								{
									plr->AddQuestKill(12779, i);
									break;
								}
							}
						}
					}	return;
				}
			}
		}
	}
};


class Scarlet_Soldiers_slain : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Scarlet_Soldiers_slain);
	Scarlet_Soldiers_slain(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnDied(Unit* mKiller)
	{

		Player* plr = NULL;
		QuestLogEntry *qle	= NULL;
		for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); itr++) 
		{
			if((*itr)->GetTypeId() == TYPEID_PLAYER)
			{
				plr = TO_PLAYER((*itr));
				if (plr != NULL)
				{
				qle = plr->GetQuestLogForEntry(12779);
				if(qle == NULL)
					return ;
	
				//	Unit* target = _unit->GetAIInterface()->GetNextTarget();
					for(int32 i = 0; i < 150; i++)
					{
				//		target = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 29102);
						SpellEntry * info = dbcSpell.LookupEntry(53114);
						if(info != NULL)
						{
							Spell * pSpell (new Spell(plr, info, true, NULL));
							if(pSpell->p_caster != NULL)
							{
								if( qle->GetQuest()->required_mob[0] == 29150 && qle->GetQuest()->required_mob[0] != 29104 && qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
								{
									plr->AddQuestKill(12779, i);
									break;
								}	return;
							}	return;
						}	return;
					}
				}
			}
		}
	}
};

void SetupDeathKnight(ScriptMgr* mgr)
{

	//CHAPTER 1
    mgr->register_quest_script(12593, new QuestInServiceOfLichKing);

//    mgr->register_dummy_spell(SPELL_RUNE_I, &PreparationForBattleEffect);
//    mgr->register_dummy_spell(SPELL_RUNE_II, &PreparationForBattleEffect);

    Arcemu::Gossip::Script* scourgeGryphon = new GossipScourgeGryphon();
    mgr->register_creature_gossip(29488, scourgeGryphon);
    mgr->register_creature_gossip(29501, scourgeGryphon);

	Arcemu::Gossip::Script* SGZ = new GossipScourgeGryphonThree();
	mgr->register_creature_gossip(28864, SGZ);
	
	Arcemu::Gossip::Script* SalanarSecc = new Salanar();
	mgr->register_creature_gossip(28653, SalanarSecc);
//
//	mgr->register_creature_gossip(28653, new Salanar);
//	mgr->register_creature_gossip(28653, new Salanar);
//	mgr->register_gossip_script(28653, new(Salanar));
//	GossipScript* SalanarSec = new Salanar();
//	mgr->register_gossip_script(28653, SalanarSec);

	mgr->register_creature_script(28788, &SalanarSec::Create);

	mgr->register_quest_script(12641, new EyeOfAcherusControll);		//Auto Complete Not Scripted Yet!
	mgr->register_gameobject_script(191609, &EyeofAcherusControl::Create);

	mgr->register_dummy_spell(51858, &SiphonofAcherus);

	uint32 acherusSouldPrisonEntries[] = { 191588, 191577, 191580, 191581, 191582, 191583, 191584, 191585, 191586, 191587, 191589, 191590, 0 };
	mgr->register_gameobject_script(acherusSouldPrisonEntries, &AcherusSoulPrison::Create);

	mgr->register_creature_script(CN_INITIATE_1, &UnworthyInitiate::Create);
	mgr->register_creature_script(CN_INITIATE_2, &UnworthyInitiate::Create);
	mgr->register_creature_script(CN_INITIATE_3, &UnworthyInitiate::Create);
	mgr->register_creature_script(CN_INITIATE_4, &UnworthyInitiate::Create);
	mgr->register_creature_script(CN_INITIATE_5, &UnworthyInitiate::Create);

	//CHAPTER 2
	mgr->register_hook(SERVER_HOOK_EVENT_ON_DUEL_FINISHED, (void *)&DeathsChallenge);

	mgr->register_creature_script(28406, &DKInitiateAI::Create);
	mgr->register_creature_gossip(28406, new DKInitiateGossipScript);


	//CHAPTER 2
	mgr->register_quest_script(12687, new IntoTheRealmOfShadows);		//Not Scripted fully auto completed
	mgr->register_dummy_spell(52264, &StolenHorse);

	mgr->register_dummy_spell(52362, &AcherusDeathcharger);

	mgr->register_gameobject_script(190769, &GiftoftheHarvester::Create);
	mgr->register_dummy_spell(52479, &GabedesErnters);
	mgr->register_creature_script(28845, &GhulAI::Create);
	mgr->register_creature_script(28834, &Massakre2::Create);					//Auto Complete Because of Vehicle BUg

//	uint32 massakre_persuadedEntries[]= { 28834, 28850, 28856, 0 };
//	mgr->register_creature_script(massakre_persuadedEntries, &Massakre::Create);

	//CHAPTER 3
	mgr->register_quest_script(12714, new TheWillOfLichKing);

	uint32 crusade_persuadedEntries[]= { 28940, 28936, 28939, 28610, 0 };
	mgr->register_creature_script(crusade_persuadedEntries, &crusade_persuaded::Create);

	mgr->register_creature_script(28912, &koltira_deathweaver_AI::Create);		//add say text to npc
	mgr->register_creature_script(29001, &Valroth_AI::Create);					//Needs full script
	mgr->register_quest_script(12727, new Breakout);

	uint32 special_surprise[] = { 29032, 29061, 29065, 29067, 29068, 29070, 29074, 29072, 29073, 29071, 0 };
	mgr->register_creature_script(special_surprise, &npc_a_special_surprise_AI::Create);

	mgr->register_creature_script(29076, &mob_scarlet_courier_AI::Create);

	//CHAPTER 4
	mgr->register_quest_script(12755, new Conversation_1);
	mgr->register_quest_script(12756, new Conversation_2);
	mgr->register_quest_script(12778, new Phase200);

	mgr->register_quest_script(12779, new Frostbrood);					//Vehicle BUG! Quest Gets Auto Completed

	mgr->register_quest_script(12801, new morgen);						//Not Scripted YET! quest Auto Complete

	mgr->register_quest_script(13189, new WhereKingsWalk);
	mgr->register_quest_script(13188, new WhereKingsWalk);


	mgr->register_creature_script(28576, &CitizenAI::Create);
	mgr->register_creature_script(28577, &CitizenAI::Create);

	mgr->register_creature_script(28557, &PeasantAI::Create);
	mgr->register_creature_script(28833, &CanonAI::Create);
	mgr->register_creature_script(28822, &MinerAI::Create);

	mgr->register_creature_script(28511, &TheEyesOfAcherus::Create);

	mgr->register_gameobject_script(191144, &AtTheOverlook::Create);
	mgr->register_dummy_aura(53061, &AmbushAtTheOverlook); //53061

	mgr->register_creature_script(29102, &Scarlet_Soldiers_slain::Create); // An End To All Things...
	mgr->register_creature_script(29104, &Scarlet_Ballista_destroyed::Create);
}