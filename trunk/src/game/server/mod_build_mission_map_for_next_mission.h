#ifndef _INCLUDED_MOD_BUILD_MISSION_MAP_FOR_NEXT_MISSION_H
#define _INCLUDED_MOD_BUILD_MISSION_MAP_FOR_NEXT_MISSION_H

#include "cbase.h"

class CMOD_Build_Mission_Map_For_Next_Mission : public CBaseEntity
{
public:

	DECLARE_CLASS( CMOD_Build_Mission_Map_For_Next_Mission, CBaseEntity );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CMOD_Build_Mission_Map_For_Next_Mission();
	~CMOD_Build_Mission_Map_For_Next_Mission();

	CNetworkVar(int, m_iPlayerPerformance);

	void Activate(inputdata_t &inputData);
	int UpdateTransmitState();
};

#endif