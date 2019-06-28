//-----------------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the Boundary Check system.
// author		: XJ
// date			: 2014 / 6 / 10
// version		: 1.0
//------------------------------------------------------------------------------
#pragma once
#include"SystemManager.h"

namespace ShapeWar
{
	class BoundaryCheckSystem:public System
	{
	public:
		BoundaryCheckSystem(int priority);
		~BoundaryCheckSystem();

	public:
		void enter();
		void excute(float dt);
		void exit();
	};
};