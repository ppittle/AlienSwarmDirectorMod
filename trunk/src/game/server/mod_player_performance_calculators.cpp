#include "cbase.h"
#include "mod_player_performance_calculators.h"

#include "asw_director.h"
#include "asw_game_resource.h"
#include "asw_marine_resource.h"
#include "asw_marine.h"
#include "asw_player.h"
#include "mod_player_performance.h"
#include "asw_marine_profile.h"
#include "asw_alien.h"

#include <vector>
using namespace std;

//Multiplies the average accuracy.  If averageAccuracy is 25% and ACCURACY_MODIFIER is 4
//CalculateAccuracyRating will return a 100 (perfect score)
#define ACCURACY_MODIFIER 1

//Amount of Friendly Fire per Marine that's allowed before a penalty
#define FRIENDLYFIRE_HANDICAP 10.0

#define FRIENDLYFIRE_MULTIPLIER 3.0

#define MAX_DIRECTOR_STRESS_HISTORY_SIZE 1500

#define DIRECTOR_STRESS_MULTIPLIER 4.5

//This counters that effect of stress, end of levels
//usually have 'finales' which result in the players
//being 100% stressed. This reduces the overall impact
//of prolonged high stress on performance metrics.
#define STRESS_MULTIPLIER 0.3

ConVar mod_player_performance_health_critical_threshold(			"mod_player_performance_health_critical_threshold", "40", 0, "This threshold indicates if health_normal_penalty or health_critical_penalty is used to calculate health penalty.  Note: This is health lost, not health remaning");
ConVar mod_player_performance_health_normal_penalty(				"mod_player_performance_health_normal_penalty", "0.4", 0, "The defualt performance penalty for lost health.  This is multipled by each HP lost.");
ConVar mod_player_performance_health_critical_penalty(				"mod_player_performance_health_critical_penalty", "0.6", 0, "The critical performance penalty for lost health.  This is multipled by each HP lost.");
ConVar mod_player_performance_accuracy_threshold(					"mod_player_performance_accuracy_threshold", "92", 0, "The threshold where a players accuracy has a positive or negative impact on performance.  Above this threshold and performance gets a boost, below and there is a penalty.");
ConVar mod_player_performance_accuracy_floor(						"mod_player_performance_accuracy_floor", "82", 0, "The floor for caculating an accuracy penalty.  Ie, this is the lowest value that is used to assess a penalty.");
ConVar mod_player_performance_accuracy_performance_modifier(		"mod_player_performance_accuracy_performance_modifier", "0.5", 0, "Multipled against the players weighted accuracy to get the performance modifier. Weighted accuracy is difference between accuracy threshold and actual accuracy.");
ConVar mod_player_performance_director_stress_threshold(			"mod_player_performance_director_stress_threshold", "9.5", 0, "The threshold when stress, as measured by the director, engenders a player performance penalty.");
ConVar mod_player_performance_director_stress_critical_threshold(	"mod_player_performance_director_stress_critical_threshold", "17", 0, "The threshold when stress, as measured by the director, engenders an extra player performance penalty.");
ConVar mod_player_performance_director_stress_penalty(				"mod_player_performance_director_stress_penalty", "10", 0, "The one time penalty assesed when director stress threshold is reached.");
ConVar mod_player_performance_director_stress_critical_penalty(		"mod_player_performance_director_stress_critical_penalty", "20", 0, "The one time penalty assesed when critical director stress threshold is reached.");
ConVar mod_player_performance_director_stress_cooldown_time(		"mod_player_performance_director_stress_cooldown_time", "20", 0, "The amount of time it takes for the director stress penalty to degrade.");
ConVar mod_player_performance_playtime_par(							"mod_player_performance_playtime_par", "270", 0, "The average amount of time a player is expected to complete a level.");
ConVar mod_player_performance_playtime_divisor_bonus(				"mod_player_performance_playtime_par_bonus", "15", 0, "The boost to performance a player receives for completing a mission under par.  For each '60' seconds under par, the player's performance is increased by 1 point.");
ConVar mod_player_performance_enemy_life_time_positive_threshold(	"mod_player_performance_enemy_life_time_positive_threshold", "4", 0, "If the average enemy life expectency is less than this, a bonus is awarded.");
ConVar mod_player_performance_enemy_life_time_positive_bonus(		"mod_player_performance_enemy_life_time_positive_bonus", "0.8", 0, "The bonus to award if enemy life expectency is less than the positive threshold.");
ConVar mod_player_performance_enemy_life_time_negative_threshold(	"mod_player_performance_enemy_life_time_negative_threshold", "15", 0, "If the average enemy life expectency is more than this, a penalty is assesed.");
ConVar mod_player_performance_enemy_life_time_negative_penalty(		"mod_player_performance_enemy_life_time_negative_penalty", "0.4", 0, "The penalty to asses if enemy life expectency is more than the negative threshold.");
ConVar mod_player_performance_enemy_life_time_negative_penalty_cap(	"mod_player_performance_enemy_life_time_negative_penalty_cap", "-5", 0, "The maximum penalty that can be assesed for enemy life time.");
ConVar mod_player_performance_fastreload_bonus(						"mod_player_performance_fastreload_bonus", "2", 0, "The boost to performance a player receives for each fast reload.");
ConVar mod_player_performance_fastreload_bonus_cap(					"mod_player_performance_fastreload_bonus_cap", "8", 0, "The maximum boost to performance a player can receive for fast reloads.");
ConVar mod_player_performance_dodgeranger_bonus(					"mod_player_performance_dodgeranger_bonus", "5", 0, "The boost to performance a player receives for dodging a ranger projectile.");
ConVar mod_player_performance_meleekill_bonus(						"mod_player_performance_meleekill_bonus", "0.7", 0, "The boost to performance a player receives for each melee kill.");
ConVar mod_player_performance_meleekill_bonus_cap(					"mod_player_performance_meleekill_bonus_cap", "5", 0, "The maxium performance boost a player can receive for melee kills.");
ConVar mod_player_performance_boomer_kill_early_bonus(				"mod_player_performance_boomer_kill_early_bonus", "5", 0, "The boost to performance a player receives for killing a boomer before it explodes.");
ConVar mod_player_performance_restart_penalty(						"mod_player_performance_restart_penalty", "35", 0, "The penalty assessed if the player restarts a level.");
ConVar mod_player_performance_restart_penalty_timelimit(			"mod_player_performance_restart_penalty_timelimit", "50", 0, "The amount of time before the restart penalty is reduced to zero.");
ConVar mod_player_performance_new_level_modifier(					"mod_player_performance_new_level_modifier", "7", 0, "The amount of the bonus/penalty asssesed on a new level based on players previous perforamnce.  This is a bonus if perf was 3, nothing if 2, penalty if 1.");
ConVar mod_player_performance_new_level_modifier_timelimit(			"mod_player_performance_new_level_modifier_timelimit", "60", 0, "The amount of time before the new level modifier is reduced to zero.");
ConVar mod_player_performance_enemy_killed_bonus(					"mod_player_performance_enemy_killed_bonus", "0.05", 0, "The bonus awarded to a player for each enemy kill.");

