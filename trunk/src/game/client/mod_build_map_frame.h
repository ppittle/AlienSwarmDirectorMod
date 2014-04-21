#ifndef _INCLUDED_MOD_BUILD_MAP_FRAME_H
#define _INCLUDED_MOD_BUILD_MAP_FRAME_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>

namespace vgui
{
	class Button;
	class ProgressBar;
};

class C_ASW_Player;
class IASW_Map_Builder;

class CMOD_Build_Map_Frame : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CMOD_Build_Map_Frame, vgui::Frame );
public:
	CMOD_Build_Map_Frame( vgui::Panel *pParent, const char *pElementName, C_ASW_Player *pPlayer );
	virtual ~CMOD_Build_Map_Frame();

	virtual void PerformLayout();
	virtual void OnThink();

	IASW_Map_Builder* GetMapBuilder() { return m_pMapBuilder; }
	//void BuildMap();
	//void SetRunMapAfterBuild( bool bRunMap ) { m_bRunMapAfterBuild = bRunMap; }
	//void SetEditMapAfterBuild( bool bEditMap, const char *szMapEditFilename );

private:
	void UpdateProgress();

	vgui::Label *m_pStatusLabel;
	vgui::ProgressBar *m_pProgressBar;
	float m_fCloseWindowTime;	
	IASW_Map_Builder *m_pMapBuilder;
	char m_szMapEditFilename[MAX_PATH];

	C_ASW_Player *m_pPlayer;
};

#endif // _INCLUDED_MOD_BUILD_MAP_FRAME_H