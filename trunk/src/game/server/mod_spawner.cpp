#include "cbase.h"
#include "baseentity.h"
#include "mod_spawner.h"
#include "asw_spawner.h"
//#include "asw_simpleai_senses.h"
#include "asw_gamerules.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( mod_spawner, CMOD_Spawner );

BEGIN_DATADESC( CMOD_Spawner )
	DEFINE_KEYFIELD( m_nMaxLiveAliens,			FIELD_INTEGER,	"MaxLiveAliens" ),
	DEFINE_KEYFIELD( m_nNumAliens,				FIELD_INTEGER,	"NumAliens" ),	
	DEFINE_KEYFIELD( m_bInfiniteAliens,			FIELD_BOOLEAN,	"InfiniteAliens" ),
	DEFINE_KEYFIELD( m_flSpawnInterval,			FIELD_FLOAT,	"SpawnInterval" ),
	DEFINE_KEYFIELD( m_flSpawnIntervalJitter,	FIELD_FLOAT,	"SpawnIntervalJitter" ),
	DEFINE_KEYFIELD( m_AlienClassNum,			FIELD_INTEGER,	"AlienClass" ),
	DEFINE_KEYFIELD( m_SpawnerState,			FIELD_INTEGER,	"SpawnerState" ),

	DEFINE_INPUTFUNC( FIELD_VOID,	"SpawnOneAlien",	InputSpawnAlien ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"StartSpawning",	InputStartSpawning ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"StopSpawning",		InputStopSpawning ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"ToggleSpawning",	InputToggleSpawning ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"ResetNumAliens",	InputResetNumAliens ),

	DEFINE_OUTPUT( m_OnAllSpawned,		"OnAllSpawned" ),
	DEFINE_OUTPUT( m_OnAllSpawnedDead,	"OnAllSpawnedDead" ),

	DEFINE_FIELD( m_nCurrentLiveAliens, FIELD_INTEGER ),
	DEFINE_FIELD( m_AlienClassName,		FIELD_STRING ),

	DEFINE_THINKFUNC( SpawnerThink ),
END_DATADESC()

CMOD_Spawner::CMOD_Spawner() : BaseClass()
{	
}

CMOD_Spawner::~CMOD_Spawner()
{

}

void CMOD_Spawner::Spawn(){
	m_nOriginalNumAliens = m_nNumAliens;	
	BaseClass::Spawn();
}

void CMOD_Spawner::InputResetNumAliens( inputdata_t &inputdata )
{
	m_nNumAliens = m_nOriginalNumAliens;
	SetSpawnerState(SST_WaitForInputs);
}