void CMOD_Player_Performance_Calculator::PrintDebugString(int offset)
{	
	engine->Con_NPrintf(offset, "Player %s Rating: [%0.2f]", m_DebugName, GetDebugValue());	
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_Health::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_hasCalculatedFullHealth = false;
}
void CMOD_Player_Performance_Calculator_Health::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	m_averageHealth = 0;

	for ( int i=0;i<pGameResource->GetMaxMarineResources();i++ )
	{
		CASW_Marine_Resource *pMR = pGameResource->GetMarineResource(i);
		if ( !pMR )
			continue;

		CASW_Marine *pMarine = pMR->GetMarineEntity();
		if ( !pMarine || pMarine->GetHealth() <= 0 )			
			continue;

		m_averageHealth += pMarine->GetHealth();				
	}

	if (!IsSinglePlayerMode())
	{
		m_averageHealth /= pGameResource->GetMaxMarineResources();
	}

	//store full health for later.
	if ((!m_hasCalculatedFullHealth) && (m_averageHealth > 0))
	{
		m_fullHealth = m_averageHealth;
		m_hasCalculatedFullHealth = true;
	}

	float healthLost = (float)(m_fullHealth - m_averageHealth);
	float percentageHealthLost = (healthLost / m_fullHealth) * 100;
	float penalty = 0;

	if (percentageHealthLost >= mod_player_performance_health_critical_threshold.GetInt())
	{
		penalty = percentageHealthLost * mod_player_performance_health_critical_penalty.GetFloat();
	}	
	else
	{
		 penalty= percentageHealthLost * mod_player_performance_health_normal_penalty.GetFloat();
	}

	//multiply by negative 1 to make it a penalty
	penalty *=-1;

	m_LastCalculatedValue = penalty;
	*performance += m_LastCalculatedValue;
}
void CMOD_Player_Performance_Calculator_Health::PrintExtraDebugInfo(int offset)
{
	int healthLost = m_fullHealth - m_averageHealth;
	char * threshold = "normal";

	if (healthLost >= mod_player_performance_health_critical_threshold.GetInt())
		threshold = "critical";
		
	engine->Con_NPrintf(offset, "Average Health [%i] out of [%i] [%s]", m_averageHealth, m_fullHealth, threshold);
	
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_Accuracy::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	float averageAccuracy = 0;
	float playersThatHaveFired = 0; 

	for ( int i=0;i<pGameResource->GetMaxMarineResources();i++ )
	{
		CASW_Marine_Resource *pMR = pGameResource->GetMarineResource(i);
		if ( !pMR )
			continue;

		float acc = 0;
		if (pMR->m_iPlayerShotsFired > 0)
		{				
				acc = float(pMR->m_iPlayerShotsFired - pMR->m_iPlayerShotsMissed) / float(pMR->m_iPlayerShotsFired);
				acc *= 100.0f;

				//This is used to print debug info.
				m_PlayerZeroAccuracy = acc;				

				averageAccuracy += acc;

				playersThatHaveFired++;			
		}		
	}
		
	if (averageAccuracy == 0)
	{
		//No one has fired a shot, so no modification.
		m_LastCalculatedValue = 0;
		return;
	}

	averageAccuracy /= playersThatHaveFired;
	averageAccuracy *= ACCURACY_MODIFIER;

	if (averageAccuracy < mod_player_performance_accuracy_floor.GetFloat())
		averageAccuracy = mod_player_performance_accuracy_floor.GetFloat();

	float adjustedAccuracy = averageAccuracy - mod_player_performance_accuracy_threshold.GetInt();	

	m_LastCalculatedValue = adjustedAccuracy * mod_player_performance_accuracy_performance_modifier.GetFloat();

	*performance += m_LastCalculatedValue;	
}
void CMOD_Player_Performance_Calculator_Accuracy::PrintExtraDebugInfo(int offset)
{
	char* suffix = "";
	if (m_PlayerZeroAccuracy < mod_player_performance_accuracy_floor.GetFloat())
		suffix = " [FLOOR]";

	engine->Con_NPrintf(offset,"Accuracy [%0.3f]%s", m_PlayerZeroAccuracy, suffix);
}

