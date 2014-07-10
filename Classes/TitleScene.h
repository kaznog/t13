//
//  TitleScene.h
//
//  Created by kaznog on 2014/02/20.
//
//

#ifndef __ccs2dx_v3tutorial001__TitleScene__
#define __ccs2dx_v3tutorial001__TitleScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"
#include "CCBLoaderLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;


class TitleScene
: public CCBLoaderLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TitleScene, create);

    TitleScene();
    ~TitleScene();

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref * pTarget, const char * pMemberVariableName, cocos2d::Node * pNode);
    void onPlayClicked(cocos2d::Ref * sender);
    void onOptionsClicked(cocos2d::Ref * sender);
    void onScoresClicked(cocos2d::Ref * sender);
    void onAboutClicked(cocos2d::Ref * sender);
    void onExitClicked(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onEnterTransitionDidFinish();

    virtual void dispose(){}
    
    Label* _SpineNode;
};

#endif /* defined(__ccs2dx_v3tutorial001__TitleScene__) */
