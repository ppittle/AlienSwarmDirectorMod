#ifndef _INCLUDED_C_MOD_BUILD_MISSION_MAP_FOR_NEXT_MISSION_H
#define _INCLUDED_C_MOD_BUILD_MISSION_MAP_FOR_NEXT_MISSION_H

#include "c_baseentity.h"

class C_MOD_Build_Mission_Map_For_Next_Mission : public C_BaseEntity
{
public:
	DECLARE_CLASS( C_MOD_Build_Mission_Map_For_Next_Mission, C_BaseEntity );
	DECLARE_CLIENTCLASS();

	int  m_iPlayerPerformance;

	C_MOD_Build_Mission_Map_For_Next_Mission(){}

	void OnMissionComplete(bool bSuccess);
	virtual void OnDataChanged(DataUpdateType_t updateType);
	void BuildMissionMapForNextMission(int playerPerformance);
	void BuildMissionMapForMission(int missionID, int playerPerformance);
	bool IsBuildingMap();
	
};

#endif // _INCLUDED_C_MOD_BUILD_MISSION_MAP_FOR_NEXT_MISSION_H