//////////////////////////////////////////////////////////////////////////////////

//NOT USED - GAME IS ASSUMED TO BE SINGLE PLAYER
void CMOD_Player_Performance_Calculator_FriendlyFire::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	float averageFriendlyFire = 0;

	for ( int i=0;i<pGameResource->GetMaxMarineResources();i++ )
	{
		CASW_Marine_Resource *pMR = pGameResource->GetMarineResource(i);
		if ( !pMR )
			continue;

		if (pMR->m_fFriendlyFireDamageDealt > FRIENDLYFIRE_HANDICAP)
			averageFriendlyFire += pMR->m_fFriendlyFireDamageDealt - FRIENDLYFIRE_HANDICAP;
		else
			averageFriendlyFire += pMR->m_fFriendlyFireDamageDealt;
	}

	if (averageFriendlyFire > 0)
	{
		averageFriendlyFire /= pGameResource->GetMaxMarineResources();
		averageFriendlyFire *= FRIENDLYFIRE_MULTIPLIER;
	}
	
	//return 100-(int)averageFriendlyFire;
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_DirectorStress::OnMissionStarted(float lastLevelRating, int numRetries)
{
	//probably a memory leak
	g_directorStressHistory = new vector<float>();	

	m_HasFinishedCoolingDown = true;			
	m_CoolDownStartTime = 0;
	m_previousStressOfPlayers = 0;
	m_LastCalculatedValue = 0;
	m_CoolDownTimeLeft = 0;
}

