//-----------------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the Input System .
// author		: XJ
// date			: 2014 / 6 / 9
// version		: 1.0
//-----------------------------------------------------------------------------
#pragma once

#include"SystemManager.h"

namespace ShapeWar
{
	class InputSystem:public System
	{
	public:
		InputSystem(int _priority);
		~InputSystem();

	public:
		void enter();
		void excute(float dt);
		void exit();
	};
};