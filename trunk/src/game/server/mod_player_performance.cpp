#include "cbase.h"
#include "asw_director.h"
#include "asw_game_resource.h"
#include "asw_marine_resource.h"
#include "asw_marine.h"
#include "asw_player.h"
#include "mod_player_performance.h"
#include "mod_player_performance_calculators.h"
#include "asw_marine_profile.h"
#include <vector>
#include "platform.h"
#include "asw_campaign_save.h"
#include "asw_campaign_info.h"



// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace std;

//returned when Performance can not be calculated
#define DEFAULT_PERFORMANCE 1

//Couldn't get bots or multiplayer to work properly, so change how score is calculated
#define SINGLE_PLAYER_MODE true

ConVar mod_player_performance_debug("mod_player_performance_debug", "0", 0, "Displays modPlayerPerformance status on screen");

ConVar mod_player_performance_force_value("mod_player_performance_force_value", "0", 0, "Permantently sets modPlayerPerformance, overriding the calculated value.");

ConVar mod_player_performance_threshold_high("mod_player_performance_threshold_high", "80", 0, "Player performance neccessary to achieve a performance ranking of '3' (high).");
ConVar mod_player_performance_threshold_medium("mod_player_performance_threshold_medium", "60", 0, "Player performance neccessary to achieve a performance ranking of '2' (medium).");

ConVar mod_player_performance_starting_value("mod_player_performance_starting_value", "79", 0, "The player performance at the beginning of a level.");

CMOD_Player_Performance* CMOD_Player_Performance::g_PlayerPerformanceSingleton = 0;

CMOD_Player_Performance* CMOD_Player_Performance::PlayerPerformance()
{	
	if (!g_PlayerPerformanceSingleton)
	{
		g_PlayerPerformanceSingleton = new CMOD_Player_Performance();		
	}

	return g_PlayerPerformanceSingleton;
}

