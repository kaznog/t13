//
//  AboutScene.h
//
//  Created by kaznog on 2014/02/21.
//
//

#ifndef __ccs2dx_v3tutorial001__AboutScene__
#define __ccs2dx_v3tutorial001__AboutScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"
#include "CCBLoaderLayer.h"
#include "Constant.h"

USING_NS_CC;
USING_NS_CC_EXT;

class AboutScene
: public CCBLoaderLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(AboutScene, create);
    
    AboutScene();
    virtual ~AboutScene();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Object * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Object * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::Object * pTarget, const char * pMemberVariableName, cocos2d::Node * node);
    
    void onBack(cocos2d::Object * sender, cocos2d::extension::Control::EventType pControlEvent);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    virtual void dispose(){}
    
};

#endif /* defined(__ccs2dx_v3tutorial001__AboutScene__) */
