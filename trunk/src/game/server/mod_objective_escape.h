#ifndef _INCLUDED_MOD_OBJECTIVE_ESCAPE_H
#define _INCLUDED_MOD_OBJECTIVE_ESCAPE_H

#include "asw_objective.h"

class CBaseTrigger;

class CMOD_Objective_Escape : public CASW_Objective
{
public:
	DECLARE_CLASS( CMOD_Objective_Escape, CASW_Objective );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CMOD_Objective_Escape();
	~CMOD_Objective_Escape();
	 
	void Spawn();
	void CheckEscapeStatus();
	bool OtherObjectivesComplete();
	bool AllLiveMarinesInExit();
	void InputMarineInEscapeArea( inputdata_t &inputdata );

	//void BuildMapForNextMission();
	CNetworkVar(int, m_iPlayerPerformance);

	CBaseTrigger* GetTrigger();
	EHANDLE m_hTrigger;	
};

extern CUtlVector<CMOD_Objective_Escape*> g_aEscapeObjectives;

#endif /* _INCLUDED_MOD_OBJECTIVE_ESCAPE_H */