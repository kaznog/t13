//
//  AboutScene.cpp
//  ccs2dx_v3tutorial001
//
//  Created by noguchika on 2014/02/21.
//
//

#include "AboutScene.h"
#include "TitleScene.h"
#include "TitleSceneLoader.h"

USING_NS_CC;
using namespace cocosbuilder;

AboutScene::AboutScene()
{
    
}

AboutScene::~AboutScene()
{
    
}

cocos2d::SEL_MenuHandler AboutScene::onResolveCCBCCMenuItemSelector(cocos2d::Object * pTarget, const char* pSelectorName)
{
    log("AboutScene ResolveCCBCCMEnuItemSelector");
    return NULL;
}

cocos2d::extension::Control::Handler AboutScene::onResolveCCBCCControlSelector(cocos2d::Object * pTarget, const char* pSelectorName)
{
    log("AboutScene ResolveCCBCCControlSelector");
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", AboutScene::onBack);
    return NULL;
}

bool AboutScene::onAssignCCBMemberVariable(cocos2d::Object * pTarget, const char * pMemberVariableName, cocos2d::Node * node)
{
    log("AboutScene AssignCCBMemberVariable");
    return false;
}

void AboutScene::onBack(cocos2d::Object* sender, cocos2d::extension::Control::EventType pControlEvent)
{
    switch(pControlEvent) {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::DRAG_INSIDE:
            break;
        case Control::EventType::DRAG_OUTSIDE:
            break;
        case Control::EventType::DRAG_ENTER:
            break;
        case Control::EventType::DRAG_EXIT:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
        {
            log("AboutScene onBack");
            this->CCBChangeScene(S_TITLE_CLASSNAME, TitleSceneLoader::loader(), S_CCB_MAINMENU);
        }
            break;
        case Control::EventType::TOUCH_UP_OUTSIDE:
            break;
        case Control::EventType::TOUCH_CANCEL:
            break;
        case Control::EventType::VALUE_CHANGED:
            break;
        default:
            assert(false); // OH SHIT!
    }
}
