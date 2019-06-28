//----------------------------------------------------------------------
// declaration	: Copyright (c), by XJ ,2014 . All right reserved .
// brief		: This file will define the Info that the game will use.
// author		: XJ
// date			: 2014 / 6 / 11
// version		: 1.0
//----------------------------------------------------------------------
#pragma once
namespace ShapeWar
{
	class GameInfo
	{
	private:
		GameInfo();
		~GameInfo();
		
	public:
		static GameInfo* getGameInfo();

	public:
		unsigned int CUR_SCORE  ;
		unsigned int MAX_HEALTH_PLAYER ;
		unsigned int CUR_HEALTH_PLAYER ;
	};
};