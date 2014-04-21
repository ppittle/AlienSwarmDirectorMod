#ifndef _INCLUDED_MOD_DYNAMICDIFFICULTYMODIFIERTRIGGER_H
#define _INCLUDED_MOD_DYNAMICDIFFICULTYMODIFIERTRIGGER_H

#include "triggers.h"

//Just like a CASW_Marine_Position_Trigger
//but only fires if the players are within the 
//difficulty threshold.
class CMOD_Dynamic_Difficulty_Modifier_Trigger : public CBaseTrigger
{
	DECLARE_CLASS ( CMOD_Dynamic_Difficulty_Modifier_Trigger, CBaseTrigger);
public:		
	void PositionTouch(CBaseEntity *pOther);
	void ActivatePositionTrigger(CBaseEntity *pActivator);
	void Spawn( void );	
	void MultiWaitOver( void );

	DECLARE_DATADESC();
	
	//0 = none, 1 = easy, 2 = medium, 3 = hard
	int m_iMinDifficultyThreshold;
	int m_iMaxDifficultyThreshold;

	int m_iDifficultyLevelOfMarines;
	//Maximum number of times to fire triggers.
	//This is will fire all triggers m_iMaxTriggerRecalculatePerformanceAndFire
	//times.  Performance is recalcualated for each trigger fire.
	int m_iMaxTriggerRecalculatePerformanceAndFire;
	int m_iTriggerFireCount;


	// Outputs
	COutputEvent m_OnTrigger;
	COutputEvent m_TriggerEasy;
	COutputEvent m_TriggerMedium;
	COutputEvent m_TriggerHard;
	//Fired if Performance is at least 1
	COutputEvent m_TriggerAtleastEasy;
	//Fired if Performance is at least 2
	COutputEvent m_TriggerAtleastMedium;
	//Fired if Performance is at least 3
	COutputEvent m_TriggerAtleastHard;

	//inputs
	void InputSetMaxFireCount( inputdata_t &inputdata );

	EHANDLE m_hMarine;
private:
	bool PerformDifficultyCheck(void);
	int GetDifficultyLevelOfMarines(void);
};


#endif
