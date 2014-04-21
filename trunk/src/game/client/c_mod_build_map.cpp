#include "cbase.h"
#include "vgui/ivgui.h"
#include <vgui/vgui.h>
#include <vgui/ischeme.h>
#include <vgui_controls/Controls.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/TextImage.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/ProgressBar.h>
#include "convar.h"
#include "asw_build_map_frame.h"
#include <vgui/isurface.h>
#include "ienginevgui.h"
#include "missionchooser/iasw_mission_chooser_source.h"
#include "missionchooser/iasw_random_missions.h"
#include "missionchooser/iasw_map_builder.h"
#include "asw_loading_panel.h"

// includes needed for the creating of a new process and handling its output
#pragma warning( disable : 4005 )
#include <windows.h>
#include <iostream>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void CC_MOD_Build_Map( const CCommand &args )
{
	if ( args.ArgC() < 2 )
	{
		Msg("Usage: mod_build_map <map name>");
		return;
	}

	vgui::VPANEL GameUIRoot = enginevgui->GetPanel( PANEL_GAMEUIDLL );
	CASW_Build_Map_Frame *pFrame = new CASW_Build_Map_Frame( NULL, "BuildMapFrame" );
	pFrame->SetParent( GameUIRoot );
	pFrame->Activate();

	int nArgC = args.ArgC();
	for ( int i = 1; i < nArgC; i++ )
	{
		if ( !Q_stricmp( args.Arg(i), "connecting" ) )
		{
			pFrame->SetRunMapAfterBuild( false );
		}
		else if ( !Q_stricmp( args.Arg(i), "edit" ) && i + 1< nArgC )
		{
			pFrame->SetEditMapAfterBuild( true, args.Arg( i + 1 ) );		// argument immediately after "edit" is the .vmf name
		}
	}

	if ( GASWLoadingPanel() )
	{
		if ( !Q_strnicmp( args[1], "maps/", 5 ) || !Q_strnicmp( args[1], "maps\\", 5 ) )
		{
			GASWLoadingPanel()->SetLoadingMapName( args[1] + 5 );
		}
		else
		{
			GASWLoadingPanel()->SetLoadingMapName( args[1] );
		}
	}

	//PJ - This line prevents the map from loading.
	pFrame->SetRunMapAfterBuild( false );
	pFrame->GetMapBuilder()->ScheduleMapBuild( args[1], Plat_FloatTime() + 0.6f );	// give some time for the window to open and appear
}
static ConCommand asw_build_map("mod_build_map", CC_MOD_Build_Map, 0 );