CMOD_Player_Performance::CMOD_Player_Performance( void ) : CAutoGameSystemPerFrame( "CMOD_Player_Performance" )
{
	m_previousRating = 0;
	m_lastLevelWeightedRating = 0;

	g_calculators = new vector<CMOD_Player_Performance_Calculator*>();	
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_Health(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_Accuracy(SINGLE_PLAYER_MODE));	
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_DirectorStress(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_PlayTime(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_AlienLifeTime(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_EnemyKillBonus(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_FastReload(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_DodgeRanger(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_MeleeKills(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_BoomerKillEarly(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_RestartPenalty(SINGLE_PLAYER_MODE));
	g_calculators->push_back(new CMOD_Player_Performance_Calculator_NewLevelModifier(SINGLE_PLAYER_MODE));

	if (!SINGLE_PLAYER_MODE)
		g_calculators->push_back(new CMOD_Player_Performance_Calculator_FriendlyFire(SINGLE_PLAYER_MODE));
}

CMOD_Player_Performance::~CMOD_Player_Performance()
{
	if (g_calculators)
		delete g_calculators;
}

//Couldn't figure out how to nativally hook into this event, 
//so asw_director.cpp calls 
void CMOD_Player_Performance::OnMissionStarted(){		

	int retries = 1;

	//Game will crash if we call GetRetries()
	//and we're not in a campaign.
	if (ASWGameResource()->IsCampaignGame())
		retries = ASWGameResource()->GetCampaignSave()->GetRetries();

	for (unsigned int i = 0; i < g_calculators->size(); i++)
	{
		//Msg("Telling calculator [%i] MissionStarted\n", i);
		g_calculators->at(i)->OnMissionStarted(m_lastLevelWeightedRating, 
			retries);
	}
}

//Couldn't figure out how to nativally hook into this event, 
//so asw_director.cpp calls 
void CMOD_Player_Performance::FrameUpdatePostEntityThink()
{	
	int rating = CalculatePerformanceButDoNotUpdateHUD(false);

	//if (rating != m_previousRating) - WriteToHUD was firing before HUD was ready to receive, so HUD would
	//be out of date on level start.
		WriteToHUD("MODPlayerPerformance", rating);

	m_previousRating = rating;
	
	if ( mod_player_performance_debug.GetInt() > 0 )
	{		
		PrintDebug();		
	}
}

void CMOD_Player_Performance::Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info )
{
	for (unsigned int i = 0; i < g_calculators->size(); i++)
	{
		g_calculators->at(i)->Event_AlienKilled(pAlien, info);
	}
}

int CMOD_Player_Performance::CalculatePerformance(bool isEndOfLevel)
{
	WriteToHUD("MODPlayerPerformanceDynamicContent", -1);
	float rating = CalculatePerformanceButDoNotUpdateHUD(isEndOfLevel);

	if (isEndOfLevel)
		m_lastLevelWeightedRating = rating;

	return rating;
}

int CMOD_Player_Performance::CalculatePerformanceButDoNotUpdateHUD(bool isEndOfLevel)
{
	//if (mod_player_performance_force_value.GetInt() > 0)
	//{
		//Don't return here, go ahead and calculate the value in case debug is on.
	//}

	CASW_Game_Resource *pGameResource = ASWGameResource();

	if (!pGameResource)
	{
		Msg("Failed to calculate performance: Couldn't get ASWGameResource");
		return DEFAULT_PERFORMANCE;
	}	

	m_totalRating = mod_player_performance_starting_value.GetInt();
	for (unsigned int i = 0; i < g_calculators->size(); i++)
	{
		g_calculators->at(i)->UpdatePerformance(&m_totalRating, isEndOfLevel, pGameResource);
	}
	
	if (m_totalRating >= mod_player_performance_threshold_high.GetInt())
		m_weightedRating = 3;
	else if (m_totalRating > mod_player_performance_threshold_medium.GetInt())
		m_weightedRating = 2;
	else 
		m_weightedRating = 1;	

	if (mod_player_performance_force_value.GetInt() > 0)
	{		
		return mod_player_performance_force_value.GetInt();
	}
	else
	{	
		return m_weightedRating;
	}
}

void CMOD_Player_Performance::PrintDebug()
{
	int screenOffset = 0;

	engine->Con_NPrintf(screenOffset++,"Players Performance: %d", m_weightedRating);
	engine->Con_NPrintf(screenOffset++,"Players Total Rating: %0.02f", m_totalRating);
	screenOffset++;
	for (unsigned int i = 0; i < g_calculators->size(); i++)	
	{
		g_calculators->at(i)->PrintDebugString( screenOffset++ );		
	}	
	screenOffset++;
	for (unsigned int i = 0; i < g_calculators->size(); i++)
	{
		if (g_calculators->at(i)->HasExtraDebugInfo())
		{
			g_calculators->at(i)->PrintExtraDebugInfo(screenOffset++);			
		}
	}	

	if (mod_player_performance_force_value.GetInt() > 0)
	{
		engine->Con_NPrintf(screenOffset++,"Player Forced Rating: ON [%d]", mod_player_performance_force_value.GetInt());
	}
	else
	{
		engine->Con_NPrintf(screenOffset++,"Player Forced Rating: OFF");
	}	
}

void CMOD_Player_Performance::WriteToHUD(const char* messagename, int rating)
{	
	for ( int i=0;i<ASWGameResource()->GetMaxMarineResources();i++ )
	{
		CASW_Marine_Resource *pMR = ASWGameResource()->GetMarineResource(i);
		if ( !pMR )
			continue;
				
		CASW_Player *pPlayer = pMR->GetCommander();
	
		if ( pPlayer && pPlayer->GetMarine() )
		{		
			CSingleUserRecipientFilter user( pPlayer );
			UserMessageBegin( user, messagename );
			WRITE_SHORT( rating );
			MessageEnd();			
		}	
	}
}