float CMOD_Player_Performance_Calculator_DirectorStress::CalculateAverageStress(CASW_Game_Resource *pGameResource){
	float stress = 0;

	for ( int i=0;i<pGameResource->GetMaxMarineResources();i++ )
	{
		CASW_Marine_Resource *pMR = pGameResource->GetMarineResource(i);
		if ( !pMR )
			continue;

		stress += (pMR->GetIntensity()->GetCurrent() * 100.0 * STRESS_MULTIPLIER);
	}
	
	if (!IsSinglePlayerMode() && pGameResource->GetMaxMarineResources() > 0)
		stress /= pGameResource->GetMaxMarineResources();

	return stress;
}

float CMOD_Player_Performance_Calculator_DirectorStress::CalculateStressPenalty(float averageStress, float previousStress)
{
	float stressPenalty = mod_player_performance_director_stress_penalty.GetInt();

	if (averageStress >= mod_player_performance_director_stress_threshold.GetInt())
	{
		//Don't let cool down time start ticking down yet.
		m_CoolDownStartTime = gpGlobals->curtime;
		m_HasFinishedCoolingDown = false;

		if (averageStress >= mod_player_performance_director_stress_critical_threshold.GetInt())
		{
			//assess the higher penalty
			stressPenalty = mod_player_performance_director_stress_critical_penalty.GetInt();
		}
	}

	if (!m_HasFinishedCoolingDown)
	{
		//Degrade stressPenalty based on cooldown_time
		m_CoolDownTimeLeft = mod_player_performance_director_stress_cooldown_time.GetInt() -
				(gpGlobals->curtime - m_CoolDownStartTime);		
	
		if (m_CoolDownTimeLeft > 0)
		{
			stressPenalty *= (m_CoolDownTimeLeft / 
				mod_player_performance_director_stress_cooldown_time.GetInt());		

			//Mutliply by -1, this is a penalty
			stressPenalty *= -1;
		}
		else
		{		
			stressPenalty = 0;
			//set m_CoolDownTimeLeft to zero for nice debug output
			m_CoolDownTimeLeft = 0;

			m_HasFinishedCoolingDown = true;
		}
	}
	else
	{
		stressPenalty = 0;
	}
	
	return stressPenalty;
}

void CMOD_Player_Performance_Calculator_DirectorStress::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	m_averageStressOfPlayers = CalculateAverageStress(pGameResource);
	
	m_LastCalculatedValue = CalculateStressPenalty(m_averageStressOfPlayers, m_previousStressOfPlayers);

	if (!isEndOfLevel)
	{
		//Don't factor director stress into End of Level calculations
		*performance += m_LastCalculatedValue;	
	}

	m_previousStressOfPlayers = m_averageStressOfPlayers;
}

void CMOD_Player_Performance_Calculator_DirectorStress::PrintExtraDebugInfo(int offset)
{	
	engine->Con_NPrintf(offset, "Current Stress: [%0.3f] Cool Down Time Left: [%0.3f]", m_averageStressOfPlayers, m_CoolDownTimeLeft);	
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_PlayTime::OnMissionStarted(float lastLevelRating, int numRetries){
	m_MissionStartTime = gpGlobals->curtime;
	m_LastCalculatedValue = 0.0f;
}

void CMOD_Player_Performance_Calculator_PlayTime::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	//This is only factored in at the end of the mission, because I don't know a good
	//way to track mission progress, as the layouts are random.
	if (!isEndOfLevel)
		return;

	float totalPlayTime = gpGlobals->curtime - m_MissionStartTime;
	int timeAgainstPar = mod_player_performance_playtime_par.GetInt() - totalPlayTime;

	m_LastCalculatedValue =  timeAgainstPar / (float)mod_player_performance_playtime_divisor_bonus.GetInt();

	*performance +=m_LastCalculatedValue;
}

