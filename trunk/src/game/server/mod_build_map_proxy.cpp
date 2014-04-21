#include "cbase.h"
#include "mod_build_map_proxy.h"

//Link a global entity name to this class (name used in Hammer etc.)
LINK_ENTITY_TO_CLASS( mod_build_map_proxy, CMOD_build_map_proxy );
 
// Server data table describing networked member variables (SendProps)
// DO NOT create this in the header! Put it in the main CPP file.
IMPLEMENT_SERVERCLASS_ST( CMOD_build_map_proxy, DT_MOD_build_map_proxy )
	SendPropInt(	SENDINFO( m_iPlayerPerformance ), 8, SPROP_UNSIGNED ),
END_SEND_TABLE()

int CMOD_build_map_proxy::UpdateTransmitState()	// always send to all clients
{
	return SetTransmitState( FL_EDICT_ALWAYS );
}
