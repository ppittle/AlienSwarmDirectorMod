#ifndef _INCLUDED_NB_MOD_LEVEL_LOADINGBUTTON_H
#define _INCLUDED_NB_MOD_LEVEL_LOADINGBUTTON_H
#ifdef _WIN32
#pragma once
#endif


#include <vgui/VGUI.h>
#include <vgui_controls/Button.h>
#include "swarm/vgui/nb_button.h"


class CNB_MOD_Level_Loading_Button : public CNB_Button
{
	DECLARE_CLASS_SIMPLE( CNB_Button, vgui::Button );

public:

	CNB_MOD_Level_Loading_Button(Panel *parent, const char *panelName, const char *text, Panel *pActionSignalTarget=NULL, const char *pCmd=NULL);
	//CNB_MOD_Level_Loading_Button(Panel *parent, const char *panelName, const wchar_t *text, Panel *pActionSignalTarget=NULL, const char *pCmd=NULL);
	virtual ~CNB_MOD_Level_Loading_Button();
	virtual void OnThink();
	// Take the string and looks it up in the localization file to convert it to unicode
	virtual void SetText(const char *tokenName);

	// Set unicode text directly
	virtual void SetText(const wchar_t *unicodeString, bool bClearUnlocalizedSymbol = false );

	virtual void OnCommand(const char * command);

private:
	bool IsNextMissionLevelGenerationComplete();
	void UpdateText();

	void SetTextInternal(const char *tokenName);
	void SetTextInternal(const wchar_t *unicodeString, bool bClearUnlocalizedSymbol = false );

	char* m_originalText;
	char* m_originalCommand;
	float m_fNextStageTime;
	float m_fProgress;
	bool m_hasSetOriginalText;
};

#endif //_INCLUDED_NB_MOD_LEVEL_LOADINGBUTTON_H