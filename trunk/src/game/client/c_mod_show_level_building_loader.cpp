#include "cbase.h"
#include "convar.h"
#include "../gameui/swarm/basemodpanel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace BaseModUI;

void CC_MOD_Show_Level_Building_Loader(const CCommand &args)
{
	//CBaseModPanel::GetSingleton().OpenWindow(WT_LOADINGPROGRESS, CBaseModPanel::GetSingleton().GetWindow(WT_MAINMENU));
}

static ConCommand asw_build_map("mod_show_level_building_loader", CC_MOD_Show_Level_Building_Loader, 0 );