void CMOD_Player_Performance_Calculator_PlayTime::PrintExtraDebugInfo(int offset)
{
	float totalPlayTime = gpGlobals->curtime - m_MissionStartTime;	
	engine->Con_NPrintf(offset, "Total Mission Play Time: [%0.3f]", totalPlayTime);	
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_AlienLifeTime::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_totalAlienLifeTime = 0;
	m_numberOfAliensKilled = 0;
	m_LastCalculatedValue = 0;
}

void CMOD_Player_Performance_Calculator_AlienLifeTime::Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info )
{	
	float total_alive_time = gpGlobals->curtime - pAlien->GetModSpawnTime();	

	 m_totalAlienLifeTime += total_alive_time;
	 m_numberOfAliensKilled++;
}
void CMOD_Player_Performance_Calculator_AlienLifeTime::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	if (m_numberOfAliensKilled == 0 )
		return;

	float averageLifeExpectency = m_totalAlienLifeTime / m_numberOfAliensKilled;

	if (averageLifeExpectency <= mod_player_performance_enemy_life_time_positive_threshold.GetFloat())
	{
		m_LastCalculatedValue = averageLifeExpectency * mod_player_performance_enemy_life_time_positive_bonus.GetFloat();
	}
	else if (averageLifeExpectency >= mod_player_performance_enemy_life_time_negative_threshold.GetFloat())
	{
		//multiply by -1, as this is a penlaty
		m_LastCalculatedValue = -1 * averageLifeExpectency * mod_player_performance_enemy_life_time_negative_penalty.GetFloat();

		if (m_LastCalculatedValue < mod_player_performance_enemy_life_time_negative_penalty_cap.GetFloat())
			m_LastCalculatedValue = mod_player_performance_enemy_life_time_negative_penalty_cap.GetFloat();

	}	
	else
	{
		m_LastCalculatedValue =  0.0f;
	}

	*performance +=m_LastCalculatedValue;
}

void CMOD_Player_Performance_Calculator_AlienLifeTime::PrintExtraDebugInfo(int offset)
{
	char * cap = "";

	if (m_LastCalculatedValue == mod_player_performance_enemy_life_time_negative_penalty_cap.GetFloat())
		cap = " [CAP]";
	engine->Con_NPrintf(offset, "Aliens Killed [%i] Total Life Time: [%0.3f]%s", m_numberOfAliensKilled, m_totalAlienLifeTime, cap);	
}

//////////////////////////////////////////////////////////////////////////////////

CMOD_Player_Performance_Calculator_FastReload::CMOD_Player_Performance_Calculator_FastReload(bool bIsSignlePlayerMode)
		:CMOD_Player_Performance_Calculator(bIsSignlePlayerMode)
{
	m_DebugName = "Fast Reload";		

	ListenForGameEvent("fast_reload");	
}

CMOD_Player_Performance_Calculator_FastReload::~CMOD_Player_Performance_Calculator_FastReload()
{
	StopListeningForAllEvents();
}

void CMOD_Player_Performance_Calculator_FastReload::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_NumberOfFastReloads = 0;	
}

void CMOD_Player_Performance_Calculator_FastReload::FireGameEvent(IGameEvent * event)
{
	const char * type = event->GetName();
	
	if ( Q_strcmp(type, "fast_reload") == 0 )
	{
		m_NumberOfFastReloads++;
	}	
}

void CMOD_Player_Performance_Calculator_FastReload::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	m_LastCalculatedValue = (float)(m_NumberOfFastReloads * mod_player_performance_fastreload_bonus.GetInt());

	if (m_LastCalculatedValue >  mod_player_performance_fastreload_bonus_cap.GetFloat())
		m_LastCalculatedValue =  mod_player_performance_fastreload_bonus_cap.GetFloat();

	*performance += m_LastCalculatedValue;
}

