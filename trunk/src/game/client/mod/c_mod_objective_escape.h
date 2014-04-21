#ifndef _INCLUDED_C_MOD_OBJECTIVE_ESCAPE_H
#define _INCLUDED_C_MOD_OBJECTIVE_ESCAPE_H

#include "c_asw_objective.h"

class C_MOD_Objective_Escape : public C_ASW_Objective
{
public:
	DECLARE_CLASS( C_MOD_Objective_Escape, C_ASW_Objective );
	DECLARE_CLIENTCLASS();

	CNetworkVar(int, m_iPlayerPerformance);

	C_MOD_Objective_Escape();

	virtual void OnDataChanged(DataUpdateType_t updateType);
	void BuildMissionMapForNextMission();
};
#endif // _INCLUDED_C_MOD_OBJECTIVE_ESCAPE_H