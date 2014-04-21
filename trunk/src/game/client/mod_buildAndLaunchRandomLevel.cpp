#include "cbase.h"

#include "mod_buildAndLaunchRandomLevel.h"
#include "../missionchooser/mod_level_builder.h"
#include "missionchooser/iasw_map_builder.h"

#include "convar.h"
#include <vgui_controls/Frame.h>
#include "ienginevgui.h"

#include "asw_build_map_frame.h" //- not having causes a problem in imaterial.h

#include "missionchooser/iasw_random_missions.h" // - required


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

 
extern void LevelBuilder();
extern void BuildLevel( const char *szMissionFile, const int iDifficultLevel, const char * szOutputFile);

void MOD_BuildAndLaunchRandomLevel(const CCommand &args)
{	
	const char * layoutFile = "\\maps\\customOuput3.layout";

	missionchooser->modLevel_Builder()->BuildMapFromLayoutFile("tilegen\\new_missions\\mod_escapeOnly.txt", layoutFile, "mod"); 
		
	//missionchooser->MapBuilder()->ScheduleMapBuild(layoutFile, 0.6f);
	//missionchooser->modLevel_Builder()->CompileAndExecuteLevel(layoutFile);
	
	//missionchooser->RandomMissions()->BuildAndLaunchRandomLevel
	//	->BuildAndLaunchRandomLevel();
}

static ConCommand MOD_BuildAndLaunchRandomLevel2("mod_BuildAndLaunchRandomLevel", MOD_BuildAndLaunchRandomLevel, "PJ - First Server Command.", 0  );

void MOD_LaunchTileGen(const CCommand &args)
{	
	/*
	vgui::VPANEL GameUIRoot = enginevgui->GetPanel( PANEL_GAMEUIDLL );	
	vgui::Frame *pFrame = dynamic_cast<vgui::Frame*>( missionchooser->RandomMissions()->CreateTileGenFrame( NULL ) );

	pFrame->SetParent( GameUIRoot );
	pFrame->MoveToCenterOfScreen();
	pFrame->Activate();	
	*/
	Msg("Disabled");
}

static ConCommand MOD_BuildAndLaunchRandomLevel3("mod_LaunchTileGen", MOD_LaunchTileGen, "Start Tile Gen.", FCVAR_CHEAT );

