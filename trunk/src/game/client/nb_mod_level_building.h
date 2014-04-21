#ifndef _INCLUDED_NB_MOD_LEVEL_LOADING_H
#define _INCLUDED_NB_MOD_LEVEL_LOADING_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui/VGUI.h>
#include <vgui_controls/EditablePanel.h>

class vgui::Label;
class CNB_Button;
class CASW_Model_Panel;
class CNB_Gradient_Bar;
class C_ASW_Player;

class CNB_MOD_Level_Building : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CNB_MOD_Level_Building, vgui::EditablePanel );
public:
	CNB_MOD_Level_Building( vgui::Panel *parent, const char *name, C_ASW_Player *pPlayer, bool missionSuccess );
	
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual void PerformLayout();	
	virtual void OnThink();
	virtual void PaintBackground();	
	void UpdateWorkingAnim();
	void UpdateProgress();
	void OnLevelBuildingComplete();
	
	vgui::Label	*m_pTitle;
	vgui::Label *m_pPercentCompleteLabel;
	vgui::Label *m_pStatusLabel;
	CNB_Gradient_Bar *m_pBanner;	
	vgui::ImagePanel *m_pWorkingAnim;

	C_ASW_Player *m_pPlayer;

	//Controls how the Windows progress game flow.
	bool m_bMissionSuccess;

	float m_flLastEngineTime;	
};

#endif // _INCLUDED_NB_MOD-LEVEL_LOADING_H