void CMOD_Player_Performance_Calculator_FastReload::PrintExtraDebugInfo(int offset)
{
	char * cap = "";

	if (m_LastCalculatedValue == mod_player_performance_fastreload_bonus_cap.GetFloat())
		cap = " [CAP]";

	engine->Con_NPrintf(offset, "Number of Fast Reloads: [%i]%s", m_NumberOfFastReloads, cap);	
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_DodgeRanger::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_HasDodgedRanger = false;
	m_LastCalculatedValue = 0;
}

void CMOD_Player_Performance_Calculator_DodgeRanger::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	if (!m_HasDodgedRanger)
	{
		for ( int i=0;i<pGameResource->GetMaxMarineResources();i++ )
		{
			CASW_Marine_Resource *pMR = pGameResource->GetMarineResource(i);
			if ( !pMR )
				continue;

			CASW_Marine *pMarine = pMR->GetMarineEntity();
			if ( !pMarine || pMarine->GetHealth() <= 0 )
				continue;

			if (!pMarine->GetMarineResource())
				continue;

			if (pMarine->GetMarineResource()->m_bDodgedRanger)
			{
				m_HasDodgedRanger = true;
				m_LastCalculatedValue = mod_player_performance_dodgeranger_bonus.GetInt();
			}
		}
	}

	*performance +=m_LastCalculatedValue;
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_MeleeKills::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_numberOfAliensKilled = 0;
	m_numberOfMeleeKills = 0;

	m_LastCalculatedValue = 0.0f;
	
}

void CMOD_Player_Performance_Calculator_MeleeKills::Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info )
{		
	 m_numberOfAliensKilled++;

	 if (!Q_strcmp(info.GetAmmoName(), "asw_marine"))
	 {
		 m_numberOfMeleeKills++;	
	 }
}

void CMOD_Player_Performance_Calculator_MeleeKills::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	if (m_numberOfAliensKilled > 0 )
	{
		m_LastCalculatedValue = 
			(float)m_numberOfMeleeKills * 
			mod_player_performance_meleekill_bonus.GetFloat();

		if (m_LastCalculatedValue > mod_player_performance_meleekill_bonus_cap.GetFloat())
			m_LastCalculatedValue = mod_player_performance_meleekill_bonus_cap.GetFloat();
	}	

	*performance +=m_LastCalculatedValue;
}

void CMOD_Player_Performance_Calculator_MeleeKills::PrintExtraDebugInfo(int offset)
{
	char * cap = "";

	if (m_LastCalculatedValue == mod_player_performance_meleekill_bonus_cap.GetFloat())
		cap = " [CAP]";

	engine->Con_NPrintf(offset, "Melee Kills [%i] Total Kills: [%i]%s", m_numberOfMeleeKills, m_numberOfAliensKilled, cap);	
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_BoomerKillEarly::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_HasBoomerKillEarly = false;
	m_LastCalculatedValue = 0;
}

void CMOD_Player_Performance_Calculator_BoomerKillEarly::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	if (!m_HasBoomerKillEarly)
	{
		for ( int i=0;i<pGameResource->GetMaxMarineResources();i++ )
		{
			CASW_Marine_Resource *pMR = pGameResource->GetMarineResource(i);
			if ( !pMR )
				continue;

			CASW_Marine *pMarine = pMR->GetMarineEntity();
			if ( !pMarine || pMarine->GetHealth() <= 0 )
				continue;

			if (!pMarine->GetMarineResource())
				continue;

			if (pMarine->GetMarineResource()->m_bKilledBoomerEarly)
			{
				m_HasBoomerKillEarly = true;
				m_LastCalculatedValue = mod_player_performance_boomer_kill_early_bonus.GetInt();
			}
		}
	}

	*performance +=m_LastCalculatedValue;
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_RestartPenalty::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_MissionStartTime = gpGlobals->curtime;
	m_LastCalculatedValue = 0;

	if (numRetries > 1)		
	{
		m_LastCalculatedValue = mod_player_performance_restart_penalty.GetFloat();
	}	
}

