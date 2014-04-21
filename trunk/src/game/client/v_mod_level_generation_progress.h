#ifndef MOD_level_generation_progress_H__
#define MOD_level_generation_progress_H__

#include "cbase.h"
#include "swarm\gameui\swarm\basemodui.h"
#include "vgui/IScheme.h"
#include "const.h"
#include "loadingtippanel.h"
#include "swarm\gameui\swarm\vloadingprogress.h"



// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class MOD_level_generation_progress : public BaseModUI::LoadingProgress	
{
	DECLARE_CLASS_SIMPLE( MOD_level_generation_progress,  BaseModUI::LoadingProgress );
	MOD_level_generation_progress( vgui::Panel *parent, const char *panelName, LoadingWindowType eLoadingType );
	~MOD_level_generation_progress();
};

class MOD_level_generation_progress_launcher
{
private:
	MOD_level_generation_progress_launcher();
	

	public:
		static MOD_level_generation_progress_launcher* m_singleton;
		static MOD_level_generation_progress_launcher* GetSingleton();
		void BeginLevelGeneration();
		void ShowLevelGenerationProgressWindow();
};

#endif