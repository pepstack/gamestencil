#include"GameInfo.h"
using namespace ShapeWar ;

GameInfo::GameInfo()
	:CUR_SCORE(0),
	MAX_HEALTH_PLAYER(5),
	CUR_HEALTH_PLAYER(0)
{

}

GameInfo::~GameInfo()
{

}

GameInfo* GameInfo::getGameInfo()
{
	static GameInfo gameInfo ;
	return &gameInfo ;
}// end for getGameInfo