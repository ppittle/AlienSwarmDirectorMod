#include "cbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar mod_player_performance_value;

class C_MOD_build_map_proxy : public C_BaseEntity
{
public:
	DECLARE_CLASS( C_MOD_build_map_proxy, C_BaseEntity ); // generic entity class macro
	DECLARE_CLIENTCLASS(); // this is a client representation of a server class 
 
public:
	// networked variables as defined in server class
	int	m_iPlayerPerformance;

	void OnDataChanged(DataUpdateType_t updateType)
	{
		Msg("C_MOD_build_map_proxy received player performance: [%d]\n", m_iPlayerPerformance);
		mod_player_performance_value.SetValue(m_iPlayerPerformance);
	}
	
};
 
//Link a global entity name to this class (name used in Hammer etc.)
LINK_ENTITY_TO_CLASS( mod_build_map_proxy, C_MOD_build_map_proxy );
 
// Link data table DT_MyEntity to client class and map variables (RecvProps)
// DO NOT create this in the header! Put it in the main CPP file.
IMPLEMENT_CLIENTCLASS_DT( C_MOD_build_map_proxy, DT_MOD_build_map_proxy, CMOD_build_map_proxy )
	RecvPropInt( RECVINFO( m_iPlayerPerformance ) ),
END_RECV_TABLE()