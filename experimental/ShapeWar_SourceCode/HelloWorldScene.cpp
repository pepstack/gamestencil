#include "HelloWorldScene.h"
#include "Round.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());
		CCSprite* title = CCSprite::create("Title_real.png");
		title->setPosition(ccp(160, 400));
		this->addChild(title);

		CCMenuItemImage * pItem = CCMenuItemImage::create("StartGame.png","StartGame_Touched.png",this,
			menu_selector(HelloWorld::menuStartCallback));
		CCMenu* pMenu = CCMenu::create(pItem,NULL);
		pMenu->setPosition(ccp(160, 200));
		this->addChild(pMenu);
        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuStartCallback(CCObject* pSender)
{
    // "Start game" menu item clicked
    // create a scene. it's an autorelease object
    CCScene *pScene = Round::scene();

    // run
	CCDirector * pDirector = CCDirector::sharedDirector();
    pDirector->replaceScene(pScene);
}

