#include "cbase.h"
#include "c_mod_marker.h"
#include "c_asw_marker.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


//IMPLEMENT_AUTO_LIST( IObjectiveMarkerList );

class C_MOD_Marker : public C_ASW_Marker
{
	DECLARE_CLASS( C_MOD_Marker, C_ASW_Marker );
	DECLARE_CLIENTCLASS();
};

IMPLEMENT_CLIENTCLASS_DT(C_MOD_Marker, DT_MOD_Marker, CMOD_Marker)
	RecvPropString( RECVINFO( m_ObjectiveName ) ),
	RecvPropInt( RECVINFO( m_nMapWidth ) ),
	RecvPropInt( RECVINFO( m_nMapHeight ) ),
	RecvPropBool( RECVINFO( m_bComplete ) ),
	RecvPropBool( RECVINFO( m_bEnabled ) ),
END_RECV_TABLE()