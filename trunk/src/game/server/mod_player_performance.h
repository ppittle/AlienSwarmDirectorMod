#ifndef _INCLUDED_MOD_PLAYER_PERFORMANCE_H
#define _INCLUDED_MOD_PLAYER_PERFORMANCE_H

#include "asw_game_resource.h"

#include <vector>
using namespace std;

class CMOD_Player_Performance_Calculator;
class CBaseEntity;
class  CTakeDamageInfo;

class CMOD_Player_Performance : CAutoGameSystemPerFrame
{
	CMOD_Player_Performance();
	~CMOD_Player_Performance();
	static CMOD_Player_Performance* g_PlayerPerformanceSingleton;
		
	vector<CMOD_Player_Performance_Calculator*>* g_calculators;

public:
	//Singleton accessor
	static CMOD_Player_Performance* PlayerPerformance();

	float m_totalRating, m_previousRating, m_lastLevelWeightedRating;
	int m_weightedRating;	
	
	virtual void FrameUpdatePostEntityThink();

	int CalculatePerformance(){return CalculatePerformance(false);}
	int CalculatePerformance(bool isEndOfLevel);
	int CalculatePerformanceButDoNotUpdateHUD(bool isEndOfLevel);

	
	void PrintDebug();
	void WriteToHUD(const char* messagename, int rating);

	//Called by asw_director
	void OnMissionStarted();
	void Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info );
};

#endif /* _INCLUDED_MOD_PLAYER_PERFORMANCE_H */
