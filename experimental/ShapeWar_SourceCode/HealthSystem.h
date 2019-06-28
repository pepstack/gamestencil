//----------------------------------------------------------------------------
// decalration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the Health System.
// author		: XJ
// date			: 2014 / 6 / 10
// version		: 1.0
//---------------------------------------------------------------------------
#pragma once
#include"SystemManager.h"

namespace ShapeWar
{
	class HealthSystem:public System
	{
	public:
		HealthSystem(int priority);
		~HealthSystem();

	public:
		void enter();
		void excute(float dt);
		void exit();
	};
};