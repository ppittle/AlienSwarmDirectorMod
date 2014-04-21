#ifndef _INCLUDED_MOD_LEVEL_BUILDER_H
#define _INCLUDED_MOD_LEVEL_BUILDER_H
#ifdef _WIN32
#pragma once
#endif


#include "../public/missionchooser/imod_level_builder.h"


class CASW_Map_Builder;
class CMapLayout;
class CLayoutSystem;
class KeyValues;

//#include "layout_system\tilegen_layout_system.h"
//#include "vgui\tilegen_pages.h"

class MOD_Level_Builder : public IMOD_Level_Builder
{
	//static MOD_Level_Builder* g_LevelBuilderSingleton;

	bool g_IsBuildingLevel;
	void SetIsBuildingLevel(bool value);
	CASW_Map_Builder *m_pASWMapBuilder;
	
public:
	//static MOD_Level_Builder* LevelBuilder();
	MOD_Level_Builder();
	~MOD_Level_Builder();

	virtual bool IsBuildingLevel();
	virtual void BuildMapForMissionFromLayoutFile( const char *szMissionName, const int iDifficultLevel, bool bCompileLevel=true);
	virtual void BuildMapFromLayoutFile( const char *szMissionRuleFile, const char *szOutputLayoutFile, const char *szThemeName, bool bCompileLevel=true);	
	virtual void CompileLevel(const char * szLayoutFile);
protected:
	CMapLayout *GenerateMapLayout(CLayoutSystem *pLayoutSystem, KeyValues *pMissionSettings);
	
};


#endif //_INCLUDED_MOD_LEVEL_BUILDER_H