//
//  OptionScene.h
//
//  Created by kaznog on 2014/02/20.
//
//

#ifndef __ccs2dx_v3tutorial001__OptionScene__
#define __ccs2dx_v3tutorial001__OptionScene__

#include "cocos2d.h"
#include "CCBLoaderLayer.h"
#include "Constant.h"

USING_NS_CC;

class OptionScene : public CCBLoaderLayer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    CREATE_FUNC(OptionScene);
    virtual void dispose(){}
    
    void onMusicToggle(cocos2d::Object * sender);
    void onBack(cocos2d::Object * sender);
};

#endif /* defined(__ccs2dx_v3tutorial001__OptionScene__) */
