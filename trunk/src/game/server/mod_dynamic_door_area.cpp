#include "cbase.h"
#include "mod_dynamic_door_area.h"
#include "asw_marine.h"
#include "asw_game_resource.h"
#include "asw_marine_resource.h"
#include "asw_marine_speech.h"
#include "asw_gamerules.h"
#include "asw_weapon.h"
#include "asw_util_shared.h"
#include "asw_melee_system.h"
#include "mod_player_performance.h"
/*
#include "asw_door.h"
#include "asw_marine.h"
#include "asw_game_resource.h"
#include "asw_marine_resource.h"
#include "asw_weapon_welder_shared.h"
#include "asw_marine_speech.h"
#include "asw_fail_advice.h"
*/

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( mod_dynamic_door_area, CMOD_Dynamic_Door_Area );

BEGIN_DATADESC( CMOD_Dynamic_Door_Area )

	DEFINE_KEYFIELD(m_iMinDifficultyThreshold, FIELD_INTEGER, "minDifficultyThreshold")	,
	DEFINE_KEYFIELD(m_iMaxDifficultyThreshold, FIELD_INTEGER, "maxDifficultyThreshold")
	
END_DATADESC()

CMOD_Dynamic_Door_Area::CMOD_Dynamic_Door_Area()
{
}

void CMOD_Dynamic_Door_Area::ActivateMultiTrigger(CBaseEntity *pActivator)
{ 
	if (GetNextThink() > gpGlobals->curtime)
		return;         // still waiting for reset time

	//If we haven't calculated difficulty, do it now.
	if (!m_bHasCheckedDifficulty)
	{
		m_bHasCheckedDifficulty = true;
		m_iDifficultyLevelOfMarines = GetDifficultyLevelOfMarines();
		m_bDifficultyThresholdReached = PerformDifficultyCheck();
	}

	//If the marines aren't in the threshold, return.
	if (!m_bDifficultyThresholdReached)		
		return;

	//Otherwise fire the trigger to open the door.
	BaseClass::ActivateMultiTrigger(pActivator);	
}

int CMOD_Dynamic_Door_Area::GetDifficultyLevelOfMarines( void )
{	
	if (!CMOD_Player_Performance::PlayerPerformance())
	{
		Msg("Failed to get player performance from CMOD_Player_Performance.  Using default");
		return 1;
	}
	
	return CMOD_Player_Performance::PlayerPerformance()->CalculatePerformance();
}

bool CMOD_Dynamic_Door_Area::PerformDifficultyCheck( void )
{
	if (m_iMinDifficultyThreshold <= 0 && m_iMaxDifficultyThreshold >= m_iDifficultyLevelOfMarines)
		return true;
	else if (m_iMaxDifficultyThreshold <= 0 && m_iMinDifficultyThreshold <= m_iDifficultyLevelOfMarines)
		return true;
	else if (m_iDifficultyLevelOfMarines >= m_iMinDifficultyThreshold &&
		m_iDifficultyLevelOfMarines <= m_iMaxDifficultyThreshold)	
		return true;	
	else
		return false;
}