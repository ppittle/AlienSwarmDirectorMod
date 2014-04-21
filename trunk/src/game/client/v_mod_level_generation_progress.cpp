#include "cbase.h"
#include "v_mod_level_generation_progress.h"

#include "swarm\gameui\swarm\vloadingprogress.h"
#include "EngineInterface.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/ProgressBar.h"
#include "vgui/ISurface.h"
#include "vgui/ILocalize.h"
#include "vgui_controls/Image.h"
#include "vgui_controls/ImagePanel.h"
#include "gameui_util.h"
#include "KeyValues.h"


#include "missionchooser/iasw_mission_chooser_source.h"
#include "missionchooser/iasw_random_missions.h"
#include "missionchooser/iasw_map_builder.h"
#include "missionchooser/imod_level_builder.h"
#include "swarm\gameui\swarm\basemodpanel.h"



// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace BaseModUI;
using namespace vgui;

MOD_level_generation_progress::MOD_level_generation_progress( vgui::Panel *parent, const char *panelName, LoadingWindowType eLoadingType )	
	: LoadingProgress(parent, panelName, eLoadingType) 
{
}

MOD_level_generation_progress::~MOD_level_generation_progress(){
}



/*
called by missioncompletepanel
void C_ASW_Player::CampaignSaveAndShow()
{
	engine->ClientCmd("cl_campaignsas");	
}
*/
MOD_level_generation_progress_launcher* MOD_level_generation_progress_launcher::m_singleton;

MOD_level_generation_progress_launcher::MOD_level_generation_progress_launcher(){}

MOD_level_generation_progress_launcher* MOD_level_generation_progress_launcher::GetSingleton()
{
	if (!m_singleton)
		m_singleton = new MOD_level_generation_progress_launcher();

	return m_singleton;
}

void MOD_level_generation_progress_launcher::ShowLevelGenerationProgressWindow()
{
	/*
	MOD_level_generation_progress *levelGeneration = static_cast<MOD_level_generation_progress *>(
		CBaseModPanel::GetSingleton().OpenWindow(WT_MOD_LEVEL_GENERATION, 0));

	levelGeneration->SetProgress(10);
	*/
}

void MOD_level_generation_progress_launcher::BeginLevelGeneration()
{
	
}
