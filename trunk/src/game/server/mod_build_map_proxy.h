#ifndef _INCLUDED_MOD_BUILD_MAP_PROXY_H
#define _INCLUDED_MOD_BUILD_MAP_PROXY_H

#include "cbase.h"

class CMOD_build_map_proxy : public CBaseEntity
{
public:
	DECLARE_CLASS(CMOD_build_map_proxy, CBaseEntity );	// setup some macros
	DECLARE_SERVERCLASS();  // make this entity networkable
 
	int UpdateTransmitState();	
 
public:
	// public networked member variables:
	CNetworkVar( int, m_iPlayerPerformance ); 
};

#endif //_INCLUDED_MOD_BUILD_MAP_PROXY_H