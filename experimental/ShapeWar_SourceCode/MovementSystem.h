//------------------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define Movement System.
// author		: XJ
// date			: 2014 / 6 / 9 
// version		: 1.0
//------------------------------------------------------------------------------
#pragma once
#include"SystemManager.h"

namespace ShapeWar
{
	class MovementSystem:public System
	{
	public:
		MovementSystem(int _priority);
		~MovementSystem();

	public:
		void enter();
		void excute(float dt);
		void exit();
	};
};