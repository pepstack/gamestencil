//--------------------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the Animate System.
// author		: XJ
// date			: 2014 / 6 11
// version		: 1.0
//---------------------------------------------------------------------------------
#pragma once
#include"SystemManager.h"

namespace ShapeWar
{
	class AnimateSystem:public System
	{
	public:
		AnimateSystem(int priority);
		~AnimateSystem();

	public:
		void enter();
		void excute(float dt);
		void exit();
	};
};