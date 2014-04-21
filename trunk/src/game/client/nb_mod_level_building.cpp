#include "cbase.h"
#include "nb_mod_level_building.h"
#include "nb_button.h"
#include "asw_model_panel.h"
#include <vgui/ISurface.h>
#include "c_asw_player.h"
#include "asw_weapon_parse.h"
#include "asw_equipment_list.h"
#include "vgui_controls/AnimationController.h"
#include "nb_header_footer.h"
#include "c_asw_player.h"
#include <vgui/VGUI.h>
#include "vgui_controls/ImagePanel.h"

#include "missionchooser/iasw_mission_chooser_source.h"
#include "missionchooser/iasw_random_missions.h"
#include "missionchooser/iasw_map_builder.h"
#include "missionchooser/imod_level_builder.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

CNB_MOD_Level_Building::CNB_MOD_Level_Building( vgui::Panel *parent, const char *name, C_ASW_Player *pPlayer, bool missionSuccess ) : BaseClass( parent, name )
{
	m_pBanner = new CNB_Gradient_Bar( this, "Banner" );
	m_pTitle = new vgui::Label( this, "Title", "" );
	m_pPercentCompleteLabel = new vgui::Label( this, "PercentCompleteLabel", "" );	
	m_pStatusLabel = new Label(this, "StatusLabel", "");		
	m_pWorkingAnim = new vgui::ImagePanel(this, "WorkingAnim");
			
	m_pPlayer = pPlayer;
	m_bMissionSuccess = missionSuccess;
	
	m_flLastEngineTime = Plat_FloatTime();
}

void CNB_MOD_Level_Building::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	
	LoadControlSettings( "resource/ui/nb_mod_level_building.res" );

	if (!m_bMissionSuccess)
		m_pTitle->SetText("#asw_building_level_mission_fail");

	SetAlpha( 0 );
	vgui::GetAnimationController()->RunAnimationCommand( this, "alpha", 255, 0, 0.5f, vgui::AnimationController::INTERPOLATOR_LINEAR );

	CLocalPlayerFilter filter;
	C_BaseEntity::EmitSound( filter, -1 /*SOUND_FROM_LOCAL_PLAYER*/, "ASWInterface.SelectWeapon" );
}

void CNB_MOD_Level_Building::PerformLayout()
{
	KeyValues *pKV = new KeyValues( "ItemModelPanel" );
	pKV->SetInt( "fov", 20 );
	pKV->SetInt( "start_framed", 0 );
	pKV->SetInt( "disable_manipulation", 1 );
	
	BaseClass::PerformLayout();
}

void CNB_MOD_Level_Building::PaintBackground()
{
	// fill background
	float flAlpha = 200.0f / 255.0f;
	vgui::surface()->DrawSetColor( Color( 16, 32, 46, 230 * flAlpha ) );
	vgui::surface()->DrawFilledRect( 0, 0, GetWide(), GetTall() );	
}

void CNB_MOD_Level_Building::OnThink()
{

	//Give some time for the window to open and render
	if (m_flLastEngineTime + 0.3f < Plat_FloatTime())
	{
		//Update progress will set m_flLastEngineTime
		UpdateWorkingAnim();
		UpdateProgress();	
	}


	BaseClass::OnThink();
}

void CNB_MOD_Level_Building::UpdateWorkingAnim()
{
	//if ( m_pWorkingAnim )
	{
		// clock the anim at 10hz
		float time = Plat_FloatTime();
		if ( ( m_flLastEngineTime + 0.1f ) < time )
		{
			m_flLastEngineTime = time;
			m_pWorkingAnim->SetFrame( m_pWorkingAnim->GetFrame() + 1 );
			
		}
	}
}

void CNB_MOD_Level_Building::UpdateProgress()
{	
	missionchooser->MapBuilder()->Update(Plat_FloatTime());
	float progress = missionchooser->MapBuilder()->GetProgress() * 100;
						
	wchar_t buffer[512];
	swprintf(buffer,sizeof(buffer),L"%d%%", (int)progress );
						
	m_pPercentCompleteLabel->SetText( buffer );

	m_pStatusLabel->SetText( missionchooser->MapBuilder()->GetStatusMessage());

	if (progress == 100)
	{
		OnLevelBuildingComplete();
		
	}
}

void CNB_MOD_Level_Building::OnLevelBuildingComplete()
{
	MarkForDeletion();
	if (m_bMissionSuccess)
		m_pPlayer->CampaignSaveAndShow();
	else
		m_pPlayer->RequestMissionRestart();

}
