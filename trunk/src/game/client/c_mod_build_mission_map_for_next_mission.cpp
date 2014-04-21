#include "cbase.h"
#include "c_mod_build_mission_map_for_next_mission.h"

#include "../shared/swarm/asw_gamerules.h"
#include "asw_campaign_info.h"
#include "c_asw_campaign_save.h"
#include "missionchooser/iasw_mission_chooser_source.h"
#include "missionchooser/iasw_random_missions.h"
#include "missionchooser/iasw_map_builder.h"
#include "../missionchooser/imod_level_builder.h"
#include "c_asw_game_resource.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_CLIENTCLASS_DT(C_MOD_Build_Mission_Map_For_Next_Mission, DT_MOD_Build_Mission_Map_For_Next_Mission, CMOD_Build_Mission_Map_For_Next_Mission)
	RecvPropInt		(RECVINFO(m_iPlayerPerformance)),	
END_RECV_TABLE()

ConVar mod_player_performance_value("mod_player_performance_value", "0", 0, "Stores the players performance.");
ConVar mod_rebuild_map_on_restart("mod_rebuild_map_on_restart", "1", 0,"Rebuild maps if the player dies and has to restart.");

void C_MOD_Build_Mission_Map_For_Next_Mission::OnDataChanged(DataUpdateType_t updateType)
{
	//For some reason the engine is broadcasting a 
	//data change event at the beginning of a level
	//and m_iPlayerPerformance isn't actually updated.
	if (m_iPlayerPerformance != 0)
	{
		//Save mod_player_performance_value so it can be used later when the mission is complete.
		mod_player_performance_value.SetValue(m_iPlayerPerformance);
	}

	BaseClass::OnDataChanged(updateType);
}

void C_MOD_Build_Mission_Map_For_Next_Mission::OnMissionComplete(bool bSuccess)
{
	if (!ASWGameRules()->IsCampaignGame())
	{
		//No next mission to build.
		return;
	}

	if (mod_player_performance_value.GetInt() == 0)
	{
		Msg("WARNING: C_MOD_Build_Mission_Map_For_Next_Mission::OnMissionComplete(): mod_player_performance_value is not set.  Was a 'mod_build_mission_map_for_next_mission' entity placed in the level?\n\n");
	}

			
	CASW_Campaign_Save *pSave = ASWGameRules()->GetCampaignSave();
	if (!pSave)
	{
		Msg("Failed to load Campaign Save with CAlienSwarm::GetCampaignInfoGetCampaignSave()\n");
		return;
	}

	if (bSuccess && ASWGameRules()->CampaignMissionsLeft() > 0)
	{
		int iNextMission = pSave->m_iNumMissionsComplete + 2;	

		BuildMissionMapForMission(iNextMission, mod_player_performance_value.GetInt());
		
	}
	else if (mod_rebuild_map_on_restart.GetInt() > 0 
		&& pSave->m_iCurrentPosition > 1)
		//&& !ASWGameRules()->IsIntroMap())
		//&& ASWGameRules()->GetCampaignInfo()->GetMission(1)->

		//&& pSave->m_iNumMissionsComplete > 1) //Don't rebuild first mission, it takes too long
	{
		//Rebuild level for restart
		int iCurrentMission = pSave->m_iNumMissionsComplete + 1;

		BuildMissionMapForMission(iCurrentMission, mod_player_performance_value.GetInt());		
	}
}



void C_MOD_Build_Mission_Map_For_Next_Mission::BuildMissionMapForMission(int missionID, int playerPerformance)
{
	Msg("Sever indicated mission is complete.  Building a new map.\n");
	CASW_Campaign_Info *pCampaign = ASWGameRules()->GetCampaignInfo();		
	if (!pCampaign)
	{
		Msg("Failed to load Campaign with CAlienSwarm::GetCampaignInfo()\n");
		return;
	}

	CASW_Campaign_Info::CASW_Campaign_Mission_t* pNextMission = 
			pCampaign->GetMission(missionID);
	if (!pNextMission)
	{
		Msg("Failed to load next campaign mission with pCampaign->GetMission(iNextMission).  Is this the end of the campaign?\n");
		return;
	}

	missionchooser->modLevel_Builder()->BuildMapForMissionFromLayoutFile(
		pNextMission->m_MapName, playerPerformance);
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
		//C_MOD_Build_Mission_Map_For_Next_Mission * pBuilder = new C_MOD_Build_Mission_Map_For_Next_Mission();
		//pBuilder->BuildMissionMapForNextMission(atoi(args.Arg(1)));	
	}
	else 
	{
		missionchooser->modLevel_Builder()->BuildMapForMissionFromLayoutFile(
			args.Arg(2), atoi(args.Arg(1)));
	}		
}


void CC_MOD_Build_Mission_Map_For_Next_Mission_Layout_Only( const CCommand &args )
{
	if ( args.ArgC() < 2 )
	{
		Msg("Usage: mod_build_mission_map_for_next_mission <player performance> <map name>\n");
		return;
	}

	if (args.ArgC() == 2)
	{
		//C_MOD_Build_Mission_Map_For_Next_Mission * pBuilder = new C_MOD_Build_Mission_Map_For_Next_Mission();
		//pBuilder->BuildMissionMapForNextMission(atoi(args.Arg(1)));	
	}
	else 
	{
		missionchooser->modLevel_Builder()->BuildMapForMissionFromLayoutFile(
			args.Arg(2), atoi(args.Arg(1)), false);
	}		
}

bool C_MOD_Build_Mission_Map_For_Next_Mission::IsBuildingMap(){
	return missionchooser->modLevel_Builder()->IsBuildingLevel();
}

static ConCommand mod_build_mission_map_for_next_mission("mod_build_mission_map_for_next_mission", CC_MOD_Build_Mission_Map_For_Next_Mission, 0 );
static ConCommand mod_build_mission_map_for_next_mission_layout_only("mod_build_mission_map_for_next_mission_layout_only", CC_MOD_Build_Mission_Map_For_Next_Mission_Layout_Only, 0 );