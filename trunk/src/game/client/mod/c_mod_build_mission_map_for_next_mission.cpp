#include "cbase.h"
#include "c_mod_build_mission_map_for_next_mission.h"

#include "../shared/swarm/asw_gamerules.h"
#include "asw_campaign_info.h"
#include "c_asw_campaign_save.h"
#include "missionchooser/iasw_mission_chooser_source.h"
#include "missionchooser/iasw_random_missions.h"
#include "missionchooser/iasw_map_builder.h"
#include "../missionchooser/imod_level_builder.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_CLIENTCLASS_DT(C_MOD_Build_Mission_Map_For_Next_Mission, DT_MOD_Build_Mission_Map_For_Next_Mission, CMOD_Build_Mission_Map_For_Next_Mission)
	RecvPropInt		(RECVINFO(m_iPlayerPerformance)),	
END_RECV_TABLE()

ConVar mod_player_performance_value("mod_player_performance_value", "0", 0, "Stores the players performance.");

C_MOD_Build_Mission_Map_For_Next_Mission::C_MOD_Build_Mission_Map_For_Next_Mission()
{
	
}

void C_MOD_Build_Mission_Map_For_Next_Mission::OnMissionComplete(bool bSuccess)
{
	if (bSuccess)
	{
		if (mod_player_performance_value.GetInt() > 0)
		{
			BuildMissionMapForNextMission(mod_player_performance_value.GetInt());
		}
		else
		{
			Msg("WARNING: C_MOD_Build_Mission_Map_For_Next_Mission::OnMissionComplete(): mod_player_performance_value is not set.  Was a 'mod_build_mission_map_for_next_mission' entity placed in the level?\n\n");
		}
	}
}

void C_MOD_Build_Mission_Map_For_Next_Mission::OnDataChanged(DataUpdateType_t updateType)
{
	//Save mod_player_performance_value so it can be used later when the mission is complete.
	mod_player_performance_value.SetValue(m_iPlayerPerformance);

	BaseClass::OnDataChanged(updateType);
}

void C_MOD_Build_Mission_Map_For_Next_Mission::BuildMissionMapForNextMission(int playerPerformance)
{
	Msg("Sever indicated mission is complete.  Building map for next mission.\n");

	if (ASWGameRules()->CampaignMissionsLeft() <= 0)
	{
		//This is the last mission.
		return;
	}

	CASW_Campaign_Info *pCampaign = ASWGameRules()->GetCampaignInfo();		
	if (!pCampaign)
	{
		Msg("Failed to load Campaign with CAlienSwarm::GetCampaignInfo()\n");
		return;
	}
		
	CASW_Campaign_Save *pSave = ASWGameRules()->GetCampaignSave();
	if (!pSave)
	{
		Msg("Failed to load Campaign Save with CAlienSwarm::GetCampaignInfoGetCampaignSave()\n");
		return;
	}

	int iNextMission = pSave->m_iNumMissionsComplete + 2;	
	CASW_Campaign_Info::CASW_Campaign_Mission_t* pNextMission = 
		pCampaign->GetMission(iNextMission);
	if (!pNextMission)
	{
		Msg("Failed to load next campaign mission with pCampaign->GetMission(iNextMission)\n");
		return;
	}
	
	missionchooser->modLevel_Builder()->BuildMapForMissionFromLayoutFile(
		pNextMission->m_MapName, playerPerformance);

}

void CC_MOD_Build_Mission_Map_For_Next_Mission( const CCommand &args )
{
	if ( args.ArgC() < 2 )
	{
		Msg("Usage: mod_build_mission_map_for_next_mission <player performance> <map name>\n");
		return;
	}

	if (args.ArgC() == 2)
	{
		C_MOD_Build_Mission_Map_For_Next_Mission * pBuilder = new C_MOD_Build_Mission_Map_For_Next_Mission();
		pBuilder->BuildMissionMapForNextMission(atoi(args.Arg(1)));	
	}
	else 
	{
		missionchooser->modLevel_Builder()->BuildMapForMissionFromLayoutFile(
			args.Arg(2), atoi(args.Arg(1)));
	}		
}

bool C_MOD_Build_Mission_Map_For_Next_Mission::IsBuildingMap(){
	return missionchooser->modLevel_Builder()->IsBuildingLevel();
}

static ConCommand asw_build_map("mod_build_mission_map_for_next_mission", CC_MOD_Build_Mission_Map_For_Next_Mission, 0 );