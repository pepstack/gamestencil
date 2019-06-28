#ifndef __ROUND_SCENE_H__
#define __ROUND_SCENE_H__

#include "cocos2d.h"

class Round : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(Round);

	//override the update method
	void update(float dt);

private:
	cocos2d::CCLabelTTF * m_LifeTTF ;
	cocos2d::CCLabelTTF * m_ScoreTTF ;
};

#endif  // __ROUND_SCENE_H__