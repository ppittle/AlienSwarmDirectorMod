#include "cbase.h"
#include "nb_mod_level_loading_button.h"
#include "swarm/vgui/nb_button.h"
#include "vgui/ISurface.h"
#include "missionchooser/iasw_mission_chooser_source.h"
#include "missionchooser/iasw_random_missions.h"
#include "missionchooser/iasw_map_builder.h"
#include "missionchooser/imod_level_builder.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

DECLARE_BUILD_FACTORY_DEFAULT_TEXT( CNB_MOD_Level_Loading_Button, CNB_MOD_Level_Loading_Button );

CNB_MOD_Level_Loading_Button::CNB_MOD_Level_Loading_Button(Panel *parent, const char *panelName, const char *text, Panel *pActionSignalTarget, const char *pCmd)
: CNB_Button( parent, panelName, text, pActionSignalTarget, pCmd )
{
	// == MANAGED_MEMBER_CREATION_START: Do not edit by hand ==
	// == MANAGED_MEMBER_CREATION_END ==
	
	m_originalText = (char *)malloc(sizeof(char) * 256);
	Q_snprintf(m_originalText, sizeof(m_originalText), "%s", text);

	m_originalCommand = (char *)malloc(sizeof(char) * 256);
	Q_snprintf(m_originalCommand, sizeof(m_originalCommand), "%s", pCmd);
	SetCommand("");	

	m_fNextStageTime = 0;
	m_hasSetOriginalText = false;
	m_fProgress = 0;
}
/*
CNB_MOD_Level_Loading_Button::CNB_MOD_Level_Loading_Button(Panel *parent, const char *panelName, const wchar_t *text, Panel *pActionSignalTarget, const char *pCmd)
: CNB_Button( parent, panelName, text, pActionSignalTarget, pCmd )
{
}
*/

CNB_MOD_Level_Loading_Button::~CNB_MOD_Level_Loading_Button()
{
}

bool CNB_MOD_Level_Loading_Button::IsNextMissionLevelGenerationComplete()
{
	if (missionchooser->MapBuilder()->IsBuildingMission() || 
		missionchooser->modLevel_Builder()->IsBuildingLevel() )
		return false;
	else
		return true;	
}

void CNB_MOD_Level_Loading_Button::UpdateText()
{
	if (m_hasSetOriginalText)
		return;

	if (gpGlobals->curtime <= m_fNextStageTime)
	{
		missionchooser->MapBuilder()->Update(Plat_FloatTime());
		m_fProgress  = missionchooser->MapBuilder()->GetProgress();
	}

	if (m_fProgress == 1.0f)
	{
		SetTextInternal(m_originalText);
		m_hasSetOriginalText = true;
	}
	else
	{
		wchar_t buffer[512];
		swprintf(buffer,sizeof(buffer),L"Building Level...%d", (int)(m_fProgress * 100) );
						
		SetTextInternal( buffer );
	}
}

void CNB_MOD_Level_Loading_Button::OnThink()
{
	BaseClass::OnThink();

	if (gpGlobals->curtime <= m_fNextStageTime)
	{
		return;
	}

	if (IsNextMissionLevelGenerationComplete())
	{
		SetCommand(m_originalCommand);
	}

	UpdateText();

	m_fNextStageTime = gpGlobals->curtime + 1.5f;	
}

// Take the string and looks it up in the localization file to convert it to unicode
void CNB_MOD_Level_Loading_Button::SetText(const char *tokenName)
{
	BaseClass::SetText(tokenName);
	GetText(m_originalText, sizeof(m_originalText));	
	UpdateText();
}

// Set unicode text directly
void CNB_MOD_Level_Loading_Button::SetText(const wchar_t *unicodeString, bool bClearUnlocalizedSymbol )
{
	
	BaseClass::SetText(unicodeString, bClearUnlocalizedSymbol);
	
	GetText(m_originalText, sizeof(m_originalText));	
	UpdateText();
}

void CNB_MOD_Level_Loading_Button::SetTextInternal(const char *tokenName)
{
	BaseClass::SetText(tokenName);
}
void CNB_MOD_Level_Loading_Button::SetTextInternal(const wchar_t *unicodeString, bool bClearUnlocalizedSymbol )
{
	BaseClass::SetText(unicodeString, bClearUnlocalizedSymbol);

}
void CNB_MOD_Level_Loading_Button::OnCommand(const char * command)
{
	BaseClass::OnCommand(command);
}