void CMOD_Player_Performance_Calculator_RestartPenalty::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	if (m_LastCalculatedValue != 0)
	{
		float penaltyTimeLeft = mod_player_performance_restart_penalty_timelimit.GetInt() -
			(gpGlobals->curtime - m_MissionStartTime);

		float percentageOfPenaltyToAssess = 
			penaltyTimeLeft / 
			mod_player_performance_restart_penalty_timelimit.GetInt();

		if (percentageOfPenaltyToAssess < 0.001)
			percentageOfPenaltyToAssess = 0;

		//Multiply by -1 because this is a penalty
		m_LastCalculatedValue =  -1 * mod_player_performance_restart_penalty.GetFloat() *
			percentageOfPenaltyToAssess;
	}

	

	*performance +=m_LastCalculatedValue;
}

void CMOD_Player_Performance_Calculator_RestartPenalty::PrintExtraDebugInfo(int offset)
{
	float timeleft = 0;
	
	if (m_LastCalculatedValue != 0)
	{
		timeleft = gpGlobals->curtime - m_MissionStartTime;
		timeleft = mod_player_performance_restart_penalty_timelimit.GetInt() - timeleft;

		if (timeleft < 0)
			timeleft = 0;
	}
	engine->Con_NPrintf(offset, "Restart penalty time left [%0.1f]", timeleft);	
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_NewLevelModifier::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_MissionStartTime = gpGlobals->curtime;

	m_LastCalculatedValue = 0;
	if (numRetries == 1)
	{
		if (lastLevelRating == 3)
			m_LastCalculatedValue = mod_player_performance_new_level_modifier.GetFloat();
		else if (lastLevelRating ==1)
			m_LastCalculatedValue = -1 * mod_player_performance_new_level_modifier.GetFloat();
	}
}

void CMOD_Player_Performance_Calculator_NewLevelModifier::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	if (m_LastCalculatedValue != 0)
	{
		float penaltyTimeLeft = 
			mod_player_performance_restart_penalty_timelimit.GetInt() -
			(gpGlobals->curtime - m_MissionStartTime);

		if (penaltyTimeLeft < 0)
		{
			m_LastCalculatedValue = 0;
		}
		else
		{
			float percentageOfPenaltyToAssess = 
				penaltyTimeLeft / 
				mod_player_performance_restart_penalty_timelimit.GetInt();

			if (percentageOfPenaltyToAssess < 0.001)
				percentageOfPenaltyToAssess = 0;

			float newValue = mod_player_performance_new_level_modifier.GetFloat() *
				percentageOfPenaltyToAssess;

			if (m_LastCalculatedValue > 0)
				m_LastCalculatedValue = newValue;
			else 
				m_LastCalculatedValue = -1 * newValue;
		}
	}

	*performance +=m_LastCalculatedValue;
}

void CMOD_Player_Performance_Calculator_NewLevelModifier::PrintExtraDebugInfo(int offset)
{
	float timeleft = 0;
	
	if (m_LastCalculatedValue != 0)
	{
		timeleft = timeleft = gpGlobals->curtime - m_MissionStartTime;
		timeleft = mod_player_performance_new_level_modifier_timelimit.GetInt() - timeleft;

		if (timeleft < 0)
			timeleft = 0;
	}

	engine->Con_NPrintf(offset, "New Level Modifier time left [%0.1f]", timeleft);	
}

//////////////////////////////////////////////////////////////////////////////////

void CMOD_Player_Performance_Calculator_EnemyKillBonus::OnMissionStarted(float lastLevelRating, int numRetries)
{
	m_EnemiesKilled = 0;
}

void CMOD_Player_Performance_Calculator_EnemyKillBonus::Event_AlienKilled( CBaseEntity *pAlien, const CTakeDamageInfo &info ){
	m_EnemiesKilled++;
}

void CMOD_Player_Performance_Calculator_EnemyKillBonus::UpdatePerformance(float * performance, bool isEndOfLevel, CASW_Game_Resource *pGameResource)
{
	m_LastCalculatedValue = mod_player_performance_enemy_killed_bonus.GetFloat() *
		m_EnemiesKilled;
	
	*performance +=m_LastCalculatedValue;
}