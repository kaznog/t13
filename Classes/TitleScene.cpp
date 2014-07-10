//
//  TitleScene.cpp
//
//  Created by kaznog on 2014/02/20.
//
//

#include "TitleScene.h"
#include "HelloWorldScene.h"
#include "OptionScene.h"
#include "AboutScene.h"
#include "AboutSceneLoader.h"
#include "ScoreScene.h"

USING_NS_CC;
using namespace cocosbuilder;

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TitleScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

TitleScene::TitleScene()
: _SpineNode(nullptr)
{
    
}

TitleScene::~TitleScene()
{
    
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void TitleScene::onEnterTransitionDidFinish()
{
    log("TitleScene onEnterTransitionDidFinish");
    auto SpineObject = _SpineNode->getLetter(8);
    auto a_delay = DelayTime::create(6);
    auto a_tint = TintTo::create(0.5, 0, 255, 0);
    auto a_rotate = RotateBy::create(4, 360);
    auto a_rep = Repeat::create(a_rotate, 1000);
    auto a_seq = Sequence::create(a_delay, a_tint, a_delay->clone(), a_rep, NULL);
    
    SpineObject->runAction(a_seq);
}

cocos2d::SEL_MenuHandler TitleScene::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPlay", TitleScene::onPlayClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onOptions", TitleScene::onOptionsClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onScores", TitleScene::onScoresClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onAbout", TitleScene::onAboutClicked);
    return NULL;
}

cocos2d::extension::Control::Handler TitleScene::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    log("TitleScene ResolveCCBCCControlSelector");
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onExit", TitleScene::onExitClicked);
    return NULL;
}

bool TitleScene::onAssignCCBMemberVariable(cocos2d::Ref * pTarget, const char * pMemberVariableName, cocos2d::Node * pNode)
{
    log("TitleScene AssignCCBMemberVariable");
    // V3.1.1になってLabelBMFontではなく、Labelで処理するようです
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "titleLabel", Label *, _SpineNode);
    return true;
}

void TitleScene::onPlayClicked(cocos2d::Ref * sender)
{
    auto scene = HelloWorld::createScene(new GameState());
    auto director = Director::getInstance();
    director->replaceScene(TransitionFade::create(0.5, scene));
}

void TitleScene::onOptionsClicked(cocos2d::Ref * sender)
{
    auto scene = OptionScene::createScene();
    auto director = Director::getInstance();
    director->replaceScene(TransitionFade::create(0.5, scene));
}

void TitleScene::onScoresClicked(cocos2d::Ref * sender)
{
    auto scene = ScoreScene::createScene();
    auto director = Director::getInstance();
    director->replaceScene(TransitionFade::create(0.5, scene));
}

void TitleScene::onAboutClicked(cocos2d::Ref * sender)
{
    this->CCBChangeScene(S_ABOUT_CLASSNAME, AboutSceneLoader::loader(), S_CCB_ABOUT);
}

void TitleScene::onExitClicked(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent)
{
    this->dispose();
    auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    audioEngine->stopAllEffects();
    audioEngine->stopBackgroundMusic();
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
