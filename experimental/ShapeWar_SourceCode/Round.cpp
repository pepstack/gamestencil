#include "Round.h"
#include "RenderSystem.h"
#include "MovementSystem.h"
#include "EntityCreator.h"
#include "EntityMananger.h"
#include "InputSystem.h"
#include "CreatorSystem.h"
#include "BoundaryCheckSystem.h"
#include "CollidDetectionSystem.h"
#include "HealthSystem.h"
#include "AnimateSystem.h"
#include "GameInfo.h"
#include "AudioSystem.h"
using namespace ShapeWar ;
using namespace cocos2d;

CCScene* Round::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        Round * layer = Round::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Round::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
        CC_BREAK_IF(! CCLayer::init());
		this->scheduleUpdate();

		//Load the background
		CCSprite* back = CCSprite::create("Background.png");
		back->setPosition(ccp(160,240));
		this->addChild(back, -1);

		//Create the UI
		char buffer[64];
		sprintf(buffer,"LIFE:%d", GameInfo::getGameInfo()->MAX_HEALTH_PLAYER);
		m_LifeTTF = CCLabelTTF::create(buffer, "Thonburi_Bold",15);
		m_LifeTTF->setAnchorPoint(ccp(0, 1.0f));
		m_LifeTTF->setColor(cocos2d::ccc3(0.0f, 0.0f, 0.0f));
		this->addChild(m_LifeTTF, 10);
		m_LifeTTF->setPosition(ccp(10, 480 - 10 ));
		
		sprintf(buffer,"SCORE:%d",0);
		m_ScoreTTF = CCLabelTTF::create(buffer,"Thonburi_Bold",15);
		m_ScoreTTF->setAnchorPoint(ccp(0, 1.0f));
		m_ScoreTTF->setColor(cocos2d::ccc3(0.0f, 0.0f, 0.0f));
		this->addChild(m_ScoreTTF, 10);
		m_ScoreTTF->setPosition(ccp(10, 480 - 25 ));

		//Load the animation
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Explosion_Pack_0.plist");

		//Load music
		AudioSystem::sharedAudioSystem()->init();
		AudioSystem::sharedAudioSystem()->loadSound("Shoot.wav",false);
		AudioSystem::sharedAudioSystem()->loadSound("Background.mid",true);
		AudioSystem::sharedAudioSystem()->loadSound("Hurt.wav",false);
		AudioSystem::sharedAudioSystem()->loadSound("Failure0000.mp3",false);
		AudioSystem::sharedAudioSystem()->playSound("Background.mid");

		//Get the entity manager
		EntityManager::getEntityManager()->registComponents(7);

		//Get the system manager
		SystemManager* pManager = SystemManager::getSystemManager() ;

		//Create the Render System
		System* pRender = new RenderSystem(1, this);
		pManager->addSystem(pRender);

		//Create the Movement System
		System* pMovement = new MovementSystem(2);
		pManager->addSystem(pMovement);

		//Create the Input System
		System* pInput = new InputSystem(5);
		pManager->addSystem(pInput);

		//Create the BoundaryCheck System
		System* pBoundary = new BoundaryCheckSystem(8);
		pManager->addSystem(pBoundary);

		//Create the Creator System
		System* pCreator = new CreatorSystem(4);
		pManager->addSystem(pCreator);

		//Create the Collid Detection System
		System* pCollidDetection = new CollidDetectionSystem(7);
		pManager->addSystem(pCollidDetection);

		//Create the Animate System
		System* pAnimate = new AnimateSystem(6);
		pManager->addSystem(pAnimate);

		//Create the Health System
		System* pHealth = new HealthSystem(3);
		pManager->addSystem(pHealth);

		//Create player
		EntityCreator::createPlayer();

        bRet = true;
    } while (0);

    return bRet;
}

void Round::update(float dt)
{
	//Get the system manager
	SystemManager* pManager = SystemManager::getSystemManager() ;
	pManager->update(dt);

	//Update the UI
	char buffer[64];
	sprintf(buffer,"Life:%d",GameInfo::getGameInfo()->CUR_HEALTH_PLAYER);
	m_LifeTTF->setString(buffer);

	sprintf(buffer,"Score:%d",GameInfo::getGameInfo()->CUR_SCORE);
	m_ScoreTTF->setString(buffer);

	if(GameInfo::getGameInfo()->CUR_HEALTH_PLAYER <= 0)
	{
		static bool isEnd = false ;
		if(isEnd == false)
		{
			pManager->pause();
			CCLayerColor * color = CCLayerColor::create(ccc4(0.0f,0.0f, 0.0f, 255.0f));
			color->setOpacity(100.0f);
			this->addChild(color, 11);

			sprintf(buffer,"Oops ! \n Your highest score is: %d", GameInfo::getGameInfo()->CUR_SCORE);
			CCLabelTTF * lose = CCLabelTTF::create(buffer,"Thonburi_Bold",20);
			lose->setPosition(ccp(160, 240));
			color->addChild(lose, 1);
			isEnd = true ;

			AudioSystem::sharedAudioSystem()->stopSound("Background.mid");
			AudioSystem::sharedAudioSystem()->playSound("Failure0000.mp3");
		}
	}
}// end for update