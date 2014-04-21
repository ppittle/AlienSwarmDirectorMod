#ifndef _INCLUDED_MOD_PLAYER_CALCULATORS_H
#define _INCLUDED_MOD_PLAYER_CALCULATORS_H

class CASW_Game_Resource;
class CBaseEtnity;
class CTakeDamageInfo;
class IGameEvent;

#include <vector>
#include "igameevents.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator{
protected:
	CMOD_Player_Performance_Calculator(bool bIsSignlePlayerMode){
		m_bIsSinglePlayerMode = bIsSignlePlayerMode;
		m_LastCalculatedValue = 0;
	}

	float m_LastCalculatedValue;
	bool m_bIsSinglePlayerMode;
	char * m_DebugName;

public:
	virtual void OnMissionStarted(float lastLevelRating, int numRetries){}
	virtual void Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info ){};

	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource) = 0;	
	bool IsSinglePlayerMode(){return m_bIsSinglePlayerMode;}

	virtual void PrintDebugString(int offset);
	virtual double GetDebugValue(){return m_LastCalculatedValue;}	
	virtual bool HasExtraDebugInfo(){ return false;}
	virtual void PrintExtraDebugInfo(int offset){}		
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_Health : public CMOD_Player_Performance_Calculator
{
public:	
	CMOD_Player_Performance_Calculator_Health(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{	
		m_DebugName = "Health";	
		m_averageHealth = 0;
		m_fullHealth = 0;
		m_hasCalculatedFullHealth = false;
	}

	int m_averageHealth;
	int m_fullHealth;
	bool m_hasCalculatedFullHealth;
		
	virtual void OnMissionStarted(float lastLevelRating, int numRetries);
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);
	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_Accuracy : public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_Accuracy(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Accuracy";		
	}

	//The accuracy of the primay player.
	float m_PlayerZeroAccuracy;

	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);
	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_FriendlyFire : public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_FriendlyFire(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Friendly Fire";		
	}
	
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_DirectorStress : public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_DirectorStress(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Dicrector Stress";	
		m_HasFinishedCoolingDown = true;				
		m_CoolDownStartTime = 0;		
		m_CoolDownTimeLeft = 0;
		m_previousStressOfPlayers = 0;
	}

	vector<float>* g_directorStressHistory;

	float m_averageStressOfPlayers, m_averageStressHistory;
	float m_previousStressOfPlayers;

	float m_CoolDownTimeLeft;
	float m_CoolDownStartTime;	
	bool m_HasFinishedCoolingDown;	

	virtual void OnMissionStarted(float lastLevelRating, int numRetries);		
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);

	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);

	virtual float CalculateAverageStress(CASW_Game_Resource *pGameResource);
	virtual float CalculateStressPenalty(float averageStress, float previousStress);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_PlayTime : public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_PlayTime(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Play Time";		
		m_MissionStartTime = 0.0;
	}

	float m_MissionStartTime;
	
	virtual void OnMissionStarted(float lastLevelRating, int numRetries);
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);

	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);	
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_AlienLifeTime : public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_AlienLifeTime(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Alien Avg Life Time";
		m_totalAlienLifeTime = 0.0f;
		m_numberOfAliensKilled = 0;
	}

	float m_totalAlienLifeTime;
	int m_numberOfAliensKilled;
	
	virtual void OnMissionStarted(float lastLevelRating, int numRetries);

	virtual void Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info );
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);

	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_FastReload : public CMOD_Player_Performance_Calculator, CGameEventListener
{
public:
	//constructor in cpp file
	CMOD_Player_Performance_Calculator_FastReload(bool bIsSignlePlayerMode);
	~CMOD_Player_Performance_Calculator_FastReload();

	int m_NumberOfFastReloads;

	virtual void OnMissionStarted(float lastLevelRating, int numRetries);
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);

	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);

	//IGameEventListenere2 methods
	virtual void FireGameEvent( IGameEvent *event );	

};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_DodgeRanger : public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_DodgeRanger(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Dodge Ranger";		
	}
	
	bool m_HasDodgedRanger;

	virtual void OnMissionStarted(float lastLevelRating, int numRetries);
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_MeleeKills : public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_MeleeKills(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Melee Kills";	
		m_numberOfAliensKilled = 0;
		m_numberOfMeleeKills = 0;
	}
	
	int m_numberOfAliensKilled;
	int m_numberOfMeleeKills;

	virtual void OnMissionStarted(float lastLevelRating, int numRetries);

	virtual void Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info );
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);

	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_BoomerKillEarly : public CMOD_Player_Performance_Calculator
{
public:

	//constructor in cpp file

	CMOD_Player_Performance_Calculator_BoomerKillEarly(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Early Boomer Kills";		
	}
	
	bool m_HasBoomerKillEarly;

	virtual void OnMissionStarted(float lastLevelRating, int numRetries);
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_RestartPenalty : 
	public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_RestartPenalty::CMOD_Player_Performance_Calculator_RestartPenalty(bool bIsSignlePlayerMode)
	:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Restart Penalty";
		m_MissionStartTime = 0.0f;
	}
	
	float m_MissionStartTime;
	
	virtual void OnMissionStarted(float lastLevelRating, int numRetries);
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);

	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_NewLevelModifier : 
	public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_NewLevelModifier::CMOD_Player_Performance_Calculator_NewLevelModifier(bool bIsSignlePlayerMode)
	:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "New Level Modifier";
		m_MissionStartTime = 0.0f;
	}
	
		
	float m_MissionStartTime;
	
	virtual void OnMissionStarted(float lastLevelRating, int numRetries);
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);

	virtual bool HasExtraDebugInfo(){ return true;}
	virtual void PrintExtraDebugInfo(int offset);	
};

//////////////////////////////////////////////////////////////////////////////////

class CMOD_Player_Performance_Calculator_EnemyKillBonus : 
	public CMOD_Player_Performance_Calculator
{
public:
	CMOD_Player_Performance_Calculator_EnemyKillBonus::CMOD_Player_Performance_Calculator_EnemyKillBonus(bool bIsSignlePlayerMode)
	:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
	{
		m_DebugName = "Enemy Kill Bonus";	
		m_EnemiesKilled = 0;
	}
			
	int m_EnemiesKilled;
	
	virtual void Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info );
	virtual void OnMissionStarted(float lastLevelRating, int numRetries);
	virtual void UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource);	
};



#endif