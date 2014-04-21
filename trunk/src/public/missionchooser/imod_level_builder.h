#ifndef IMOD_LEVEL_BUILDER_H
#define IMOD_LEVEL_BUILDER_H

#if defined( COMPILER_MSVC )
#pragma once
#endif

class IMOD_Level_Builder
{
public:
	virtual bool IsBuildingLevel() = 0;
		virtual void BuildMapForMissionFromLayoutFile( const char *szMissionName, const int iDifficultLevel, bool bCompileLevel=true) = 0;
	virtual void BuildMapFromLayoutFile( const char *szMissionRuleFile, const char *szOutputLayoutFile, const char *szThemeName, bool bCompileLevel=true) =0;	
	virtual void CompileLevel(const char * szLayoutFile) = 0;
};


#endif //IMOD_LEVEL_BUILDER_H