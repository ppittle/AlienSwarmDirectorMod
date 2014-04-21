#ifndef _INCLUDED_MOD_SPAWNER_H
#define _INCLUDED_MOD_SPAWNER_H
#ifdef _WIN32
#pragma once
#endif

#include "asw_spawner.h"

class CMOD_Spawner : public CASW_Spawner
{
public:
	DECLARE_CLASS( CMOD_Spawner, CASW_Spawner );
	DECLARE_DATADESC();

	CMOD_Spawner();
	virtual ~CMOD_Spawner();

	virtual void			Spawn();

	//inputs
	void InputResetNumAliens( inputdata_t &inputdata );
protected:
	int m_nOriginalNumAliens;
};

#endif