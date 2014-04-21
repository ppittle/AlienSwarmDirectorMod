#include "cbase.h"
#include "c_mod_objective_escape.h"
#include "../shared/swarm/asw_gamerules.h"
#include "asw_campaign_info.h"
#include "c_asw_campaign_save.h"
#include "missionchooser/iasw_mission_chooser_source.h"
#include "missionchooser/iasw_random_missions.h"
#include "missionchooser/iasw_map_builder.h"
#include "../missionchooser/imod_level_builder.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_CLIENTCLASS_DT(C_MOD_Objective_Escape, DT_MOD_Objective_Escape, CMOD_Objective_Escape)
	RecvPropInt		(RECVINFO(m_iPlayerPerformance)),	
END_RECV_TABLE()

C_MOD_Objective_Escape::C_MOD_Objective_Escape()
{
}

void C_MOD_Objective_Escape::OnDataChanged(DataUpdateType_t updateType)
{
	BuildMissionMapForNextMission();
	
	BaseClass::OnDataChanged(updateType);
}

void C_MOD_Objective_Escape::BuildMissionMapForNextMission()
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

	int iNextMission = pSave->m_iNumMissionsComplete + 1;	
	CASW_Campaign_Info::CASW_Campaign_Mission_t* pNextMission = 
		pCampaign->GetMission(iNextMission);
	if (!pNextMission)
	{
		Msg("Failed to load next campaign mission with pCampaign->GetMission(iNextMission)\n");
		return;
	}
	
	missionchooser->modLevel_Builder()->BuildMapForMissionFromLayoutFile(
		pNextMission->m_MapName, m_iPlayerPerformance);

}