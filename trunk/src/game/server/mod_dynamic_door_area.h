#ifndef _DEFINED_MOD_DYNAMIC_DOOR_AREA_H
#define _DEFINED_MOD_DYNAMIC_DOOR_AREA_H


#include "triggers.h"
//#include "asw_use_area.h"
#include "asw_door_area.h"


class CASW_Door;
class CASW_Marine;

/*
class CMOD_Dynamic_Door_Area : public CASW_Use_Area
{
	DECLARE_CLASS( CMOD_Dynamic_Door_Area, CASW_Use_Area );
public:
	CMOD_Dynamic_Door_Area();	
	virtual void ActivateMultiTrigger(CBaseEntity *pActivator);
	CASW_Door* GetASWDoor();

	bool HasWelder(CASW_Marine *pMarine);

	virtual Class_T	Classify( void ) { return (Class_T) CLASS_ASW_DOOR_AREA; }

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	float m_fNextCutCheck;	

	bool m_bHasCheckedDifficulty;
	bool m_bDifficultyThresholdReached;
	//0 = none, 1 = easy, 2 = medium, 3 = hard
	int m_iMinDifficultyThreshold;
	int m_iMaxDifficultyThreshold;

	int m_iDifficultyLevelOfMarines;
private:
	bool PerformDifficultyCheck(void);
	int GetDifficultyLevelOfMarines(void);
};
*/

class CMOD_Dynamic_Door_Area : public CASW_Door_Area //CASW_Use_Area
{
	DECLARE_CLASS( CMOD_Dynamic_Door_Area, CASW_Door_Area );

public:
	CMOD_Dynamic_Door_Area();

	void ActivateMultiTrigger(CBaseEntity *pActivator);

	DECLARE_DATADESC();

	bool m_bHasCheckedDifficulty;
	bool m_bDifficultyThresholdReached;
	//0 = none, 1 = easy, 2 = medium, 3 = hard
	int m_iMinDifficultyThreshold;
	int m_iMaxDifficultyThreshold;

	int m_iDifficultyLevelOfMarines;
private:
	bool PerformDifficultyCheck(void);
	int GetDifficultyLevelOfMarines(void);

};

#endif /* _DEFINED_MOD_DYNAMIC_DOOR_AREA_H */
