#include "cbase.h"
#include "mod_build_mission_map_for_next_mission.h"
#include "mod_player_performance.h"
#include "mod_build_map_proxy.h"

LINK_ENTITY_TO_CLASS( mod_build_mission_map_for_next_mission, CMOD_Build_Mission_Map_For_Next_Mission );

BEGIN_DATADESC( CMOD_Build_Mission_Map_For_Next_Mission )
	DEFINE_FIELD( m_iPlayerPerformance, FIELD_INTEGER ),
	DEFINE_INPUTFUNC(FIELD_VOID, "Activate", Activate),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CMOD_Build_Mission_Map_For_Next_Mission, DT_MOD_Build_Mission_Map_For_Next_Mission)
	SendPropInt		(SENDINFO(m_iPlayerPerformance)),
END_SEND_TABLE()

CMOD_Build_Mission_Map_For_Next_Mission::CMOD_Build_Mission_Map_For_Next_Mission()
{
}

CMOD_Build_Mission_Map_For_Next_Mission::~CMOD_Build_Mission_Map_For_Next_Mission()
{}

int CMOD_Build_Mission_Map_For_Next_Mission::UpdateTransmitState()	// always send to all clients
{
	return SetTransmitState( FL_EDICT_ALWAYS );
}

void CMOD_Build_Mission_Map_For_Next_Mission::Activate(inputdata_t &inputData)
{
	Msg("CMOD_Build_Mission_Map_For_Next_Mission activated!  Telling client to build map.\n\n");

	//update m_iPlayerPerformance.  Engine will broadcast to clients
	m_iPlayerPerformance = CMOD_Player_Performance::PlayerPerformance()->CalculatePerformance(true);

	Msg("Player Performance: [%d]", m_iPlayerPerformance);

	/*
	CMOD_build_map_proxy * proxy = static_cast<CMOD_build_map_proxy*>(
		CreateEntityByName( "mod_build_map_proxy" )); // create an object of this entity class

	proxy->m_iPlayerPerformance = m_iPlayerPerformance;
	*/
}